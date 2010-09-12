
import logging

class SimpleCookieMiddleware(object):
    def process_request(self, request):
        request.NEW_COOKIES = {}

    def process_response(self, request, response):
        SESSION_COOKIE_AGE = 60 * 60 * 24 * 7 * 2 

        for k, v in request.NEW_COOKIES.items():
            value, age, path = v
            age = age or SESSION_COOKIE_AGE
            response.set_cookie(k, value, 
                    max_age = age,
                    expires=None, domain=None,
                    path=path,
                    secure=None)
        
        return response
        