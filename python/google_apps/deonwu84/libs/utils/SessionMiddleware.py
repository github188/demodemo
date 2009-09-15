
import logging

class SessionMiddleware(object):
    def process_request(self, request):
        sessionid = request.COOKIES.get('sessionid', '')
        if not sessionid:
            request.COOKIES['sessionid'] = self._new_session()
        #request.sessionId = sessionid or self._new_session()
        request.NEW_COOKIES = []

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
