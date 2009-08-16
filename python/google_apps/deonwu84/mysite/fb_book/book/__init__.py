# -*- coding: utf-8 -*-
from mysite.fb_book.models import *
import re
from datetime import datetime
import logging
from google.appengine.ext import db

def initalize_book(book):
    import uuid, hashlib 
    default = BookUser(parent=book, name=u"银行")
    default.put()
    book.account = default
    book.book_key = hashlib.md5(str(uuid.uuid4())).hexdigest()
    book.put()
    
    if BookAccountRecord.all().filter("action =", "dummy").get() is None:        
        dummy_date = datetime.strptime("2001-01-01", "%Y-%m-%d")
        BookAccountRecord(book_date=dummy_date,
                          action='dummy', host='dummy',
                          people='dummy').put()
    if BookUser.all().filter("name =", "dummy").get() is None:
        BookUser(name='dummy').put()
    
    if book.name.startswith("demo"):
        initalize_demo_book(book)
    
    return book

def initalize_demo_book(book):
    logging.warning("initalize_demo_book......%s" % book.name)
    BookUser(parent=book, name=u"张三").put()
    BookUser(parent=book, name=u"李四").put()
    BookUser(parent=book, name=u"王五").put()
    BookUser(parent=book, name=u"孙钱").put()
    BookUser(parent=book, name=u"唐伯虎", discount=1.2).put()
    BookUser(parent=book, name=u"秋香", discount=0.8).put()
    
    def add_fb_record_log(*args):
        logging.warning("demo:%s" % add_fb_record(*args))
    
    add_fb_record_log(book, "2008-01-20", "save_money", "50", "",
                  u"张三,李四,", u"唐伯虎,秋香", u"初始化")
    add_fb_record_log(book, "2008-01-20", "save_money", "100", "",
                  u"张三", "", u"初始化")
    add_fb_record_log(book, "2008-01-23", "expense", "100", "",
                  u"张三,李四", u"唐伯虎", u"外婆家")
    add_fb_record_log(book, "2008-02-24", "expense", "150", u"孙钱",
                  u"张三,王五", u"孙钱", u"外婆家")
    add_fb_record_log(book, "2008-02-23", "expense", "150", u"张三",
                  u"张三,王五", u"唐伯虎", u"外婆家")
    add_fb_record_log(book, "2008-03-23", "transfer", "7", u"唐伯虎",
                  u"张三", "", u"午饭")
    add_fb_record_log(book, "2008-04-23", "expense", "20", "",
                  u"张三 李四", u"唐伯虎", u"外婆家")
    add_fb_record_log(book, "2008-03-23", "transfer", "10", u"唐伯虎",
                  u"张三，李四，秋香", "", u"红包")
    
def add_fb_record(book, date, fb_type="", money="", master="", user_list="", other="", comments=""):
    error = ""
    user_list = re.split(r"[;,\s]", "%s,%s" % (user_list, other))
    user_list = set([ e.strip() for e in user_list if e.strip() ])
    #list book users
    bookusers = BookUser.all().ancestor(book).filter("name IN", list(user_list)).fetch(1000)
    #create new user
    exists_user = [ e.name for e in bookusers ]
    
    date = datetime.strptime(date, "%Y-%m-%d")        
    for name in user_list:
        if name in exists_user: continue
        u = BookUser(parent=book, name=name)
        u.put()
        bookusers.append(u)
    sum_discount = sum([e.discount for e in bookusers])
    
    master = master.strip()
    if master:
        master = BookUser.all().ancestor(book).filter("name =", master).get()
    else:
        master = db.get(book.account.key())
    if not isinstance(master, BookUser): error="not found fb master."
    
    try:
        money = float(money)
    except:
        error = "%s" % money
    
    if error: return error
    
    if fb_type == "expense":
        db.run_in_transaction(_create_fb_record_update_balance,
                              book, date, fb_type, money, sum_discount,
                              master, bookusers, comments
                              )
    elif fb_type == "save_money":
        db.run_in_transaction(_save_money_update_balance,
                              book, date, fb_type, money, bookusers, comments
                              )
    elif fb_type == "transfer":
        db.run_in_transaction(_transfer_update_balance,
                              book, date, fb_type, money, master,
                              bookusers, comments)        
    else:
        error = "Not supported fb action %s" % fb_type
        
    for u in bookusers:
        u.lastupdate = datetime.now()
        u.put()

def _transfer_update_balance(book, date, type, expense, master, 
                            users_list, comments):
    logging.info("transfer: data=%s, master=%s, users_list=%s, comments=%s" %
                 (date, master, [e.name for e in users_list], comments)
                 )
    
    name_list = [e.name for e in users_list]
    name_list.sort()
    people = ",".join(name_list)
    book_record = BookAccountRecord(parent=book, book_date=date, action=type,
                                    host=master.name,
                                    expense=expense * len(users_list),
                                    people=people, comment=comments)
    book_record.put()
    for u in users_list:
        if u.name == master.name: raise RuntimeError, "Can't transfer money to self!"
        u.balance += expense
        BookUserRecord(parent=book, bookrecord=book_record, user=u,
                       expense=expense,
                       balance=u.balance,
                       action=type, ).put()
                       
    master.balance -= expense * len(users_list)
    master.put()
    BookUserRecord(parent=book, bookrecord=book_record, user=master,
                   expense=expense * len(users_list) * -1,
                   balance=master.balance,
                   action=type,
                   comments="%s->%s" % (expense, people)
                   ).put()

def _save_money_update_balance(book, date, type, expense, 
                               users_list, comments):
    name_list = [e.name for e in users_list]
    name_list.sort()
    people = ",".join(name_list)
    book_record = BookAccountRecord(parent=book, book_date=date, action=type,
                                    host=book.account.name,
                                    expense=expense * len(users_list),
                                    people=people, comment=comments)
    book_record.put()
    for u in users_list:
        if u == book.account: raise RuntimeError, "Can't save money in book account!"
        u.balance += expense
        BookUserRecord(parent=book, bookrecord=book_record, user=u,
                       expense=expense,
                       balance=u.balance,
                       action=type, ).put()
                       
    master = book.account
    master.balance += expense * len(users_list)
    master.put()
    BookUserRecord(parent=book, bookrecord=book_record, user=master,
                   expense=expense * len(users_list),
                   balance=master.balance,
                   action=type, ).put()
    

def _create_fb_record_update_balance(book, date, type, expense, sum_discount,
                                     master, 
                                     users_list, comments):
    logging.info("expense: data=%s, master=%s, users_list=%s, comments=%s" %
                 (date, master, [e.name for e in users_list], comments)
                 )
    
    name_list = [e.name for e in users_list]
    name_list.sort()
    people = ",".join(name_list)
    book_record = BookAccountRecord(parent=book, book_date=date, action=type, 
                                    host=master.name,
                                    people=people, 
                                    expense=expense,
                                    comment=comments)
    book_record.put()
    dis_count_expense = expense / sum_discount
    for u in users_list:
        amount = dis_count_expense * u.discount
        u.balance -= amount
        if u.name == master.name:
            u.balance += expense
            amount = amount - expense
        BookUserRecord(parent=book, bookrecord=book_record, user=u,
                       expense=amount * -1,
                       balance=u.balance,
                       action=type, ).put()
        u.put()
    if master.name == book.account.name:
        #logging.info("update book account!")
        master.balance -= expense
        BookUserRecord(parent=book, bookrecord=book_record, user=master,
                       expense=expense * -1,
                       balance=master.balance,
                       action=type, ).put()
        master.put()
    

def list_book_details(book, start_date="", limit=31, user_count=15, user_name_in_row='N'):
    
    if user_name_in_row == 'N':
        bookusers = BookUser.all().ancestor(book).order("-lastupdate").fetch(12)
        bookusers = [ e for e in bookusers if e.name != book.account.name]
        bookusers.sort(lambda x, y: cmp(x.name, y.name))
        fb_list = BookAccountRecord.all().ancestor(book).order("-book_date")\
            .order("-create_date").fetch(limit)
        min_col = 12
    else:
        bookusers = BookUser.all().ancestor(book).filter("name !=", 
            book.account.name).order("name").fetch(limit)
        fb_list = BookAccountRecord.all().ancestor(book).order("-book_date")\
            .order("-create_date").fetch(10)
        min_col = 9
        
    from book_views import BookDetailView
    return BookDetailView(book, bookusers, fb_list, min_col=min_col)

def list_user_details(book, user, limit=100):
    user_action = BookUserRecord.all().ancestor(book).filter("user =", user)\
                    .order("-create_date").fetch(limit)
    user_action.sort(lambda x, y: cmp(y.bookrecord.book_date, x.bookrecord.book_date))
    
    return user_action
    
