
import urllib2, httplib
import StringIO
import gzip
from urlparse import urlparse
from sailing.common.common import *
import logging
import socket
import os

# timeout in seconds
timeout = 10
socket.setdefaulttimeout(timeout)

class HTTPClient(object):
    
    def __init__(self, www_root=None):
        self.proxy = None
        self.last_url = None
        self.logger = logging.getLogger("HttpClient")
        #import urllib
    
    def set_proxy(self, proxy, www_root=None):
        if proxy:
            self.proxy = urllib2.ProxyHandler(proxy)
            
    def _http_handlers(self):
        return self.proxy and [self.proxy, ] or []
    
    def relative_path(self, url):
        
        if url.startswith("http:"):
            self.last_url = urlparse(url)
        elif url.startswith('/'):
            pass
        elif url.startswith('?'):
            pass
        else:
            pass
        
        return self.last_url.geturl()
    
    def download(self, url, save_as):
        
        self.logger.info("download:%s-->%s" % (url, save_as))        
        self.post(url, save_as)
        
        return '200'
        
    def post(self, url, save_as, data=None):
        if data and isinstance(data, dict):
            data = urllib.urlencode(data)
        elif data and os.path.isfile(data):
            fd = open(data, 'r')
            data = fd.read()
            fd.close()
            
        if not exists_path(dir_name(save_as)): make_path(dir_name(save_as))
        
        data = self._http_request(url, data)
        
        fd = open(save_as, "w+")
        fd.write(data)
        fd.close()
        
        return '200'
    
    def get(self, url):
        return self._http_request(url, None)
        
    
    def _http_request(self, url, req_data):
    
        #url = self.relative_path(url)
        try:
            #httplib.HTTPConnection.debuglevel = 1
            if req_data:
                request = urllib2.Request(url, req_data)
            else:
                request = urllib2.Request(url)
                
            request.add_header('Accept-encoding', 'gzip') 
            opener = urllib2.build_opener(*self._http_handlers())
            
            f = opener.open(request)
            #print f
            encoding = f.headers.get('Content-Encoding')
            if encoding and 'gzip' in encoding:
                compresseddata = f.read()                              
                compressedstream = StringIO.StringIO(compresseddata)   
                gzipper = gzip.GzipFile(fileobj=compressedstream)      
                data = gzipper.read()
                gzipper.close()
            else:
                data = f.read()
                f.close()
       
        except urllib2.HTTPError, e:
            raise   
        
        return data
        
    
    