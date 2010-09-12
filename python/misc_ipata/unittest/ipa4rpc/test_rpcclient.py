import unittest

from ipa4rpc.serializer import Serializer
from ipa4rpc import RPCStub, RPCProxy
from ipa4rpc.test import SimpleRPCServer

class TestRPC_Client(unittest.TestCase):
    
    def setUp(self):
        self.server = server = SimpleRPCServer(RemoteClass)
        server.start()
        self.server_port = server.server_port
    
    def test_rpc_client_call(self):
        
        client = RPCProxy("http://127.0.0.1:%s" % self.server_port)
        
        self.server._rpc.logger = self
        
        method = client.rpc_func#('hello')
        method.logger = self
        result = method("hello")
        
        self.assertEqual(result, 'xxx:hello')
        
    def info(self, x):
        print x
        
    def exception(self, e):
        print e
        
    def tearDown(self,):
        self.server.close()
        
class TestRPC_copy_method(unittest.TestCase):
        
    def test_new_copy_proxy(self):
        client = RPCProxy("http://127.0.0.1:", sid='aaa')
        self.assertEqual(client.logger, None)
        self.assertEqual(client.sid, 'aaa')
        
        logger = lambda:0
        
        client.logger = logger
        
        new_client = client.new_method
        self.assertEqual(new_client.logger, logger)
        self.assertEqual(new_client.sid, 'aaa')
        
        
                
class RemoteClass(object):
    def rpc_func(self, name):
        return "xxx:%s" % name