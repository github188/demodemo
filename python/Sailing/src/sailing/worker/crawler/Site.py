from __future__ import with_statement
import os
from contextlib import closing
from urlparse import urlparse
from tempfile import NamedTemporaryFile as tempfile

from linkcrawler import LinkCrawler
from sailing.common.utils import PathUtils as utils
from sailing.worker.crawler import Crawler

from sailing.common.common import *
import logging

class DownLoadSite(Crawler):
    
    def __init__(self, **param):
        
        self.process_ext = ('.js', '.html', '.css')
        self.logger = logging.getLogger("DownloadSite")
    
    def crawl(self, status, path, orginal_url, next_task, site):
        if status != '200': return
        p, ext = os.path.splitext(path)
        if ext not in self.process_ext: return
        
        if ext == ".html":
            self._process_html(path, orginal_url, next_task, site)
        elif ext == ".js":
            self._process_js(path, orginal_url, next_task, site)
        elif ext == ".css":
            self._process_css(path, orginal_url, next_task, site)
    
    def _process_html(self, path, url, task, site):
        
        url_mapping = {}
        def _process_link(l):
            if not url_mapping.has_key(l):
                if l.count(":"): #mailto:, javascript:, http:
                    url_mapping[l] = l
                else:
                    url_mapping[l] = utils.absolute_path(path, l)
            
            self.logger.info("url mapping:%s --> %s" % (l, url_mapping[l]))
            return url_mapping[l]
        
        crawler = LinkCrawler()
        crawler.crawling(site.real_path(path), url, _process_link)
        
        for link, path in url_mapping.iteritems():
            if link.count(':') > 0: continue
            link = utils.absolute_url(url, link)
            task.add_action("%s==>%s" % (link, path))
        
    def _process_css(self, path, *arg):
        pass

    def _process_js(self, path, *arg):
        pass
