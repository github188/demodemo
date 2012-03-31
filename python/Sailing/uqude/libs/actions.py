
import logging

class LoginUqude(object):
    def __init__(self, ):
        self.logger = logging.getLogger("uqude")
        
    def __call__(site, http, url, local_url, *args):
        self.logger.info("start login url............")
        