import unittest
from pytel.interpreter.interpreter import Interpreter
from pytel.ast import parse_file
import os
from pytel.ast.c_parser import CParser

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
        #bytedata.show()
        
        main = interp.function_reference("main", None)
        main.call(["xxx", "bb"])
        
    
class TestPyTel_Compiler(unittest.TestCase):
    
    def setUp(self):
        pass
    
    def _complie_express_code(self, code):
        xx = "void function main() %s endfunction" % code
        
        ast = CParser().parse(xx, "#0")
        interp = Interpreter()
        bytedata = interp.assemble_ast(ast)
        
        func = bytedata.func_reference("main")
        
        return func.bytedata.code
    
    def test_c_function_call(self):
        byte = self._complie_express_code("sayHello(abc, 123);")
        self.assertEqual(byte, ['LOAD_VAR', 'abc', 'PUSH', 123, 'CALL', 'sayHello', 2])
        
        byte = self._complie_express_code("sayHello(b, a[0+2]);")
        self.assertEqual(byte, ['LOAD_VAR', 'b', 
                                'PUSH', 0, 'PUSH', 2, 'PLUS', 
                                'POP_REG', '[C0]', 
                                'LOAD_ARRAY', 'a', '[C0]', 1, 
                                'CALL', 'sayHello', 2])
                
    def test_c_function_define(self):
        def cc(code):
            ast = CParser().parse(code, "#0")
            interp = Interpreter()
            bytedata = interp.assemble_ast(ast)
            func = bytedata.functions.values()[0]            
            return func.bytedata.code
        
        byte = cc("""void function sayHello(string world, integer times);
                     endfunction
                  """)
        self.assertEqual(byte, ['DECL_VAR', None, 'string', 'world', 
                                'SAVE_VAR', 'world', 
                                'DECL_VAR', None, 'integer', 'times', 
                                'SAVE_VAR', 'times'])
        
        byte = cc("""void function sayHello(string w[], integer t[]);
                     endfunction
                  """)
        self.assertEqual(byte, ['DECL_ARRAY', None, 'string', 'w', 
                                'SAVE_REF', 'w', 
                                'DECL_ARRAY', None, 'integer', 't', 
                                'SAVE_REF', 't'])
