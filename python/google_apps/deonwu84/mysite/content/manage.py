# -*- coding: utf-8 -*-
from google.appengine.api import memcache
from models import *
import logging
"""

list mode:
1. list
2. hot
3. new
4. active <-- voted?
5. random

"""
import hashlib
def md5(str): return hashlib.md5(str).hexdigest()

class ErrorCode:
    ERROR_VALUE = 'err_value'
    TOO_FAST = 'too_fast'
    NO_MESSAGE = 'no_msg'
    OK = 'ok'
    
def parameter_checking(r, func, args, kwargs):
    if kwargs.has_key('limit'): kwargs = int(kwargs['limit'])
    if kwargs.has_key('offset'): kwargs = int(kwargs['offset'])
    
    def retrieval_or_save_cookies(name):
        if name not in func.func_code.co_varnames: return
        if not kwargs.has_key(name):
            value = r.COOKIES.get(name, None)
            if value is not None: kwargs[name] = value
        elif kwargs.has_key(name):
            r.NEW_COOKIES.append((name, value, None))
    
    retrieval_or_save_cookies('track')
    retrieval_or_save_cookies('lang')
        
class ContentManage(object):
    
    def tag_list(self, category, offset=0, limit=100, mode='list', track='', ipaddr=''):
        #offset, limit = int(offset), int(limit)
        if not category: 
            logging.info("tag list: category is None")
            return (0, [])
        logging.info("tag list: category=%s,lang=%s, mode=%s, track=%s, ippaddr=%s" % (category.code,
                     category.lang, mode, track, ipaddr))
        
        cache_key = "tag_list_%s_%s_%s_%s" % (category.id, mode, offset, limit)
        tag_list = self.__cache(cache_key)
        if tag_list is not None: return tag_list
        
        order_by = {'list':'-order',
                    'hot':'-count',
                    'new':'-update_date',
                    'active':'-order',}.get(mode, "-order")
        
        count = ContentTag.all().ancestor(category).count()
        tag_list = ContentTag.all().ancestor(category).order(order_by).fetch(limit, offset)
        
        time = mode == 'new' and 5 * 60 or 30 * 60
        self.__cache(cache_key, (count, tag_list), time)
        return (count, tag_list)

    def cate_list(self, lang='', track='', ipaddr=''):
        logging.info("cate list: lang=%s, track=%s, ippaddr=%s" % (
                     lang, track, ipaddr))
        if not lang: return (0, [])
        
        cache_key = "cate_list_%s" % lang
        cate_list = self.__cache(cache_key)
        if cate_list is not None: return cate_list
        
        count = ContentCategory.all().filter("lang =", lang).count()
        cate_list = ContentCategory.all().filter("lang =", lang).fetch(100)
        self.__cache(cache_key, (count, cate_list))
        return (count, cate_list)
    
    def tag_message(self, category, tag='', offset=0, limit=50, mode='list', track='', ipaddr=''):
        #offset, limit = int(offset), int(limit)
        if not category: return None        
        logging.info("tag: category=%s,lang=%s, tag=%s, mode=%s, track=%s, ippaddr=%s" % (category.code,
                     category.lang, tag, mode, track, ipaddr))
                
        cache_key = "tag_message_%s_%s_%s_%s" % (category.id, mode, offset, limit, tag)
        message_list = self.__cache(cache_key)
        if message_list is not None: return message_list

        query = ContentTag.load_by_name(category, tag).message_query().filter("status >", 0)
        
        count = query.count()
        message_list = self.__query_message(query, offset, limit, mode)
        self.__cache(cache_key, (count, message_list))
        
        return (count, message_list)
        
    def category_message(self, category, offset=0, limit=50, mode='list', track='', ipaddr=''):
        #offset, limit = int(offset), int(limit)
        if not category: 
            logging.info("category_message: category is None")
            return (0, [])
        logging.info("cate: category=%s, lang=%s, mode=%s, track=%s, ippaddr=%s" % (category.code,
                     category.lang, mode, track, ipaddr))
                
        cache_key = "cate_message_%s_%s_%s_%s" % (category.id, mode, offset, limit)
        message_list = self.__cache(cache_key)
        if message_list is not None: return message_list
        
        query = ContentMessage.all().ancestor(category).filter("status >", 0)
        count = query.count()
        message_list = self.__query_message(query, offset, limit, mode)
        self.__cache(cache_key, (count, message_list))
        
        return (count, message_list)
    
    def __query_message(self, query, offset, limit, mode):
        order_by = {'hot':'-vote_weight',
                    'new':'-create_date',
                    'active':'-update_date',}.get(mode, "")
        
        if order_by: query = query.order(order_by) 
        return query.fetch(limit, offset)
    
    def random(self, list):
        index = random.randrange(0, len(list))
        return list[index]
    
    def vote(self, category, msg_id='', v='', msg='', user=None, track='', ipaddr=''):
        if not category: return None
        logging.info("vote: category=%s, lang=%s, msg_id=%s, v=%s, track=%s, ippaddr=%s" % (category.code,
                     category.lang, msg_id, v, mode, track, ipaddr))
                
        if not str(v).isdigit(): return (ErrorCode.ERROR_VALUE, "vote value '%s'" % v)
        
        if user is None: user = self.load_user("guest")
        
        cache_key = "vote_%s_%s_%s_%s" % (category.id, msg_id, ipaddr, user.name)
        count = memcache.incr(cache_key, namespace='content', initial_value=0)
        if (user.name != 'guest' and count > 1) or count > 10:
            return (ErrorCode.TOO_FAST, "")
        
        message = ContentMessage.load(cate, msg_id)
        if message is None: return (ErrorCode.NO_MESSAGE, "msg_id=%s" % msg_id)
        
        ContentVote(parent=message, voter=user, summary=msg, track=track, ipaddr=ipaddr, ).put()
        
        return (ErrorCode.OK, "")
    
    def post(self, category, message='', tags='', user=None, track='', ipaddr=''):
        if not category: return None
        if user is None: user = self.load_user("guest")
        logging.info("post: category=%s, lang=%s, track=%s, ippaddr=%s" % (category.code,
                     category.lang, track, ipaddr))        
        
        md5msg = md5(message)
        cache_key = "new_message_%s_%s_%s" % (category.id, ipaddr, user.name)
        msg = self.__cache(cache_key)
        if msg == md5msg:
            return (ErrorCode.TOO_FAST, "")
        
        message = ContentMessage(parent=category,
                                 user=user,
                                 text=message,
                                 track=track,
                                 ipaddr=ipaddr)
        message.add_tags(tags)
        message.put()
        
        self.__cache(cache_key, md5msg)
        
        return (ErrorCode.OK, "")

    def update_tag(self, category, msg_id='', tags=''):
        if not category: return None
        logging.info("update tag: category=%s, lang=%s, msg_id=%s, tags=%s, track=%s, ippaddr=%s" % (category.code,
                     category.lang, msg_id, tags, track, ipaddr))               
        message = ContentMessage.load(cate, msg_id)
        if message is None: return (ErrorCode.NO_MESSAGE, "msg_id=%s" % msg_id)
        
        message.add_tags(tags)
        return (ErrorCode.OK, "")
    
    def load_category(self, code='', lang='', name='', desc=''):
        #ContentCategory.all().filter("") .order("-lastupdate").fetch(12)
        logging.info("load_category: code=%s, lang=%s, name=%s" % (code, lang, name))
        if not code: return None
        cache_key = "category_%s_%s" % (code, lang)
        category = self.__cache(cache_key)
        if category is not None: return category
        
        category = ContentCategory.all().filter("code =", code).filter("lang =", lang).get()
        if category is None and name:
            category = ContentCategory(code=code, name=name, lang=lang, desc=desc)
            category.put()
            
        self.__cache(cache_key, category)
        return category
    
    def load_tag(self, cate, tag):
        logging.info("load_tag: code=%s, lang=%s, name=%s" % (cate.code, cate.lang, tag))
        return ContentTag.load_by_name(cate, tag)
        
    
    def load_user(self, name='', session='', track=''):
        if not name: return None
        cache_key = "user_%s" % (name)
        user = self.__cache(cache_key)
        if user is not None: return user
        
        user = ContentUser.all().filter("name =", name).get()
        if user is None:
            user = ContentUser(name=name, track=track)
            user.put()
        self.__cache(cache_key, user)
        return user
        
    def __cache(self, key, value=None, time=30 * 60):
        if value is not None:
            memcache.add(key=key, value=value, time=time, namespace='content')
        
        return memcache.get(key, namespace='content')
