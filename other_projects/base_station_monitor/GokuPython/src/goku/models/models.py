
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
    def __init__(self, url):
        pass
        
class RouteServer(object):    
    def __init__(self, ):
        pass
    
