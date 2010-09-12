
import sys
import cPickle, cStringIO, zlib, base64

class ClassLoader(object):
    def __init__(self, pkg_list):
        self.pkg_list = pkg_list
        self.logger = None
    
    def __call__(self, pkg, clazz):
        self.logger and self.logger.debug("load class:%s.%s" % (pkg, clazz))
        cls = None
        for e in self.pkg_list:
            if hasattr(e, clazz):
                cls = getattr(e, clazz)
                break
        cls = cls or self.load(pkg, clazz)
        self.logger and self.logger.debug("cls:%s" % cls)
        return cls
    
    def load(self, module, name):
        __import__(module)
        mod = sys.modules[module]
        klass = getattr(mod, name)
        return klass
    
class Serializer(object):
    
    def __init__(self, namespaces=None):
        self.cache = cStringIO.StringIO()
        self._pickle = cPickle.Pickler(self.cache, -1)
        self._unpickle = cPickle.Unpickler(self.cache)
        
        self._cl = None
        if namespaces and isinstance(namespaces, ClassLoader):
            self._cl = namespaces
        elif namespaces:
            self._cl = ClassLoader(namespaces)
         
        if self._cl:
            self._unpickle.find_global = self._cl

    def serialize(self, obj):
        self.clear_cache()
        self._pickle.dump(obj)
        
        compress = '0'
        data = self.cache.getvalue()
        if len(data) > 1024:
            compress = '1'
            data = zlib.compress(data)
        
        return compress + base64.encodestring(data)
    
    def deserialize(self, data):
        self.clear_cache()
        
        compress = data[0]
        data = base64.decodestring(data[1:])
        if compress == '1':
            data = zlib.decompress(data)
            
        self.cache.write(data)
        self.cache.reset()
        return self._unpickle.load()
    
    def clear_cache(self):
        self.cache.reset()
        self.cache.truncate()
