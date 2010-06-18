
import logging

class SessionMiddleware(object):
    def process_request(self, request):
        sessionid = request.COOKIES.get('sessionid', '')
        if not sessionid:
            request.COOKIES['sessionid'] = self._new_session()
        #request.sessionId = sessionid or self._new_session()
        request.NEW_COOKIES = []
        request.session = MemCacheSession(request.COOKIES['sessionid'])

    def _new_session(self):
        import hashlib
        import uuid #.uuid4()
        
        return hashlib.md5(str(uuid.uuid4())).hexdigest()


    def process_response(self, request, response):
        SESSION_COOKIE_AGE = 60 * 60 * 24 * 7 * 2 
        sessionid = request.COOKIES.get('sessionid', '')
        
        #logging.info("set cookies:%s" % sessionid)
        response.set_cookie('sessionid',
                sessionid, max_age = SESSION_COOKIE_AGE,
                expires=None, domain=None,
                path='/',
                secure=None)
        for k, v, age in request.NEW_COOKIES:
            age = age or SESSION_COOKIE_AGE
            response.set_cookie(k, v, 
                    max_age = age,
                    expires=None, domain=None,
                    path='/',
                    secure=None)
        
        return response

from google.appengine.api import memcache
class MemCacheSession(dict):
    def __init__(self, session_id):
        self.session_id = session_id
        self.cached_session = {}
    
    def add(self, key, value, timeout=3600):
        key = self.__cachedkey__(key)
        self.cached_session[key] = value
        memcache.set(key=key, value=value, time=timeout, namespace='session')
        return value
    
    def get(self, key):
        key = self.__cachedkey__(key)
        if self.cached_session.has_key(key):
            return self.cached_session[key]
        value = memcache.get(key, namespace='session')
        self.cached_session[key] = value
        return value
    
    def remove(self, key):
        key = self.__cachedkey__(key)
        if self.cached_session.has_key(key): 
            del self.cached_session[key]
        return memcache.delete(key=key, namespace='session')
    
    def __cachedkey__(self, key):
        return "%s_%s" % (self.session_id, key)
    
    def __getitem__(self, key):
        return self.get(key)
    
    def __setitem__(self, key, value):
        return self.add(key, value)
    
    def __delitem__(self, key):
        return self.remove(key)
        