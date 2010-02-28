import unittest
from pytel.interpreter.interpreter import Interpreter
from pytel.ast import parse_file
import os

class TestPyTel_interpreter(unittest.TestCase):
    
    def setUp(self):
        pass
    
    def _complie_source(self, path):
        ast = parse_file(path)
        self.interp = Interpreter()
        bytedata = self.interp.assemble_ast(ast)
        
        return bytedata
    
    def test_simple_helloword(self):
        
        bytedata = self._complie_source(os.path.join(os.path.dirname(__file__), "helloword.t"))
        interp = self.interp
        from pytel.builtin import BUILTIN_LIST
        interp.init_builtin_function(BUILTIN_LIST)
        interp.link_global_code(bytedata)
        bytedata.show()
        
        main = interp.function_reference("main", None)
        main.call(["xxx", "bb"])
        
        
        