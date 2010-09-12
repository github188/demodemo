# -*- coding: utf-8 -*-
import urllib
from serializer import Serializer, ClassLoader
from copy import copy

class RPCException(Exception): pass

class RPCProxy(object):
    
    def __init__(self, uri, method=None, namespaces=None, sid=''):
        self.__serviceURL = uri
        self.__serviceName = method
        self.namespaces = isinstance(namespaces, ClassLoader) and namespaces or \
                              ClassLoader(namespaces or [])
        self._seril = Serializer(self.namespaces)
        self.sid = sid
        self.logger = None
        self.start_call_listener = []
        self.end_call_listener = []
    
    def __call__(self, *args, **kw):
        args = self._seril.serialize((args, kw))
        
        post_data = {"method": self.__serviceName, 
                     'params': args, 
                     'id':'httprpc',
                     'sid':self.sid}
        
        #@todo xx
        for l in self.start_call_listener:
            l(name=self.__serviceName, args=args, kw_args=kw)
        
        rpc_response = self.post_request(self.__serviceURL, post_data)
        try:
            respdata = rpc_response.read()
            ret, e = self._seril.deserialize(respdata)
        except Exception, e:
            raise RPCException("Failed to deserialize response data:%s, exception:%s" % 
                               (respdata, e))
        finally:
            rpc_response.close()
        
        #@todo xx
        for l in self.end_call_listener:
            l(name=self.__serviceName, ret=ret, exception=e)
        
        if e: raise e
        
        return ret
    
    def post_request(self, url, data={}):
        postdata = urllib.urlencode(data)
        
        self.logger and self.logger.info("post_request:%s, postdata:%s" % (url, postdata))
        #respdata = urllib.urlopen(self.__serviceURL, postdata) #.read()
        fd = urllib.urlopen(self.__serviceURL, postdata)
        if fd.headers.type != "application/octet-stream":
            raise RPCException("Not supported content-type '%s', response from %s, application/octet-stream is required." % (fd.headers.type, self.__serviceURL))
        return fd
        
    def __getattr__(self, name):
        if name.startswith("__"):
            raise AttributeError, "'RPCProxy' object has no attribute '%s'" % name
        
        if hasattr(self, '__serviceName') and self.__serviceName:
            name = "%s.%s" % (self.__serviceName, name)
        obj = copy(self)
        obj.__serviceName = name
        obj._seril = self._seril
        obj.namespaces = self.namespaces
        return obj
    
    def __getstate__(self,):
        odict = self.__dict__.copy()
        if odict.has_key('_seril'):
            del odict['_seril']
        if odict.has_key('namespaces'):
            del odict['namespaces']
            
        return odict
    
    def __setstate__(self, st):
        self.__dict__.update(st)
    
    def copy(self):
        return RPCProxy(self.__serviceURL, self.__serviceName, self.namespaces, self.sid)

class RPCStub(object):
    def __init__(self, uri, stub, namespace=None):
        self.uri = uri
        self.stub = stub
        self.namespace = namespace
        self._seril = Serializer(namespace)
        self.logger = None
        
    def process(self, method, data):
        ret = exception = None
        try:
            args, kw = self._seril.deserialize(data)
            try:
                self.logger and self.logger.info(u"method:%s, args:%s, kw:%s" % (method,
                                                args, kw))
            except Exception, e:
                #@todo:  fix the decode problem.
                self.logger and self.logger.info(str(e))
                
            
            h = self._local_service(self.stub, method)
            if h is None: raise RPCException(u"Not found interface '%s'" % method)
            
            ret = h(*args, **kw)
            
            self.logger and self.logger.info("return:%s" % (ret, ))
        except BaseException, e:
            exception = e
            self.logger and self.logger.exception(e)
        
        return self._seril.serialize((ret, exception))
    
    def __call__(self, method, data):
        return self.process(method, data)
        
    def _local_service(self, obj, method):
        try:
            for e in method.split('.'):
                obj = getattr(obj, e)
        except:
            return None
            
        return obj
