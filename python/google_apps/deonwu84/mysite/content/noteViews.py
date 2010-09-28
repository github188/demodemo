# -*- coding: utf-8 -*-
from models import *
import logging, datetime

def IP(r): return r.META["REMOTE_ADDR"]

def list_category(r, user, cate_id):
    logging.debug("list_category: user=%s, cate_id=%s, ippaddr=%s" % (
                 user, cate_id, IP(r)))
    owner = _load_user(user)
    category = _load_category(owner, cate_id)
    cate_list = UserCategory.all().ancestor(owner).filter("parent_category =", category).fetch(100)
    
    result_list = []
    for t in cate_list:
        result_list.append({})
        result_list[-1]['id'] = t.id
        result_list[-1]['cate_id'] = t.cate_id
        result_list[-1]['nodeType'] = t.nodeType
        result_list[-1]['name'] = t.name
        result_list[-1]['update_date'] = t.update_date.strftime("%Y-%m-%d %H:%M:%S")
    return result_list

def update_category(r, user, parent, cate_id, name, nodeType, ):
    logging.debug("update_category: user=%s, parent=%s, cate_id=%s, name=%s, nodeType=%s, ippaddr=%s" % (
                 user, parent, cate_id, name, nodeType, IP(r)))
    
    result = {"status": 'OK', }
    
    owner = _load_user(user)
    cate = _load_category(owner, parent)
    sub_cate = _load_category(owner, cate_id)
    
    sub_cate.parent_category = cate
    sub_cate.name = name
    sub_cate.nodeType = int(nodeType)
    sub_cate.update_date = datetime.datetime.now()
    sub_cate.put()

    result['date'] = sub_cate.update_date.strftime("%Y-%m-%d %H:%M:%S")
    return result
    
def sync_message(r, user, message_id, text=''):
    logging.debug("sync_message: user=%s, message_id=%s, sync=%s ippaddr=%s" % (
                  user, message_id, text and 'up' or 'down', IP(r)))
    
    owner = _load_user(user)
    message = _load_message(owner, message_id)
    result = {"status": 'OK', }
    if text:
        message.text = db.Text(text)
        message.update_date = datetime.datetime.now()
        message.put()
    else:
        result['text'] = message.text
    
    result['date'] = message.update_date.strftime("%Y-%m-%d %H:%M:%S")
            
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
