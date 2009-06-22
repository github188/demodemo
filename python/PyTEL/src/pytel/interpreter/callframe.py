
class CallFrame(object):
    
    def __init__(self, bytedata, parent=None):
        self.parentFrame = parent
        
        # amount of stack frames before this one on the interpretation stack
        self.frameIndex = 0
        
        #current bytedata sequence
        self.bytedata = bytedata
        
        #frame status, 
        # Stack structure
        # stack[0 <= i < localShift]: arguments and local variables
        # stack[localShift <= i <= emptyStackTop]: used for local temporaries
        # stack[emptyStackTop < i < stack.length]: stack data
        # sDbl[i]: if stack[i] is UniqueTag.DOUBLE_MARK, sDbl[i] holds the number value        
        self.stack = []
        
        self.vars = {}
        
        self.functions = {}
        
        self.registers = {'CO':None, 'C1':None, 'C2':None, 'C3':None, }
        
        self.operation_index = 0
        
    def pop(self):
        return self.stack.pop()
    
    def push(self, data):
        self.stack.append(data)
        return data
    
    def top(self):
        return self.stack[-1]
    
    def goto(self, index):
        self.operation_index = index
        return self.operation_index
    
    def next_operation(self, index = -1):
        return self.next_data(index)
    
    def next_data(self, index = -1):
        if index != -1: self.operation_index = index
        
        o = self.bytedata.code[self.operation_index]
        self.operation_index += 1
        
        return o
        
    def var_reference(self, name, ref=None):
        if ref is not None:
            self.vars[name] = ref
                   
        if self.vars.has_key(name):
            return self.vars[name]
        else:
            return None

    def func_reference(self, name, func=None):
        if ref is not None:
            self.functions[name] = ref
                
        if self.functions.has_key(name):
            return self.functions[name]
        else:
            return None
    
    
    def register(self, name, value=None):
        if not self.registers.has_key(name):
            raise Exception("Not defined register '%s'." % name)
        
        if value:
            self.registers[name] = value
        
        return self.registers[name]
    
    def reset(self):
        self.operation_index = 0
        self.stack = []
        
    