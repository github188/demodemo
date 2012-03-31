
import logging
	
class GuessTopicCrwaler(object):
    logger = logging.getLogger("guess")
    
    def __init__(self, **kw):
        pass
    
    def crawl(self, status, path, orginal_url, next_task, site):
        self.logger.info("try go load new topic...")
        next_task.save_data("0000", 'this is test')
        