
import sys
"""
it's complied coding byte. 
"""
class ByteData(object):
    
    def __init__(self):
        
        #a sequence used to generate label name
        self.labels = 0
        
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
        self.functions = []
        
        #current coding fragment return type
        self.return_type = 'void'

        #current coding fragment name.
        self.name = 'void'
        
    def update_coord(self, coord):
        self.coords.append((len(self.code), corrd))
    
    def add_label(self, name=None):
        name = name or self.new_label()
        if self.labels.has_key(name):raise Exception("already exists label '%s'" % name)
        
        self.labels[name] = len(self.code)
        
        return name
        
    def new_label(self, ):
        self.labels += 1
        
        return "label_%s" % self.labels
    
    def show(self, buf=sys.stdout):

        buf.write("name:%s\n" % self.name)
        buf.write("return_type:%s\n" % self.return_type)
        buf.write("argNames:%s\n" % ",".join(self.argNames))
        
        labels = [ (k, v) for k, v in self.labels.iteritems() ]
        labels.sort(lambda x, y: cmp(x[1], y[1]))
        i = 0
        while i < len(self.code):
            if len(labels) > 0 and labels[-1][1] == i:
                buf.write("%s:\n" % labels[-1][0])
            
            buf.write("%s:\n" % self.code[i])
            
            
            
            
            
            
            
            
            
            
            
            
    
    