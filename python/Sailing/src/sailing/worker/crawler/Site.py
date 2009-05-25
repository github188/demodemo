from __future__ import with_statement
import os
from contextlib import closing
from urlparse import urlparse
from tempfile import NamedTemporaryFile as tempfile

from htmlreader import HtmlReader
from sailing.common.utils import PathUtils as utils
from sailing.worker.crawler import Crawler

from sailing.common.common import *

class DownLoadSite(Crawler):
    
    def __init__(self, **param):
        
        self.process_ext = ('.js', '.html', '.css')
    
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
    
    def _replace_links(self, site, path, mapping):        
        def _replace(l):
            for k, v in mapping.iteritems():
                l = l.replace(k, utils.relative_url(path, v))
            return l
        
        real_path = site.real_path(path)
        
        temp_file = real_path + ".tmp"        
        fd = open(temp_file, "w+")
        
        with closing(open(real_path, 'r')) as links:
            for l in links:
                fd.write(_replace(l))
                
        fd.flush()
        fd.close()
        #copy_to(temp_file, real_path)
        #remove_path(temp_file)
        
    def _add_download_task(self, url, mapping, task):
        
        for link, path in mapping.iteritems():
            link = utils.absolute_url(url, link)
            task.add_action("%s==>%s" % (link, path))
    
    def _process_html(self, path, url, task, site):
        
        url_mapping = {}
        handler = HTMLHandlers(url, path, site.hostname)
        HtmlReader(handler).parseFile(site.real_path(path))
        
        self._replace_links(site, path, handler.url_mapping)
        self._add_download_task(url, handler.url_mapping, task)
        
    def _process_css(self, path):
        pass

    def _process_js(self, path):
        pass


class HTMLHandlers(object):
    
    def __init__(self, cur_url='', local_path='', web_host=''):
        self.cur_url = cur_url
        self.local_path = local_path
        self.web_host = web_host
        self.url_mapping = {}
    
    def a_start(self, href=None, **p):
        self._process_link(href)
    
    def img_start(self, src=None, **p):
        self._process_link(src)
    
    def scripts_start(self, src=None, **p):
        self._process_link(src)
    
    def _process_link(self, l):
        if l is None: return
        if self.url_mapping.has_key(l): return
        if l[:5].lower() == "http:":
            url = urlparse(l)
            host_name = url.hostname
            if url.port: host_name += ":%s" % url.port      
            if host_name.lower() == self.web_host.lower():
                local_path = utils.absolute_path(self.local_path, url.path)
                self.url_mapping[l] = local_path
        else:
            local_path = utils.absolute_path(self.local_path, l)
            self.url_mapping[l] = local_path

