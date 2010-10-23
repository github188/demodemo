# -*- coding: utf-8 -*-
from models import *
import logging, datetime
from google.appengine.api import users
#from datetime.tzinfo

def IP(r): return r.META["REMOTE_ADDR"]

def cur_user():
    return "deon"
    u = users.get_current_user()
    return u and u.email().split("@")[0].lower() or None

def list_category(r, user, cate_id):
    logging.debug("list_category: user=%s, cate_id=%s, ippaddr=%s" % (
                 user, cate_id, IP(r)))
    owner = _load_user(user)
    category = _load_category(owner, cate_id)
    cate_list = UserCategory.all().ancestor(owner).filter("parent_category =", category).fetch(100)
    
    result_list = [ _category2json(e) for e in cate_list]
    return result_list

def get_category(r, user, cate_id):
    logging.debug("get_category: user=%s, cate_id=%s, ippaddr=%s" % (
                 user, cate_id, IP(r)))
    owner = _load_user(user)
    cate_list = UserCategory.all().ancestor(owner).filter("cate_id =", cate_id).fetch(1)
    result_list = [ _category2json(e) for e in cate_list]
    return result_list

def _category2json(cate):
    o = {}
    o['id'] = str(cate.id)
    o['cate_id'] = str(cate.cate_id)
    o['nodeType'] = str(cate.nodeType)
    o['name'] = cate.name
    o['update_date'] = cate.update_date.strftime("%Y-%m-%d %H:%M:%S")
    o['order_by'] = cate.order_by or ""
    o['position'] = cate.position or 0
    if cate.parent_category:
        o['parentId'] = cate.parent_category.cate_id
    else:
        o['parentId'] = ""
    o['name'] = cate.name
    return o

def update_category(r, user, parent, cate_id, name, nodeType, updateDate='', order_by='', position='0'):
    logging.debug("update_category: user=%s, parent=%s, cate_id=%s, name=%s, nodeType=%s, ippaddr=%s" % (
                 user, parent, cate_id, name, nodeType, IP(r)))
    
    if str(user).strip().lower() != cur_user():        
        return {"status": 'AUTH_ERROR', "msg": "%s!=%s" %(user, cur_user())}
    
    result = {"status": 'OK', }
    
    owner = _load_user(user)
    sub_cate = _load_category(owner, cate_id)
    if parent:
        cate = _load_category(owner, parent)
        sub_cate.parent_category = cate
    
    sub_cate.name = name
    sub_cate.nodeType = int(nodeType)
    sub_cate.order_by = order_by or ""
    sub_cate.position = position or "0"
    if updateDate:
        sub_cate.update_date = datetime.datetime.strptime(updateDate, "%Y-%m-%d %H:%M:%S") #.now()
    else:
        sub_cate.update_date = datetime.datetime.now()
    sub_cate.put()

    result['date'] = sub_cate.update_date.strftime("%Y-%m-%d %H:%M:%S")
    return result
    
def sync_message(r, user, message_id, text=None):
    logging.debug("sync_message: user=%s, message_id=%s, sync=%s ippaddr=%s" % (
                  user, message_id, text and 'up' or 'down', IP(r)))
    
    owner = _load_user(user)
    message = _load_message(owner, message_id)
    result = {"status": 'OK', }
    try:
        if text is not None:
            if str(user).strip().lower() != cur_user():        
                return {"status": 'AUTH_ERROR', }
            if db.Text(text) != message.text: 
                _save_history_version(owner, message)
                message.version = message.version + 1
                message.text = db.Text(text)
                message.update_date = datetime.datetime.now()
                message.put()
        else:
            result['text'] = message.text
    except BaseException, e:
        result['status'] = "ERR"
        result['msg'] = str(e)
    return result
    
def _save_history_version(owner, msg):
    message = HistoryVersion(parent=owner, message_id=msg.message_id)
    message.version = msg.version
    message.text = msg.text
    message.tags = msg.tags
    logging.info("save version:%s, id:%s" % (msg.version, msg.message_id))
    message.put()

def delete(r, user, cate_id, nodeType='cate', mode=''):
    logging.debug("delete: user=%s, cate_id=%s, nodeType=%s ippaddr=%s" % (
                  user, cate_id, nodeType, IP(r)))
    if str(user).strip().lower() != cur_user():
        if mode == 'web':
            return ("redirect:%s" % users.create_login_url(r.path), )
        else:     
            return {"status": 'AUTH_ERROR', "msg": "%s!=%s" %(user, cur_user())}
    
    owner = _load_user(user)
    removed = _load_category(owner, "__removed__")
    if nodeType == 'cate':
        cate = _load_category(owner, cate_id)
        cate.parent_category = removed
        cate.update_date = datetime.datetime.now()
        cate.put()
    if mode == 'web':
        return ("redirect:/note/recycle?user=%s" % user)
    return {'status': 'OK'}

def restore(r, user, cate_id, ):
    logging.debug("delete: user=%s, cate_id=%s, ippaddr=%s" % (
                  user, cate_id, IP(r)))
    if str(user).strip().lower() != cur_user():
        return ("redirect:%s" % users.create_login_url(r.path), )
    owner = _load_user(user)
    removed = _load_category(owner, "removed")
    cate = _load_category(owner, cate_id)
    cate.parent_category = removed
    cate.put()
    return ("redirect:/note/recycle?user=%s" % user, )

def force_delete(r, user, cate_id, ):
    logging.debug("delete: user=%s, cate_id=%s, ippaddr=%s" % (
                  user, cate_id, IP(r)))
    if str(user).strip().lower() != cur_user():
        return ("redirect:%s" % users.create_login_url(r.path), )
    owner = _load_user(user)
    _load_category(owner, cate_id).delete();
    _load_message(owner, cate_id).delete();
    return ("redirect:/note/recycle?user=%s" % user, )

def recycle(r, user=''):
    if str(user).strip().lower() != cur_user():
        return ("redirect:%s" % users.create_login_url(r.path), )
    owner = _load_user(user)
    category = _load_category(owner, "__removed__")
    removed_list = UserCategory.all().ancestor(owner).filter("parent_category =", category).order("-update_date").fetch(100)
    cate_list = []
    
    return ("note_book_recycle.html", locals())

def webPage(r, user='', cate_id='index', version="", rollback='N'):
    owner = _load_user(user)
    cate_id = cate_id == 'index' and '0000001' or cate_id 
    category = _load_category(owner, cate_id)
    message = None
    org_msg = None
    
    if category.nodeType == 2:
        category = category.parent_category
        message = _load_message(owner, cate_id)
        message.rollback = False
        
    if str(user).strip().lower() == cur_user() and version:
        org_msg = message
        message = HistoryVersion.all().ancestor(owner).filter("message_id =", cate_id)\
                    .filter("version =", int(version)).get()
        if message:
            message.rollback = True
            if rollback == 'Y':
                _save_history_version(owner, org_msg)
                org_msg.text = message.text
                org_msg.tags = message.tags
                org_msg.version += 1
                org_msg.put()            
    if message and str(user).strip().lower() == cur_user():
        message.version_list = [ e for e in range(message.version) ][-10:]
    
    dir_list = UserCategory.all().ancestor(owner).filter("parent_category =", category) #.fetch(100)
    if category.order_by: dir_list = dir_list.order(category.order_by)
    dir_list = dir_list.fetch(100)
    
    cate_list = [ e for e in dir_list if e.nodeType == 1 ]
    file_list = [ e for e in dir_list if e.nodeType == 2 ]
    parent = None
    if category.parent_category:
        parent = category.parent_category
    
    return ("note_book_web_view.html", locals())

def default_view(r):
    return webPage(r, "test", 'index')

def update_userinfo(r, user):
    pass

def _load_message(owner, message_id): 
    message = ContentMessage.all().ancestor(owner).filter("message_id =", message_id).get()
    if message is None:
        message = ContentMessage(parent=owner, message_id=message_id)
        message.put()
    return message

def _load_category(owner, cate_id):
    category = UserCategory.all().ancestor(owner).filter("cate_id =", cate_id).get()
    if category is None:
        category = UserCategory(parent=owner, cate_id=cate_id)
        category.update_date = datetime.datetime.now()
        category.put()
    return category

def _load_user(name='', ):
    name = str(name).strip().lower();
    if not name: return None
    user = ContentUser.all().filter("name =", name).get()
    if user is None:
        user = ContentUser(name=name,)
        user.put()
    return user

