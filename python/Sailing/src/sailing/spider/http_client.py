
import urllib2, httplib
import StringIO
import gzip
from urlparse import urlparse
from sailing.common.common import *

class HTTPClient(object):
    
    def __init__(self, www_root=None):
        self.proxy = None
        self.last_url = None
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
        
        if not exists_path(dir_name(save_as)): make_path(dir_name(save_as))
        fd = open(save_as, "w+")
        
        url = self.relative_path(url)
        
        try:
            #httplib.HTTPConnection.debuglevel = 1
            request = urllib2.Request(url)
            request.add_header('Accept-encoding', 'gzip') 
            opener = urllib2.build_opener(*self._http_handlers())
            
            f = opener.open(request)
            #print f
            if 'gzip' in f.headers.get('Content-Encoding'):
                compresseddata = f.read()                              
                compressedstream = StringIO.StringIO(compresseddata)   
                gzipper = gzip.GzipFile(fileobj=compressedstream)      
                data = gzipper.read()    

                gzipper.close()
        except:
            data = f.read()
            f.close()
            
        fd.write(data)
        fd.close()
        
        return '200'
    
    