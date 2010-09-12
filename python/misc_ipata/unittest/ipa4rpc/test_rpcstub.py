import unittest

from ipa4rpc.serializer import Serializer
from ipa4rpc import RPCStub

class TestRPC_Serializer(unittest.TestCase):
    
    def test_rpc_stub(self):
        service = RPCStub('', RemoteClass(), )
        ser = Serializer()
        args = 'args'
        
        method = 'rpc_func'
        params = ser.serialize(((args, ), {}))
        
        data = service(method, params)
        
        (ret, e) = ser.deserialize(data)
        
        self.assertEqual(ret, "xxx:%s" % args)
        self.assertEqual(e, None)
        
        
class RemoteClass(object):
    
    def rpc_func(self, name):
        return "xxx:%s" % name