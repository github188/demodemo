
import logging
__all__ = ['LocationNode', 'BTSNode', 'CenterServer', 'RouteServer']

class LocationNode(object):
    def __init__(self, code, name):
        self.code = code
        self.name = name
        self.children = []
    
    def add_children(self, node):
        self.children.append(node)
    
    @property
    def view_label(self):
        return self.name
    
    @property
    def is_leaf(self): return False

class BTSNode(object):
    def __init__(self, code, name):
        self.code = code
        self.name = name
        self.children = []

    @property
    def view_label(self):
        return self.name
    
    @property
    def is_leaf(self): return True
    

class CenterServer(object):
    def __init__(self, proxy):
        logging.info("xxxxx:%s" % proxy)
        self.proxy = proxy
    
    def login(self, username, password):
        ret = self.proxy.login(user=username, password=password)
        ret = ret.strip()
        logging.info("login username='%s', password='%s', ret='%s'" % 
                     (username, '*' * len(password), ret))
        if ":" in ret:
            code, msg = ret.split(":", 1)
        else:
            code = "-2"
        return code
        
class RouteServer(object):
    def __init__(self, ):
        pass
    
