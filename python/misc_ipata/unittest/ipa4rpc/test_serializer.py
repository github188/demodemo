import unittest

from ipa4rpc.serializer import Serializer

class TestRPC_Serializer(unittest.TestCase):
    
    def test_simple_serializer(self):
        ser = Serializer()
        obj = {'t1':'test', }
        data = ser.serialize(obj)
        self.assertEqual(data[0], '0')
        
        obj1 = ser.deserialize(data)
        
        self.assertTrue(isinstance(obj1, dict))
        self.assertEqual(obj1.keys(), ['t1', ])
        self.assertEqual(obj1['t1'], 'test')
        self.assertNotEqual(id(obj1), id(obj))
        
        obj2 = {'t12':'test2', }
        data = ser.serialize(obj2)
        obj3 = ser.deserialize(data)
        
        self.assertEqual(obj2, obj3)


    def test_compress_serializer(self):
        ser = Serializer()
        obj = {'t1':'test' * 1024 , }
        data = ser.serialize(obj)
        
        self.assertEqual(data[0], '1')
        self.assertEqual(len(data), 74)
        
        obj1 = ser.deserialize(data)
        self.assertEqual(obj, obj1)
        
    def test_serializer_with_namespaces(self):
        ns = lambda x:1
        ns.MockClass = MockClass2
        
        ser = Serializer([ns, ])
        obj = MockClass('test')
        obj.test = 'testvalue1'
        data = ser.serialize(obj)
        
        obj1 = ser.deserialize(data)
        
        self.assertEqual(type(obj1), MockClass2)
        self.assertEqual(obj1.test, 'testvalue1')
        self.assertEqual(obj1.value, 'test')
        
        
class MockClass(object):
    def __init__(self, value=''):
        self.value = value
    
class MockClass2(object):pass
        