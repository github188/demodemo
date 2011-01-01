
import urllib
import logging
import json

def JSON_Proxy(object):
    def __init__(self, url):
        self.rpc_root = url.strip("/")
        
    def __call__(self, **kw):
        rpc_response = self.post_request(self.rpc_root, kw)
        ret = {}
        
        try:
            respdata = rpc_response.read()
            ret = json.loads(respdata)
        except Exception, e:
            logging.exception(e)
        finally:
            rpc_response.close()
            
        return ret
    
    def post_request(self, url, data={}):
        postdata = urllib.urlencode(data)
        
        logging.debug("post_request:%s, postdata:%s" % (url, postdata))
        #respdata = urllib.urlopen(self.__serviceURL, postdata) #.read()
        fd = urllib.urlopen(url, postdata)
        if fd.headers.type != "application/octet-stream":
            logging.error("Response error conent type:%s" % fd.headers.type)
        return fd    
    
    def __getattr__(self, name):
        if name.startswith("rpc_"):
            return JSON_Proxy("%s/%s" % (self.rpc_root, name))

