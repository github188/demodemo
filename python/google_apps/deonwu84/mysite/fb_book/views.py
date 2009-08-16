# -*- coding: utf-8 -*-
import logging
from google.appengine.api import memcache
from book import initalize_book, list_book_details, add_fb_record, list_user_details
from models import * 
from datetime import datetime
import hashlib, uuid

def list_book(r, book='', key="", date="", limit=60, user_name_in_row='N'):
    cur_book = book_session(r)
    if cur_book is None and str(book).isdigit():
        cur_book = BookAccount.load(book)
        if cur_book.book_key != key: cur_book = None
    if cur_book:
        fb_views = list_book_details(cur_book, date, limit, 12, user_name_in_row)
        host = (r.META["SERVER_NAME"])
        port = (r.META["SERVER_PORT"])
        port = port != '80' and ":%s" % port or ""
        account_list_url = "http://%s%s/fb_book/list_book?book=%s&key=%s" % (host, port, 
                cur_book.id, cur_book.book_key)
                
        return ("fb_book_detail_list.html", {"fb_views": fb_views, 
                                             "account_list_url":account_list_url,
                                             "fb_date": datetime.now().strftime("%Y-%m-%d"),
                                             "show_menu": book_session(r) is not None,
                                             "book_key": cur_book.book_key
                                             })
        
    return ("redirect:/fb_book/index", )

def update_key(r, ):
    cur_book = book_session(r)
    cur_book.book_key = hashlib.md5(str(uuid.uuid4())).hexdigest()
    cur_book.put()
    #book_session(r, remove=True)
    book_session(r, cur_book)
    return ("redirect:/fb_book/list_book", )
    

def user_fb_list(r, id='', key="", limit=60):
    fb_list = None
    try:
        user = BookUser.load(id)
        if user and user.parent().book_key == key:
            fb_list = list_user_details(user.parent(), user, limit)
    except Exception, e:
        logging.error("user_fb_list: %s" % e)
        
    if fb_list is None:
        return ("redirect:/fb_book/index", )
    
    return ("fb_book_user_detail_list.html", {"current_user":user, 
                                              "user_fb_list":fb_list, 
                                              "fb_book": user.parent(),
                                              })

def demo_book(r, book='', key=""):
    return ("fb_book_detail_list_demo.html", {})

def action(r, date, fb_type="", money="", master="", user_list="", other="", comment=""):
    cur_book = book_session(r)
    logging.info("fb action:date=%s, fb_type=%s, money=%s, master=%s, user_list=%s, other=%s, comment=%s" %
                 (date, fb_type, money, master, user_list, other, comment))
    if cur_book is not None:
        error_msg = add_fb_record(cur_book, date, fb_type, money, 
                                  master, user_list, other, comment)
        if error_msg:
            return {"status":"ERR", "message": error_msg}
        return {"status":"OK", "inner_html": ""}
    else:
        return {"status":"ERR", "message": "登录账户过期，或没有登录。"}

def index(r, ):
    if book_session(r) is not None: 
        return ("redirect:/fb_book/list_book", )
    return ("fb_book_index.html", {})
    
def default_view(r,):
    return index(r)

def login(r, name='', admin_user='', admin_password=''):
    if book_session(r) is not None: return ("redirect:/fb_book/index", )
    if r.method != "POST": return ("fb_book_index.html", {})
    
    book = BookAccount.all().filter('name =', name).get()
    old_admin_password = admin_password
    admin_password = hashlib.md5(admin_password).hexdigest()
    if book is None:
        if admin_user and admin_password:
            book = BookAccount(name=name, admin_user=admin_user, 
                               admin_password=admin_password)
            book.put()
            initalize_book(book)
            book_session(r, book)
            return ("redirect:/fb_book/index", )
        else:
            error = "创建新账户时管理账户和密码不能为空!"
            
    elif book is not None:
        error = ""
        if book.admin_user != admin_user:
            error = "错误的管理账户!"
        elif book.admin_password != admin_password:
            error = "错误的管理密码!"
        else:
            book_session(r, book)
            return ("redirect:/fb_book/index", )
            
    return ("fb_book_index.html", {"error": error, 
                                   'name':name,
                                   'admin_user':admin_user,
                                   'admin_password':old_admin_password})

def logout(r):
    book_session(r, None, True)
    return ("redirect:/fb_book/index", )
    
def book_session(r, book=None, remove=None):
    sessionId = r.COOKIES.get('sessionid', None)
    if not sessionId: return None
    logging.debug("cur book account session id:%s" % sessionId)
    
    if book is not None:
        logging.debug("save current book:%s" % book)
        memcache.set(key=sessionId, value=book, time=3600, namespace='fb_book')
    
    if remove is True:
        memcache.delete(key=sessionId, namespace='fb_book')
        #r.COOKIES['sessionid'] = ''
        return None
    
    book = memcache.get(sessionId, namespace='fb_book')
    logging.debug("retrieve current book:%s" % book)
    return book
