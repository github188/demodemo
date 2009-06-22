
from pytel.interpreter.interpreter import Interpreter
from pytel.ast import parse_file

class TestPyTel_interpreter(unittest.TestCase):
    
    def setUp(self):
        pass
    
    def _complie_source(self, path):
        ast = parse_file(path)
        interp = Interpreter()
        bytedata = interp.complie(ast)
        
        return bytedata
    
    def test_simple_helloword(self):
        
        data = self._complie_source("helloword.t")
        
        data.show()
        