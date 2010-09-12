
import urllib
        
class JSONProxy(object):
    def __init__(self, serviceURL, serviceName=None):
        self.__serviceURL = serviceURL
        self.__serviceName = serviceName

    def __getattr__(self, name):
        if self.__serviceName != None:
            name = "%s/%s" % (self.__serviceName, name)
        return JSONProxy(self.__serviceURL, name)

    def __call__(self, **kw):
        from simplejson import loads
        postdata = urllib.urlencode(kw)
        respdata = urllib.urlopen("%s/%s" % (self.__serviceURL,
                                           self.__serviceName
                                           ), 
                                  postdata).read()
        return loads(respdata)