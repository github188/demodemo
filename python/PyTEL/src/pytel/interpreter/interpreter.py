
class Operation(object):
    
    #Stack: ... value1 -> ...
    pop = 'POP'
    
    #Stack: ... value1 -> ...[REG]
    pop_reg = 'POP_REG'

    push = 'PUSH'
    
    #Store stack top into return register and then pop it
    pop_result = 'POP_RESULT'
    
    goto = 'GOTO'
    if_goto = 'IF_GOTO'
    unless_goto = 'UNLESS_GOTO'
        
    ifeq_pop = 'IFEQ_POP'
    
    #call function
    #OP, NAME, ARGS
    call = 'CALL' 
    
    #return 
    ret_func = 'RETURN_FUNC'
    
    #return void func
    ret_void_func = 'RETURN_VOID_FUNC'
    
    #data const variable to data stack
    #load_const_var = 'LOAD_CONST_VAR'
    
    #set named var to data
    #OP, NAME, ==> [0]
    load_var = 'LOAD_VAR'

    #set named var to data
    load_ref = 'LOAD_REF'
    #OP, NAME, START, LEN ==> [0]
    load_array = 'LOAD_ARRAY'
    
    #save variable
    #OP, NAME, <== [0]
    save_var = 'SAVE_VAR'
    save_ref = 'SAVE_REF'
    #OP, NAME, START, LEN<== [0]
    save_array = 'SAVE_ARRAY'
    
    #decl variable: OP, qualifier, TYPE, NAME.
    decl_ref = 'DECL_REF'
    decl_var = 'DECL_VAR'
    
    #decl variable: OP, qualifier, TYPE, NAME, LEN
    decl_array = 'DECL_ARRAY'
    
    #del_var = 'DEL_VAR'
    
    null_var = 'NULL_VAR'
    
class InternalVariable(object):
    def __init__(self, name, type, const=False, array=False, length=0):
        self.name = name
        self.type = type
        self.length = length
        self.const = const
        self.value = None

class InternalReference(object):
    def __init__(self, name, frame):
        self.frame = frame
        self.name = name

class Function(object):
    def __init__(self, name, ret_type, args, bytedata=None, function=None):
        self.name = name
        self.ret_type = ret_type
        self.args = args
        self.bytedata = bytedata
        self.function = fucntion
        self.builtIn = bytedata == None
        
        self.inter = None
        

class UserFunction(Function):
        
    def call(self, *args):
        return self.function(*args)

class BuiltInFunction(Function):
    
    def call(self, *args):
        frame = self.inter.new_callframe(self.bytedata)
        
        args.reverse()
        for arg in args:        
            frame.push(arg)
        self.inter.interpreter(self.bytedata, frame)
        
        return self.inter.return_value


OP = Operation
from bytedata import ByteData
from callframe import CallFrame
import types
class Interpreter(object):
    
    def __init__(self):
        self.bytedata = None
        self.ast = None
        #statement 
        self.statement_stack = []
        self.frame = None
        
        self.global_frame = None
        self.return_value = None

    def complie(self, ast):
        self.ast = ast
        self.bytedata = ByteData()
        
        return self.bytedata
    
    def interpreter(self, bytedata, frame=None):
        if frame is None:
            self.frame = self.new_callframe(bytedata)
        
        frame.bytedata = bytedata
        
        while self.frame is not None:
           op = self.frame.next_operation()
           handler_name = "_op_%s" % op
           if  hasattr(self, handler_name):
               handler = getattr(self, handler_name)
               handler(self.frame, )
           else:
               self.error_msg("Not supported operation '%s'" % op)
    
    def init_global_frame(self):
        self.global_frame = self.new_callframe(None, None)
    
    def init_builtin_function(self, builtin_list):
        
        for func in builtin_list:
            if callable(func):
                name = func.func_code.co_name
                args_length = func.func_code.co_argcount
                decl = Function(name, 'unknown', args_length, None, func)
                self.global_frame.func_reference(name, decl)
                
            elif isinstance(func, types.TupleType):
                decl = Function(*func)
                self.global_frame.func_reference(decl.name, decl)
    
    def new_callframe(self, frame, bytedata):
        cur_frame = CallFrame(bytedata=bytedata, parent=frame)
        return cur_frame
    
    def function_reference(self, name, ref=None):
        func = self.global_frame.func_reference(name, ref)
        if func:
            func.inter = self
            return func
        else:
            self.error_msg("function '%s' not define!" % name)

    def var_reference(self, name, ref=None):
        
        var = self.frame.var_reference(name, ref) or self.global_frame.var_reference(name)
        if var: 
            return var
        else:
            self.error_msg("variable '%s' not define!" % var)

    
    def add_operation(self, *args):
        self.bytedata.code.extend(args)
        
    def update_coord(self, coord):
        self.bytedata.update_coord(coord)

    def new_label(self,):
        return self.bytedata.new_label()

    def add_label(self, label=None):
        return self.bytedata.add_label(label)
        
    def error_msg(self, msg, coord=None):
        print "msg: %s, coord:%s" % (msg, coord)
    
    def _generate_coding(self, ast):
        handler = "_c_%s" % type(ast).__name__
        if hasattr(self, handler):
            getattr(self, handler)(ast)
        else:
            error_msg("not found AST handler:%s" % ast)
            
    def _eval_param(self, frame, val):
        """[C0] --- register
           [-1] --- stack tail
           [1] --- stack pop
        """        
        if type(val) == str:
            if val[0] == '[':
                reg = val[1:-2]
                try:
                    reg = int(reg)
                    if reg > 0:
                        for e in range(reg):
                            reg = frame.pop()
                    else:
                        reg = frame.stack[reg]
                    return reg
                except:
                    return frame.register(reg) 
        else:
            return val
    
    def _c_FunctionDecl(self, funDecl):
        
        inter = Interpreter()
        inter.ast = funDecl
        function = inter.bytedata = ByteData()
        
        #compile function topology.
        function.return_type = funDecl.type
        function.name = funDecl.name
        function.param = funDecl.param
        
        inter._c_ParamList(funDecl.param)
        inter._generate_coding(funDecl.stmt)
        
        self.bytedate.functions.append(inter.bytedata)
        
        func = Function(funDecl.name, funDecl.type, len(funDecl.param), function)
        self.function_reference(funDecl.name, func)
        

    def _c_ParamList(self, paramList):
        for param in paramList.children():
            self._c_Param(param)
        
    def _c_Param(self, param):
        
        init_op = None
        
        if param.out:
            op = OP.decl_ref
            init_op = OP.save_ref
        elif param.var.type == 'array':
            op = OP.decl_array
            init_op = OP.save_array
        else:
            op = OP.decl_var
            init_op = OP.save_var
            
        self.add_operation(op, None, param.type, param.var.name)
        self.add_operation(init_op, param.var.name)
        
        
    def _c_Compound(self, comp):
        
        for decl in comp.decls:
            self._c_declare(decl)
        
        for stmt in comp.stmts:
            self._c_statement(stmt)

    def _c_BinaryOp(self, node):
        op = node.op
        #self._c_expression(node.left)
        #self._c_expression(node.right)
        if op.type == 'OR':
            true_label = self.new_label()
            self._generate_coding(node.left)
            self.add_operation(OP.if_goto, true_label)
            self.add_operation(OP.pop)
            self._generate_coding(node.right)
            self.add_label(true_label)
        elif op.type == 'AND':
            false_label = self.new_label()
            self._generate_coding(node.left)
            self.add_operation(OP.unless_goto, false_label)
            self.add_operation(OP.pop)
            self._generate_coding(node.right)
            self.add_label(false_label)
        else:
            self._c_expression(node.left)
            self._c_expression(node.right)
            self.bytedata.code("op_" % op.type)
            
    def _c_FuncCall(self, comp):
        len_args = len(self.args)
        for expr in self.args:
            self._generate_coding(expr)
        
        self.add_operation(OP.call, comp.name, len_args)
            
        
    def _c_Decl(self, decl):
        qualifier = decl.qualifier
        type = decl.type
        
        self.update_coord(decl.coord)
        
        def init_array(name, length, init):
            
            self.add_operation(OP.decl_array, qualifier, type, name, length)
            
            if init:
                for value in init:
                    self._generate_coding(value)
                self.add_operation(OP.save_array, name, 0, len(init))
        
        def init_var(name, init):
            #length = len(init)
            self.add_operation(OP.decl_var, qualifier, type, name)
            if init: 
                self._generate_coding(init)
            else:
                self.add_operation(OP.null_var)
                
            self.add_operation(OP.save_var, name)
            
        for var in vars:
            op = OP.decl_array if var.name.type == 'array' else OP.decl_var
            if var.name.type == 'array':
                length = eval(var.index) or len(var.init or [])
                init_array(var.name.name, length, var.init)
            else:
                init_var(var.name.name, var.init)
        
        
    def _c_Constant(self, const):
        #ord
        if const.type == 'char':
            self.add_operation(OP.push, ord(const.value))
        elif const.type == 'integer':
            if const.value.startswith("0x"):
                self.add_operation(OP.push, int(const.value, 16))
            elif const.value.startswith("0"):
                self.add_operation(OP.push, int(const.value, 8))
            else:
                self.add_operation(OP.push, int(const.value))
        elif const.type == 'string':
            self.add_operation(OP.push, const.value)
        
        else:
            self.error_msg("invalid const", coord)

    def _c_Varible(self, var):
        if var.type == 'array':
            self._generate_coding(var.index)
            self.add_operation(OP.pop_reg, "[C0]")
            self.add_operation(OP.load_array, var.name, '[C0]', 1)
        else:
            self.add_operation(OP.load_var, var.name)
    
    def _c_If(self, stmt):
        
        true_label = self.new_label()
        false_label = self.new_label()
        
        end_label = self.new_label()
        
        self._generate_coding(stmt.cond)
        self.add_operation(OP.unless_goto, false_label)
        self._generate_coding(stmt.iftrue)
        self.add_operation(OP.goto, end_label)
        self.add_label(false_label)
        
        self._generate_coding(stmt.iffalse)
        self.add_label(end_label)
        self.add_operation(OP.pop) #pop the result of expression
        
    def _c_For(self, stmt):
        
        self.statement_stack.append(stmt)
        
        self._generate_coding(stmt.init)

        stmt.cond_label = self.new_label()
        stmt.next_label = self.new_label()
        stmt.end_label = self.new_label()
        stmt.pop_label = self.new_label()
        
        #start condition expression
        self.add_label(stmt.cond_label)
        self._generate_coding(stmt.cond)
        self.add_operation(OP.unless_goto, stmt.pop_label)
        
        #start statement
        self.add_operation(OP.pop)
        self._generate_coding(stmt.stmt)
        
        #start next statement
        self.add_label(stmt.next_label)
        self._generate_coding(stmt.next)
        self.add_operation(OP.goto, stmt.cond_label)
        
        #pop condition result.
        self.add_label(stmt.pop_label)
        self.add_operation(OP.pop)
        
        #
        self.add_label(stmt.end_label)
        
        self.statement_stack.pop()
    
    def _c_Break(self, st):
        
        if len(self.statement_stack) > 0:
            stmt = self.statement_stack[-1]
            self.add_operation(OP.goto, stmt.end_label)
        else:
            pass
        
    def _c_Continue(self, st):
        
        if len(self.statement_stack) > 0:
            stmt = self.statement_stack[-1]
            self.add_operation(OP.goto, stmt.next_label)
        else:
            pass
        
    def _c_Return(self, stmt):
        
        if stmt.expr:
            self._generate_coding(stmt.expr)
            self.add_operation(OP.ret_func)
        else:
            self.add_operation(OP.ret_void_func)
    
    def _c_While(self, stmt):
        
        self.statement_stack.append(stmt)

        stmt.cond_label = self.new_label()
        stmt.end_label = self.new_label()
        stmt.pop_label = self.new_label()
        
        #start condition expression
        self.add_label(stmt.cond_label)
        self._generate_coding(stmt.cond)
        self.add_operation(OP.unless_goto, stmt.pop_label)
        
        #start statement
        self.add_operation(OP.pop)
        self._generate_coding(stmt.stmt)
        self.add_operation(OP.goto, stmt.cond_label)
        
        #pop condition result.
        self.add_label(stmt.pop_label)
        self.add_operation(OP.pop)
        
        self.add_label(stmt.end_label)
        
        self.statement_stack.pop()
    
    def _op_POP(self, frame):
        frame.pop()
    
    def _op_PUSH(self, frame):
        data = frame.next_data()
        frame.push(data)
    
    def _op_DECL_VAR(self, frame):
        #decl variable: OP, qualifier, TYPE, NAME.
        const, type, name = frame.next_data(), frame.next_data(), frame.next_data()
        
        #self, name, type, array=False, length=0, const=False
        var =  InternalVariable(name, type, const == 'const')
        
        self.var_reference(name, var)
        
    def _op_IF_GOTO(self, frame):
        label = frame.next_data()
        if frame.top(): frame.goto(label)
    
    def _op_UNLESS_GOTO(self, frame):
        label = frame.next_data()
        if not frame.top(): frame.goto(label)
        
    def _op_LOAD_VAR(self, frame):
        name = frame.next_data()
        var = self.var_reference(name)
        
        frame.push(var.value)

    def _op_LOAD_ARRAY(self, frame):
        name = frame.next_data()
        index = frame.next_data()
        length = frame.next_data()
        
        index = self._eval_param(frame, index)
        length = self._eval_param(frame, length)
        
        var = self.var_reference(name)
        
        if var is not None:
            value = var.value[index: index + length]
            for v in value:
                frame.push(v)
            if length != 1:
                frame.push(length)
                
    def _op_SAVE_VAR(self, frame):
        name = frame.next_data()
        value = frame.top()
        
        var = self.var_reference(name)
        
        if var.const and var.value is not None:
            self.error_msg("variable '%s' is const." % name)
        else:
            if var.type == 'string' and isinstance(value, basestring):
                var.value = value
            elif var.type == 'integer' and isinstance(value, int):
                var.value = value
            elif var.type == 'float' and isinstance(value, float):
                var.value = value
            else:
                self.error_msg("error variable type '%s' != '%s'", (var.type, type(value)))
                
    def _op_SAVE_ARRAY(self, frame):
        name = frame.next_data()
        index = frame.next_data()
        length = frame.next_data()
        
        index = self._eval_param(frame, index)
        length = self._eval_param(frame, length)
        
        var = self.var_reference(name)
        
        value = []
        for v in range(length):
            value.insert(0, frame.pop())
        
        var.value[index: index + length] = value
                
    def _op_CALL(self, frame):
        
        self.return_value = None
        name = frame.next_data()
        
        args_length = frame.next_data()
        args = []
        for e in range(args_length):
            args.append(frame.pop())
        
        func = self.function_reference(name)
        if func == None:
            self.error_msg("function %s not defined before!" % name)
        
        if func.builtIn:
            args.reverse()
            result = func.call(*args)
            if result is not None:
                frame.push(result)
        else:
            frame = self.new_callframe(frame, func.bytedata)
            for arg in args: frame.push(arg)
            self.frame = frame
    
    def _op_RETURN_FUNC(self, frame):
        self.return_value = value = frame.pop
        parent_frame = frame.parentFrame
        if parent_frame is not None:
            parent_frame.push(value)
        self.frame = parent_frame

    def _op_RETURN_VOID_FUNC(self, frame):
        self.frame = frame.parentFrame
    
    def _op_PLUS(self, frame):
        var1 = frame.pop()
        var2 = frame.pop()
        frame.push(var2 + var1)

    def _op_MINUS(self, frame):
        var1 = frame.pop()
        var2 = frame.pop()
        frame.push(var2 - var1)

    def _op_TIMES(self, frame):
        var1 = frame.pop()
        var2 = frame.pop()
        frame.push(var2 * var1)

    def _op_DIVIDE(self, frame):
        var1 = frame.pop()
        var2 = frame.pop()
        frame.push(var2 / var1)

    def _op_MOD(self, frame):
        var1 = frame.pop()
        var2 = frame.pop()
        frame.push(var2 % var1)
    
    def _op_LT(self, frame):
        var1 = frame.pop()
        var2 = frame.pop()
        frame.push(1 if var2 < var1 else 0)

    def _op_LE(self, frame):
        var1 = frame.pop()
        var2 = frame.pop()
        frame.push(1 if var2 <= var1 else 0)
        
    def _op_GT(self, frame):
        var1 = frame.pop()
        var2 = frame.pop()
        frame.push(1 if var2 > var1 else 0)

    def _op_GE(self, frame):
        var1 = frame.pop()
        var2 = frame.pop()
        frame.push(1 if var2 >= var1 else 0)

    def _op_EQ(self, frame):
        var1 = frame.pop()
        var2 = frame.pop()
        frame.push(1 if var2 == var1 else 0)

    def _op_NE(self, frame):
        var1 = frame.pop()
        var2 = frame.pop()
        frame.push(1 if var2 != var1 else 0)

    def _op_BOR(self, frame):
        var1 = frame.pop()
        var2 = frame.pop()
        frame.push(var2 | var1)

    def _op_BAND(self, frame):
        var1 = frame.pop()
        var2 = frame.pop()
        frame.push(var2 & var1)

    def _op_BNOT(self, frame):
        var1 = frame.pop()
        frame.push(~var1)

    def _op_BXOR(self, frame):
        var1 = frame.pop()
        var2 = frame.pop()
        frame.push(var2 ^ var1)

    
