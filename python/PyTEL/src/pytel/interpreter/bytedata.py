
import sys
"""
it's complied coding byte. 
"""
class ByteData(object):
    
    def __init__(self):
        
        #a sequence used to generate label name
        self.label_sep = 0
        
        #byte
        self.code = []
        
        self.coords = []
        self.labels = {}
        
        self.stringTable = []
        self.integerTable = []
        self.floatTable = []
        
        self.argNames = []
        self.argConst = 0
        
        #nested function declare
        self.functions = {}
        
        #current coding fragment return type
        self.return_type = 'void'

        #current coding fragment name.
        self.name = 'void'
        
    def update_coord(self, coord):
        self.coords.append((len(self.code), coord))
    
    def add_label(self, name=None):
        name = name or self.new_label()
        if self.labels.has_key(name):raise Exception("already exists label '%s'" % name)
        
        self.labels[name] = len(self.code)
        
        return name
        
    def new_label(self, ):
        self.label_sep += 1
        
        return "label_%s" % self.label_sep
    
    def add_operation(self, *args):
        self.code.extend(args)
    
    def func_reference(self, name, func=None):
        if func is not None:
            self.functions[name] = func
                
        if self.functions.has_key(name):
            return self.functions[name]
        else:
            return None

    def show(self, buf=sys.stdout):

        buf.write("name:%s\n" % self.name)
        buf.write("return_type:%s\n" % self.return_type)
        buf.write("argNames:%s\n" % ",".join(self.argNames))
        
        labels = [ (k, v) for k, v in self.labels.iteritems() ]
        labels.sort(lambda x, y: cmp(x[1], y[1]))
        labels.reverse()
        
        for i in range(len(self.code)):
            if len(labels) > 0 and labels[-1][1] == i:
                buf.write("%s:\n" % labels[-1][0])
                labels.pop()
            
            buf.write("(%s, %s)," % (i, self.code[i]))
        
        if self.functions:
            buf.write("\n----FUNC DECL----\n")
            for name, code in self.functions.iteritems():
                buf.write("Function:%s,\n" % name)
                code.bytedata.show(buf)
        
            
            
            
            
            
            
            
            
            
            
            
    
    