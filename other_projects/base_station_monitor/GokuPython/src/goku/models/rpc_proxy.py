
import urllib2, urllib
import logging
import json

class JSON_Proxy(object):
    def __init__(self, url, action='index', sid=None):
        self.rpc_root = url.strip("/")
        self.action = action
        self.sid = sid
        
    def __call__(self, **kw):
        rpc_response = self.post_request(self.rpc_root, kw)
        ret = {}
        
        try:
            respdata = rpc_response.read()
            if rpc_response.headers.type == 'application/octet-stream':
                ret = json.loads(respdata)
            else:
                ret = respdata
        except Exception, e:
            logging.exception(e)
        finally:
            rpc_response.close()
            
        return ret
    
    def post_request(self, url, data={}):
        data['q'] = self.action
        if self.sid:data['sid'] = self.sid
        postdata = urllib.urlencode(data)
        
        logging.debug("post_request:%s, postdata:%s" % (url, postdata))
        #respdata = urllib.urlopen(self.__serviceURL, postdata) #.read()
        fd = urllib.urlopen(url, postdata, proxies={})
        if fd.headers.type not in ["application/octet-stream", 'text/plain']:
            logging.error("Response error conent type:%s" % fd.headers.type)
        return fd
    
    def __getattr__(self, name):
        if name.startswith("rpc_") or name in ['login', 'logout']:
            return JSON_Proxy(self.rpc_root, name, self.sid)
        else:
            raise AttributeError("Not found action:%s" % name)

