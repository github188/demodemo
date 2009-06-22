
from interpreter import Interpreter

from pytel.ast import parse_file
from pytel.builtin import BUILTIN_LIST

class PyTEL(object):
    
    def __init__(self):

        self.inter = Interpreter()
        self.inter.init_global_frame()
        self.inter.init_builtin_function(BUILTIN_LIST)
        
    
    def load(self, source):
        ast = parse_file(source)
        bytedata = self.inter.complie(ast)
        
        frame = self.inter.global_frame
        frame.reset()
        self.inter.interpreter(byttedata, frame)
    
    def run(self, *args):
        main = self.inter.function_reference('main')
        main(args, len(args))
    
    def run_function(self, name, *args):
        function = self.inter.function_reference(name)
        return function.call(args)
    
    def find_variable(self, name):
        pass
    