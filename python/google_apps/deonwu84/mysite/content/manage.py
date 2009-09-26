# -*- coding: utf-8 -*-
from google.appengine.api import memcache
from models import *
from google.appengine.ext import db
import logging
from datetime import datetime
"""

list mode:
1. list
2. hot
3. new
4. active <-- voted?
5. random

"""
import hashlib
def md5(str):
    if isinstance(str, unicode): str = str.encode("utf-8")
    return hashlib.md5(str).hexdigest()

class ErrorCode:
    ERROR_VALUE = 'err_value'
    TOO_FAST = 'too_fast'
    NO_MESSAGE = 'no_msg'
    OK = 'ok'
    
def parameter_checking(r, func, args, kwargs):
    if kwargs.has_key('limit'): kwargs['limit'] = int(kwargs['limit'])
    if kwargs.has_key('offset'): kwargs['offset'] = int(kwargs['offset'])
    
    def retrieval_or_save_cookies(name):
        if name not in func.func_code.co_varnames: return
        if not kwargs.has_key(name):
            value = r.COOKIES.get(name, 'zh')
            kwargs[name] = value
        elif kwargs.has_key(name):
            r.NEW_COOKIES.append((name, kwargs[name], None))
    
    retrieval_or_save_cookies('track')
    retrieval_or_save_cookies('lang')
    

def cached(key_length=1, timeout=5 * 60 * 60):
    def check_cache(f):
        reqiured_args_count = f.func_code.co_argcount - len(f.func_defaults)
        def _wrapper(*args, **kwargs):
            keys = [f.__name__, ]
            for i in range(1, key_length+1):
                if i >= len(args):
                    v = f.func_defaults[i - reqiured_args_count]
                else:
                    v = args[i]
                if isinstance(v, basestring):
                    keys.append(unicode(v))
                elif hasattr(v, 'id'):
                    keys.append(str(v.id))
                else:
                    keys.append(str(v))
    
            cache_key = u"_".join(keys)
            if len(cache_key) > 32: cache_key = md5(cache_key)
            
            logging.info("hit cache key:%s" % cache_key)
            value = memcache.get(cache_key, namespace='content')
            if value is not None: return value
            
            logging.info("failed to hit cache key:%s" % cache_key)
            value = f(*args, **kwargs)
            memcache.add(key=cache_key, value=value, time=timeout, namespace='content')
            ContentManage.cached_keys.add(cache_key)
            return value
        return _wrapper
        
    return check_cache
        
class ContentManage(object):
    
    cached_keys = set()
    
    @cached(key_length=4)
    def tag_list(self, category, offset=0, limit=100, mode='list', track='', ipaddr=''):
        #offset, limit = int(offset), int(limit)
        if not category: 
            logging.info("tag list: category is None")
            return (0, [])
        logging.info("tag list: category=%s,lang=%s, mode=%s, track=%s, ippaddr=%s" % (category.code,
                     category.lang, mode, track, ipaddr))
        order_by = {'list':'-order',
                    'hot':'-count',
                    'new':'-update_date',
                    'active':'-order',}.get(mode, "-order")
        
        count = ContentTag.all().ancestor(category).count()
        tag_list = ContentTag.all().ancestor(category).order(order_by).fetch(limit, (offset -1) * limit)
        
        return (count, tag_list)

    @cached(key_length=1, timeout=5 * 60)
    def cate_list(self, lang='', track='', ipaddr=''):
        logging.info("cate list: lang=%s, track=%s, ippaddr=%s" % (
                     lang, track, ipaddr))
        if not lang: return (0, [])
                        
        count = ContentCategory.all().filter("lang =", lang).count()
        cate_list = ContentCategory.all().filter("lang =", lang).fetch(100)

        return (count, cate_list)
    
    @cached(key_length=5, timeout=5 * 60)
    def tag_message(self, category, tag='', offset=0, limit=50, mode='list', track='', ipaddr=''):
        #offset, limit = int(offset), int(limit)
        if not category: return None        
        logging.info("tag: category=%s,lang=%s, tag=%s, mode=%s, track=%s, ippaddr=%s" % (category.code,
                     category.lang, tag, mode, track, ipaddr))
        
        order_by = {'hot':'vote_weight desc',
                    'new':'create_date desc',
                    'active':'update_date desc',}.get(mode, "")
        
        if order_by: 
            order_by = "status desc, %s" % order_by
        else:
            order_by = "status desc"
        
        query = ContentTag.load_by_name(category, tag).message_query("status >= 0", order_by)
        count = query.count()
        message_list = query.fetch(limit, (offset -1) * limit)
        # self.__query_message(query, offset, limit, mode)
        
        return (count, message_list)
    
    @cached(key_length=4)
    def category_message(self, category, offset=0, limit=50, mode='list', track='', ipaddr=''):
        #offset, limit = int(offset), int(limit)
        if not category: 
            logging.info("category_message: category is None")
            return (0, [])
        logging.info("cate: category=%s, lang=%s, mode=%s, track=%s, ippaddr=%s" % (category.code,
                     category.lang, mode, track, ipaddr))
                        
        query = ContentMessage.all().ancestor(category).filter("status >=", 0).order("status")
        count = query.count()
        message_list = self.__query_message(query, offset, limit, mode)
        
        return (count, message_list)
    
    def __query_message(self, query, offset, limit, mode):
        order_by = {'hot':'-vote_weight',
                    'new':'-create_date',
                    'active':'-update_date',}.get(mode, "")
        
        if order_by: query = query.order(order_by) 
        return query.fetch(limit, (offset -1) * limit)
    
    def random(self, list):
        index = random.randrange(0, len(list))
        return list[index]
    
    def vote(self, category, msg_id='', v='', msg='', user=None, track='', ipaddr=''):
        if not category: return None
        logging.info("vote: category=%s, lang=%s, msg_id=%s, v=%s, track=%s, ippaddr=%s" % (category.code,
                     category.lang, msg_id, v, track, ipaddr))
        
        try:
            v = int(v)
        except: 
            return (ErrorCode.ERROR_VALUE, "vote value '%s'" % v)
        
        if not isinstance(user, ContentUser): user = self.load_user(unicode(user) or "guest")
        #if user is None: user = self.load_user("guest")
        
        cache_key = "vote_%s_%s_%s_%s" % (category.id, msg_id, ipaddr, user.name)
        count = memcache.incr(cache_key, namespace='content', )
        if count is None:
            memcache.set(cache_key, value=1, namespace='content', )
            count = 1
        logging.info("%s-->%s" % (cache_key, count))
        if (user.name != 'guest' and count > 1) or count > 10:
            return (ErrorCode.TOO_FAST, "")
        
        message = ContentMessage.load(category, msg_id)
        if message is None: return (ErrorCode.NO_MESSAGE, "msg_id=%s" % msg_id)
        
        message.vote_weight += v
        if v > 0:
            message.vote_up += v
        else:
            message.vote_down += v
        message.update_date = datetime.now()
        status = message.vote_weight / 10
        if status > message.status: message.status = status
        
        message.put()
        ContentVote(parent=message, value=v, voter=user, summary=msg, track=track, ipaddr=ipaddr,).put()
        
        return (ErrorCode.OK, "")
    
    def post(self, category, message='', tags='', user=None, track='', ipaddr=''):
        if not category: 
            logging.info("post: category is None")
            return (0, [])
        if not isinstance(user, ContentUser): user = self.load_user(unicode(user) or "guest")
        message, tags = unicode(message), unicode(tags)
        if tags.count(u"空格") > 0: tags = ''
        logging.info("post: category=%s, lang=%s, tags=%s, user=%s, track=%s, ippaddr=%s" % (category.code,
                     category.lang, tags, user.name, track, ipaddr))
        
        md5msg = md5(message)
        cache_key = "new_message_%s_%s_%s" % (category.id, ipaddr, user.name)
        msg = self.__cache(cache_key)
        if msg == md5msg:
            return (ErrorCode.TOO_FAST, "")
        
        message = ContentMessage(parent=category,
                                 user=user,
                                 text=db.Text(message),
                                 track=track,
                                 ipaddr=ipaddr)
        message.add_tags(tags)
        message.put()
        
        self.clear_cache()
        #clean up all the cache....
        #memcache.flush_all()
        
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
    
    @cached(key_length=2)
    def load_category(self, code='', lang='', name='', desc=''):
        #ContentCategory.all().filter("") .order("-lastupdate").fetch(12)
        logging.info("load_category: code=%s, lang=%s, name=%s" % (code, lang, name))
        if not code: return None
        
        category = ContentCategory.all().filter("code =", code).filter("lang =", lang).get()
        if category is None and name:
            category = ContentCategory(code=code, name=name, lang=lang, desc=desc)
            category.put()
            
        return category
    
    def load_tag(self, cate, tag):
        logging.info("load_tag: code=%s, lang=%s, name=%s" % (cate.code, cate.lang, tag))
        return ContentTag.load_by_name(cate, tag)
        
    @cached(key_length=1)    
    def load_user(self, name='', session='', track=''):
        if not name: return None
                
        user = ContentUser.all().filter("name =", name).get()
        if user is None:
            user = ContentUser(name=name, track=track)
            user.put()
        return user
        
    def __cache(self, key, value=None, time=30 * 60):
        if value is not None:
            memcache.add(key=key, value=value, time=time, namespace='content')
        
        return memcache.get(key, namespace='content')
    
    def clear_cache(self, ):
        for k in self.cached_keys:
            #logging.info("remove cache:%s" % k)
            memcache.delete(k, namespace='content')
        ContentManage.cached_keys = set()
