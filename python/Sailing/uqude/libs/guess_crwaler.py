from __future__ import with_statement
import os
from contextlib import closing
from urlparse import urlparse
from tempfile import NamedTemporaryFile as tempfile

from sailing.worker.crawler.linkcrawler import LinkCrawler
from sailing.common.utils import PathUtils as utils
from sailing.worker.crawler import Crawler

from sailing.common.common import *
import logging

class GuessTopicCrwaler(object):
    logger = logging.getLogger("guess")
    
    
    def crawl(self, status, path, orginal_url, next_task, site):
        if status != '200': return
        p, ext = os.path.splitext(path)
        
        if ext == ".html":
            self._process_html(path, orginal_url, next_task, site)
    
    def _process_html(self, path, url, task, site):
        
        links = set()
        def _process_link(l):
            links.add(l)
            return None
        
        crawler = LinkCrawler()
        crawler.crawling(site.real_path(path), url, _process_link)
        
        for link in links:
            if not re.match("/guess/\d+/", link):
            	self.logger.info("ignore link:%s" % link) 
            	continue
            else:
            	self.logger.info("fetch:%s" % link)
            	
            path = "%s/topic.data" % link.strip("/")
            
            self.logger.info("add spider:%s==>%s" % (link, path))
            task.add_action("%s==>%s@fetch_guess_topic" % (link, path))
    

   
        