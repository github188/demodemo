
import logging
__all__ = ['LocationNode', 'BTSNode', 'CenterServer', 'RouteServer']

class LocationNode(object):
    def __init__(self, code=None, name=None):
        self.uuid = code
        self.name = name
        self.children = []
        self.btsList = []
    
    def add_children(self, node):
        self.children.append(node)
        
    def bind(self, data):
        self.uuid = data['uuid']
        self.name = data['name']
        self.children = []
        for sub in data['children']:
            self.children.append(LocationNode())
            self.children[-1].bind(sub)
        for sub in data['listBTS']:
            self.children.append(BTSNode())
            self.children[-1].bind(sub)
    
    @property
    def view_label(self):
        return "%s-%s" % (self.uuid, self.name)
    
    @property
    def is_leaf(self): return False
    
class AlarmRecord(object):
    def bind(self, data):
        self.__dict__.update(data)        

class BTSNode(object):
    def __init__(self, code='', name=''):
        self.uuid = code
        self.name = name
        self.children = []
        
    def bind(self, data):
        self.__dict__.update(data)        

    @property
    def view_label(self):
        return "%s-%s" % (self.uuid, self.name)
    
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
        if code == '0':
            x, sid = ret.split("$", 1)
            self.proxy.sid = sid
        
        return code
    
    def load_bts_list(self):
        ret = self.proxy.rpc_list_bts()
        node = None
        if ret['status'] == '0':
            node = LocationNode()
            node.bind(ret['data'])
        else:
            logging.info("load_bts_list: %s" % ret['status'])
        return node
    
    def load_alarm_list(self, ):
        ret = self.proxy.rpc_list_alarm()
        alarm_list = []
        if ret['status'] == '0':
            for r in ret['data']['data']:
                alarm_list.append(AlarmRecord())
                alarm_list[-1].bind(r)
        else:
            logging.info("load_alarm_list: %s" % ret['status'])
            
        return alarm_list
        
class RouteServer(object):
    def __init__(self, ):
        pass
    
