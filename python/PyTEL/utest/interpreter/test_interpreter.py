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
    
    def _complie_with_label(self, code):
        xx = "void function main() %s endfunction" % code
        
        ast = CParser().parse(xx, "#0")
        interp = Interpreter()
        bytedata = interp.assemble_ast(ast)
        
        func = bytedata.func_reference("main")
        
        return (func.bytedata.code, func.bytedata.labels) 
    
    def test_c_decl(self):
        byte = self._complie_express_code("integer i=1;")
        self.assertEqual(byte, ['DECL_VAR', None, 'integer', 'i', 
                                'PUSH', 1, 'SAVE_VAR', 'i'])
        
        byte = self._complie_express_code("const integer i=1;")
        self.assertEqual(byte, ['DECL_VAR', 'const', 'integer', 'i', 
                                'PUSH', 1, 'SAVE_VAR', 'i'])
        
        byte = self._complie_express_code("integer i=1,b,c=1;")
        self.assertEqual(byte, ['DECL_VAR', None, 'integer', 'i', 
                                'PUSH', 1, 'SAVE_VAR', 'i', 
                                'DECL_VAR', None, 'integer', 'b', 
                                'NULL_VAR', 'SAVE_VAR', 'b', 
                                'DECL_VAR', None, 'integer', 'c', 
                                'PUSH', 1, 'SAVE_VAR', 'c'])
        
        byte = self._complie_express_code("integer i[5];")
        self.assertEqual(byte, ['DECL_ARRAY', None, 'integer', 'i', 5])

        byte = self._complie_express_code("integer i[]={1,2,3};")
        self.assertEqual(byte, ['DECL_ARRAY', None, 'integer', 'i', 3, 
                                'PUSH', 1, 'PUSH', 2, 'PUSH', 3, 
                                'SAVE_ARRAY', 'i', 0, 3])

    def test_c_expression(self):
        byte = self._complie_express_code("1+2;")
        self.assertEqual(byte, ['PUSH', 1, 'PUSH', 2, 'PLUS'])

        byte = self._complie_express_code("1+2*3;")
        self.assertEqual(byte, ['PUSH', 1, 'PUSH', 2, 'PUSH', 3, 
                                'TIMES', 'PLUS'])
        
        byte = self._complie_express_code("(1+2)*3;")
        self.assertEqual(byte, ['PUSH', 1, 'PUSH', 2, 'PLUS', 
                                'PUSH', 3, 'TIMES'])
        
        byte = self._complie_express_code("(1+2)*3/4;")
        self.assertEqual(byte, ['PUSH', 1, 'PUSH', 2, 'PLUS', 
                                'PUSH', 3, 'TIMES', 
                                'PUSH', 4, 'DIVIDE'])

        
    def test_c_assignment(self):
        byte = self._complie_express_code("a=1+2;")
        self.assertEqual(byte, ['PUSH', 1, 'PUSH', 2, 'PLUS', 
                                'SAVE_VAR', 'a'])

        byte = self._complie_express_code("a=dofunc();")
        self.assertEqual(byte, ['CALL', 'dofunc', 0, 'SAVE_VAR', 'a'])

        byte = self._complie_express_code("a[i+1]=dofunc();")
        self.assertEqual(byte, ['CALL', 'dofunc', 0, 
                                'LOAD_VAR', 'i', 'PUSH', 1, 
                                'PLUS', 
                                'POP_REG', '[C0]', 
                                'SAVE_ARRAY', 'a', '[C0]', 1])

    def test_c_if_statement(self):
        byte, labels = self._complie_with_label("if(1>2) hello(); endif")
        self.assertEqual(byte, ['PUSH', 1, 'PUSH', 2, 'GT', 
                                'UNLESS_GOTO', 'label_2', 
                                'CALL', 'hello', 0, 
                                'GOTO', 'label_3', 
                                'POP'])
        self.assertEqual(labels, {'label_2': 12, 'label_3': 12})

    def test_c_if_else_statement(self):
        byte, labels = self._complie_with_label("if(1) do_true();else do_false(); endif")
        self.assertEqual(byte,  ['PUSH', 1, 
                                 'UNLESS_GOTO', 'label_2', 
                                 'CALL', 'do_true', 0, 
                                 'GOTO', 'label_3', 
                                 'CALL', 'do_false', 0, 
                                 'POP'])
        self.assertEqual(labels, {'label_2': 9, 'label_3': 12})
        
    def test_c_for_statement(self):
        byte, labels = self._complie_with_label("for(i=1; i<2; i=i+1) do_true();endfor")
        self.assertEqual(byte,  ['PUSH', 1, 'SAVE_VAR', 'i', 
                                 'LOAD_VAR', 'i', 'PUSH', 2, 'LT', 'UNLESS_GOTO', 'label_4', 
                                 'POP', 
                                 'CALL', 'do_true', 0, 
                                 'LOAD_VAR', 'i', 'PUSH', 1, 'PLUS', 'SAVE_VAR', 'i', 
                                 'GOTO', 'label_1', 
                                 'POP'])
        self.assertEqual(labels, {'label_4': 24, 'label_1': 4, 'label_2': 15, 'label_3': 25})
        
    def test_c_for_break(self):
        byte, labels = self._complie_with_label("for(;;) break; endfor")
        self.assertEqual(byte,  ['PUSH', 1, 'UNLESS_GOTO', 'label_4', 
                                 'POP', 
                                 'GOTO', 'label_3', 
                                 'GOTO', 'label_1', 
                                 'POP'])
        self.assertEqual(labels, {'label_4': 9, 'label_1': 0, 'label_2': 7, 'label_3': 10})        

    def test_c_for_continue(self):
        byte, labels = self._complie_with_label("for(;;) continue; endfor")
        self.assertEqual(byte,  ['PUSH', 1, 'UNLESS_GOTO', 'label_4', 
                                 'POP', 
                                 'GOTO', 'label_2', 
                                 'GOTO', 'label_1', 
                                 'POP'])
        self.assertEqual(labels, {'label_4': 9, 'label_1': 0, 'label_2': 7, 'label_3': 10})        

    def test_c_while_end(self):
        byte, labels = self._complie_with_label("while(1>2) do_some(); endwhile")
        self.assertEqual(byte,  ['PUSH', 1, 'PUSH', 2, 'GT', 
                                 'UNLESS_GOTO', 'label_3', 
                                 'POP', 'CALL', 'do_some', 0, 
                                 'GOTO', 'label_1', 
                                 'POP'])
        self.assertEqual(labels, {'label_1': 0, 'label_2': 14, 'label_3': 13})        
    
    def atest_c_switch_case(self):
        byte, labels = self._complie_with_label("""switch(1) 
                                                case 2:do_2();
                                                case 3:do_3();
                                                default:do_default();
                                                endswitch""")
        self.assertEqual(byte,  ['PUSH', 1, 'PUSH', 2, 'GT', 
                                 'UNLESS_GOTO', 'label_3', 
                                 'POP', 'CALL', 'do_some', 0, 
                                 'GOTO', 'label_1', 
                                 'POP'])
        self.assertEqual(labels, {'label_1': 0, 'label_2': 14, 'label_3': 13})        

    def atest_c_do_while(self):
        byte, labels = self._complie_with_label("do do_some(); while (1>2)")
        self.assertEqual(byte,  ['PUSH', 1, 'UNLESS_GOTO', 'label_4', 
                                 'POP', 
                                 'GOTO', 'label_2', 
                                 'GOTO', 'label_1', 
                                 'POP'])
        self.assertEqual(labels, {'label_4': 9, 'label_1': 0, 'label_2': 7, 'label_3': 10})        

    
    def test_c_function_call(self):
        byte = self._complie_express_code("sayHello(abc, 123);")
        self.assertEqual(byte, ['LOAD_VAR', 'abc', 'PUSH', 123, 'CALL', 'sayHello', 2])
        
        byte = self._complie_express_code("sayHello(b, a[0+2]);")
        self.assertEqual(byte, ['LOAD_VAR', 'b', 
                                'PUSH', 0, 'PUSH', 2, 'PLUS', 
                                'POP_REG', '[C0]', 
                                'LOAD_ARRAY', 'a', '[C0]', 1, 
                                'CALL', 'sayHello', 2])
                
    def test_c_function_decl(self):
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

        byte = cc("""integer function sayHello(string w[], integer t[]);
                        return 1;
                     endfunction
                  """)
        self.assertEqual(byte, ['DECL_ARRAY', None, 'string', 'w', 
                                'SAVE_REF', 'w', 
                                'DECL_ARRAY', None, 'integer', 't', 
                                'SAVE_REF', 't', 
                                'PUSH', 1, 'RETURN_FUNC'])
        
        