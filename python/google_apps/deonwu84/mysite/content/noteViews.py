# -*- coding: utf-8 -*-
from models import *
import logging, datetime
#from datetime.tzinfo

def IP(r): return r.META["REMOTE_ADDR"]

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
    if cate.parent_category:
        o['parentId'] = cate.parent_category.cate_id
    else:
        o['parentId'] = ""
    o['name'] = cate.name
    return o

def update_category(r, user, parent, cate_id, name, nodeType, updateDate=''):
    logging.debug("update_category: user=%s, parent=%s, cate_id=%s, name=%s, nodeType=%s, ippaddr=%s" % (
                 user, parent, cate_id, name, nodeType, IP(r)))
    
    result = {"status": 'OK', }
    
    owner = _load_user(user)
    sub_cate = _load_category(owner, cate_id)
    if parent:
        cate = _load_category(owner, parent)
        sub_cate.parent_category = cate
    
    sub_cate.name = name
    sub_cate.nodeType = int(nodeType)
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
    if text is not None:
        message.text = db.Text(text)
        message.update_date = datetime.datetime.now()
        message.put()
    else:
        result['text'] = message.text
    
    #result['tzname'] = message.update_date.tzname() 
    #result['date'] = message.update_date.strftime("%Y-%m-%d %H:%M:%S")
            
    return result

def delete(r, user, cate_id, nodeType='cate'):
    logging.debug("delete: user=%s, cate_id=%s, nodeType=%s ippaddr=%s" % (
                  user, cate_id, nodeType, IP(r)))
    
    owner = _load_user(user)
    if nodeType == 'cate':
        _load_category(owner, cate_id).delete()
    else:
        _load_message(owner, cate_id).delete()
    
    return {'status': 'OK'}

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
        category.put()
    return category

def _load_user(name='', ):
    if not name: return None    
    user = ContentUser.all().filter("name =", name).get()
    if user is None:
        user = ContentUser(name=name,)
        user.put()
    return user
