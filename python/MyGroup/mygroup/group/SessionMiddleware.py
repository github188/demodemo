
import logging

class SessionMiddleware(object):
    def process_request(self, request):
        sessionid = request.COOKIES.get('sessionid', '')
        if not sessionid:
            request.COOKIES['sessionid'] = self._new_session()
        #request.sessionId = sessionid or self._new_session()

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
        return response
