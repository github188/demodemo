
import logging

class LoginUqude(object):
    def __init__(self, ):
        self.logger = logging.getLogger("uqude")
        
    def __call__(self, site, http, next_task, url, local_url, *args):
        self.logger.info("start login url............")
        
        
class FetchGuessTopic(object):
    def __init__(self, ):
        pass
    
    def __call__(self, site, http, next_task, url, local_url, *args):
        
        self.logger.info("start fetch topic, url:%s" % url)
        data = http.get(url)
        
        