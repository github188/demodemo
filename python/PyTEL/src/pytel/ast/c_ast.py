#-----------------------------------------------------------------
# ** ATTENTION **
# This code was automatically generated from the file:
# _c_ast.yaml 
#
# Do not modify it directly. Modify the configuration file and
# run the generator again.
# ** ** *** ** **
#
# pycparser: c_ast.py
#
# AST Node classes.
#
# Copyright (C) 2008, Eli Bendersky
# License: LGPL
#-----------------------------------------------------------------


import sys


class Node(object):
    """ Abstract base class for AST nodes.
    """
    def children(self):
        """ A sequence of all children that are Nodes
        """
        pass

    def show(self, buf=sys.stdout, offset=0, attrnames=False, showcoord=False):
        """ Pretty print the Node and all its attributes and
            children (recursively) to a buffer.
            
            file:   
                Open IO buffer into which the Node is printed.
            
            offset: 
                Initial offset (amount of leading spaces) 
            
            attrnames:
                True if you want to see the attribute names in
                name=value pairs. False to only see the values.
            
            showcoord:
                Do you want the coordinates of each Node to be
                displayed.
        """
        pass


class NodeVisitor(object):
    """ A base NodeVisitor class for visiting c_ast nodes. 
        Subclass it and define your own visit_XXX methods, where
        XXX is the class name you want to visit with these 
        methods.
        
        For example:
        
        class ConstantVisitor(NodeVisitor):
            def __init__(self):
                self.values = []
            
            def visit_Constant(self, node):
                self.values.append(node.value)

        Creates a list of values of all the constant nodes 
        encountered below the given node. To use it:
        
        cv = ConstantVisitor()
        cv.visit(node)
        
        Notes:
        
        *   generic_visit() will be called for AST nodes for which 
            no visit_XXX method was defined. 
        *   The children of nodes for which a visit_XXX was 
            defined will not be visited - if you need this, call
            generic_visit() on the node. 
            You can use:
                NodeVisitor.generic_visit(self, node)
        *   Modeled after Python's own AST visiting facilities
            (the ast module of Python 3.0)
    """
    def visit(self, node):
        """ Visit a node. 
        """
        method = 'visit_' + node.__class__.__name__
        visitor = getattr(self, method, self.generic_visit)
        return visitor(node)
        
    def generic_visit(self, node):
        """ Called if no explicit visitor function exists for a 
            node. Implements preorder visiting of the node.
        """
        for c in node.children():
            self.visit(c)


class Case(Node):
    def __init__(self, expr, stmt, coord=None):
        self.expr = expr
        self.stmt = stmt
        self.coord = coord

    def children(self):
        nodelist = []
        if self.expr is not None: nodelist.append(self.expr)
        if self.stmt is not None: nodelist.append(self.stmt)
        return tuple(nodelist)

    def show(self, buf=sys.stdout, offset=0, attrnames=False, showcoord=False):
        lead = ' ' * offset
        buf.write(lead + 'Case: ')

        if showcoord:
            buf.write(' (at %s)' % self.coord)
        buf.write('\n')

        for c in self.children():
            c.show(buf, offset + 2, attrnames, showcoord)


class For(Node):
    def __init__(self, init, cond, next, stmt, coord=None):
        self.init = init
        self.cond = cond
        self.next = next
        self.stmt = stmt
        self.coord = coord

    def children(self):
        nodelist = []
        if self.init is not None: nodelist.append(self.init)
        if self.cond is not None: nodelist.append(self.cond)
        if self.next is not None: nodelist.append(self.next)
        if self.stmt is not None: nodelist.append(self.stmt)
        return tuple(nodelist)

    def show(self, buf=sys.stdout, offset=0, attrnames=False, showcoord=False):
        lead = ' ' * offset
        buf.write(lead + 'For: ')

        if showcoord:
            buf.write(' (at %s)' % self.coord)
        buf.write('\n')

        for c in self.children():
            c.show(buf, offset + 2, attrnames, showcoord)


class UnaryOp(Node):
    def __init__(self, op, expr, coord=None):
        self.op = op
        self.expr = expr
        self.coord = coord

    def children(self):
        nodelist = []
        if self.expr is not None: nodelist.append(self.expr)
        return tuple(nodelist)

    def show(self, buf=sys.stdout, offset=0, attrnames=False, showcoord=False):
        lead = ' ' * offset
        buf.write(lead + 'UnaryOp: ')

        if attrnames:
            attrstr = ', '.join('%s=%s' % nv for nv in [("op", repr(self.op))])
        else:
            attrstr = ', '.join('%s' % v for v in [self.op])
        buf.write(attrstr)

        if showcoord:
            buf.write(' (at %s)' % self.coord)
        buf.write('\n')

        for c in self.children():
            c.show(buf, offset + 2, attrnames, showcoord)


class Default(Node):
    def __init__(self, stmt, coord=None):
        self.stmt = stmt
        self.coord = coord

    def children(self):
        nodelist = []
        if self.stmt is not None: nodelist.append(self.stmt)
        return tuple(nodelist)

    def show(self, buf=sys.stdout, offset=0, attrnames=False, showcoord=False):
        lead = ' ' * offset
        buf.write(lead + 'Default: ')

        if showcoord:
            buf.write(' (at %s)' % self.coord)
        buf.write('\n')

        for c in self.children():
            c.show(buf, offset + 2, attrnames, showcoord)


class Param(Node):
    def __init__(self, out, type, var, coord=None):
        self.out = out
        self.type = type
        self.var = var
        self.coord = coord

    def children(self):
        nodelist = []
        if self.var is not None: nodelist.append(self.var)
        return tuple(nodelist)

    def show(self, buf=sys.stdout, offset=0, attrnames=False, showcoord=False):
        lead = ' ' * offset
        buf.write(lead + 'Param: ')

        if attrnames:
            attrstr = ', '.join('%s=%s' % nv for nv in [("out", repr(self.out)), ("type", repr(self.type))])
        else:
            attrstr = ', '.join('%s' % v for v in [self.out, self.type])
        buf.write(attrstr)

        if showcoord:
            buf.write(' (at %s)' % self.coord)
        buf.write('\n')

        for c in self.children():
            c.show(buf, offset + 2, attrnames, showcoord)


class Label(Node):
    def __init__(self, name, stmt, coord=None):
        self.name = name
        self.stmt = stmt
        self.coord = coord

    def children(self):
        nodelist = []
        if self.stmt is not None: nodelist.append(self.stmt)
        return tuple(nodelist)

    def show(self, buf=sys.stdout, offset=0, attrnames=False, showcoord=False):
        lead = ' ' * offset
        buf.write(lead + 'Label: ')

        if attrnames:
            attrstr = ', '.join('%s=%s' % nv for nv in [("name", repr(self.name))])
        else:
            attrstr = ', '.join('%s' % v for v in [self.name])
        buf.write(attrstr)

        if showcoord:
            buf.write(' (at %s)' % self.coord)
        buf.write('\n')

        for c in self.children():
            c.show(buf, offset + 2, attrnames, showcoord)


class FunctionDecl(Node):
    def __init__(self, type, name, param, stmt, coord=None):
        self.type = type
        self.name = name
        self.param = param
        self.stmt = stmt
        self.coord = coord

    def children(self):
        nodelist = []
        if self.name is not None: nodelist.append(self.name)
        if self.param is not None: nodelist.append(self.param)
        if self.stmt is not None: nodelist.append(self.stmt)
        return tuple(nodelist)

    def show(self, buf=sys.stdout, offset=0, attrnames=False, showcoord=False):
        lead = ' ' * offset
        buf.write(lead + 'FunctionDecl: ')

        if attrnames:
            attrstr = ', '.join('%s=%s' % nv for nv in [("type", repr(self.type))])
        else:
            attrstr = ', '.join('%s' % v for v in [self.type])
        buf.write(attrstr)

        if showcoord:
            buf.write(' (at %s)' % self.coord)
        buf.write('\n')

        for c in self.children():
            c.show(buf, offset + 2, attrnames, showcoord)


class FileAST(Node):
    def __init__(self, ext, coord=None):
        self.ext = ext
        self.coord = coord

    def children(self):
        nodelist = []
        if self.ext is not None: nodelist.extend(self.ext)
        return tuple(nodelist)

    def show(self, buf=sys.stdout, offset=0, attrnames=False, showcoord=False):
        lead = ' ' * offset
        buf.write(lead + 'FileAST: ')

        if showcoord:
            buf.write(' (at %s)' % self.coord)
        buf.write('\n')

        for c in self.children():
            c.show(buf, offset + 2, attrnames, showcoord)


class While(Node):
    def __init__(self, cond, stmt, coord=None):
        self.cond = cond
        self.stmt = stmt
        self.coord = coord

    def children(self):
        nodelist = []
        if self.cond is not None: nodelist.append(self.cond)
        if self.stmt is not None: nodelist.append(self.stmt)
        return tuple(nodelist)

    def show(self, buf=sys.stdout, offset=0, attrnames=False, showcoord=False):
        lead = ' ' * offset
        buf.write(lead + 'While: ')

        if showcoord:
            buf.write(' (at %s)' % self.coord)
        buf.write('\n')

        for c in self.children():
            c.show(buf, offset + 2, attrnames, showcoord)


class Continue(Node):
    def __init__(self, coord=None):
        self.coord = coord

    def children(self):
        return ()

    def show(self, buf=sys.stdout, offset=0, attrnames=False, showcoord=False):
        lead = ' ' * offset
        buf.write(lead + 'Continue: ')

        if showcoord:
            buf.write(' (at %s)' % self.coord)
        buf.write('\n')

        for c in self.children():
            c.show(buf, offset + 2, attrnames, showcoord)


class ParamList(Node):
    def __init__(self, params, coord=None):
        self.params = params
        self.coord = coord

    def children(self):
        nodelist = []
        if self.params is not None: nodelist.extend(self.params)
        return tuple(nodelist)

    def show(self, buf=sys.stdout, offset=0, attrnames=False, showcoord=False):
        lead = ' ' * offset
        buf.write(lead + 'ParamList: ')

        if showcoord:
            buf.write(' (at %s)' % self.coord)
        buf.write('\n')

        for c in self.children():
            c.show(buf, offset + 2, attrnames, showcoord)


class Constant(Node):
    def __init__(self, type, value, coord=None):
        self.type = type
        self.value = value
        self.coord = coord

    def children(self):
        nodelist = []
        return tuple(nodelist)

    def show(self, buf=sys.stdout, offset=0, attrnames=False, showcoord=False):
        lead = ' ' * offset
        buf.write(lead + 'Constant: ')

        if attrnames:
            attrstr = ', '.join('%s=%s' % nv for nv in [("type", repr(self.type)), ("value", repr(self.value))])
        else:
            attrstr = ', '.join('%s' % v for v in [self.type, self.value])
        buf.write(attrstr)

        if showcoord:
            buf.write(' (at %s)' % self.coord)
        buf.write('\n')

        for c in self.children():
            c.show(buf, offset + 2, attrnames, showcoord)


class ID(Node):
    def __init__(self, name, coord=None):
        self.name = name
        self.coord = coord

    def children(self):
        nodelist = []
        return tuple(nodelist)

    def show(self, buf=sys.stdout, offset=0, attrnames=False, showcoord=False):
        lead = ' ' * offset
        buf.write(lead + 'ID: ')

        if attrnames:
            attrstr = ', '.join('%s=%s' % nv for nv in [("name", repr(self.name))])
        else:
            attrstr = ', '.join('%s' % v for v in [self.name])
        buf.write(attrstr)

        if showcoord:
            buf.write(' (at %s)' % self.coord)
        buf.write('\n')

        for c in self.children():
            c.show(buf, offset + 2, attrnames, showcoord)


class Goto(Node):
    def __init__(self, name, coord=None):
        self.name = name
        self.coord = coord

    def children(self):
        nodelist = []
        return tuple(nodelist)

    def show(self, buf=sys.stdout, offset=0, attrnames=False, showcoord=False):
        lead = ' ' * offset
        buf.write(lead + 'Goto: ')

        if attrnames:
            attrstr = ', '.join('%s=%s' % nv for nv in [("name", repr(self.name))])
        else:
            attrstr = ', '.join('%s' % v for v in [self.name])
        buf.write(attrstr)

        if showcoord:
            buf.write(' (at %s)' % self.coord)
        buf.write('\n')

        for c in self.children():
            c.show(buf, offset + 2, attrnames, showcoord)


class Decl(Node):
    def __init__(self, qualifier, type, vars, coord=None):
        self.qualifier = qualifier
        self.type = type
        self.vars = vars
        self.coord = coord

    def children(self):
        nodelist = []
        if self.vars is not None: nodelist.extend(self.vars)
        return tuple(nodelist)

    def show(self, buf=sys.stdout, offset=0, attrnames=False, showcoord=False):
        lead = ' ' * offset
        buf.write(lead + 'Decl: ')

        if attrnames:
            attrstr = ', '.join('%s=%s' % nv for nv in [("qualifier", repr(self.qualifier)), ("type", repr(self.type))])
        else:
            attrstr = ', '.join('%s' % v for v in [self.qualifier, self.type])
        buf.write(attrstr)

        if showcoord:
            buf.write(' (at %s)' % self.coord)
        buf.write('\n')

        for c in self.children():
            c.show(buf, offset + 2, attrnames, showcoord)


class Return(Node):
    def __init__(self, expr, coord=None):
        self.expr = expr
        self.coord = coord

    def children(self):
        nodelist = []
        if self.expr is not None: nodelist.append(self.expr)
        return tuple(nodelist)

    def show(self, buf=sys.stdout, offset=0, attrnames=False, showcoord=False):
        lead = ' ' * offset
        buf.write(lead + 'Return: ')

        if showcoord:
            buf.write(' (at %s)' % self.coord)
        buf.write('\n')

        for c in self.children():
            c.show(buf, offset + 2, attrnames, showcoord)


class FuncCall(Node):
    def __init__(self, name, args, coord=None):
        self.name = name
        self.args = args
        self.coord = coord

    def children(self):
        nodelist = []
        if self.name is not None: nodelist.append(self.name)
        if self.args is not None: nodelist.append(self.args)
        return tuple(nodelist)

    def show(self, buf=sys.stdout, offset=0, attrnames=False, showcoord=False):
        lead = ' ' * offset
        buf.write(lead + 'FuncCall: ')

        if showcoord:
            buf.write(' (at %s)' % self.coord)
        buf.write('\n')

        for c in self.children():
            c.show(buf, offset + 2, attrnames, showcoord)


class Assignment(Node):
    def __init__(self, op, var, value, coord=None):
        self.op = op
        self.var = var
        self.value = value
        self.coord = coord

    def children(self):
        nodelist = []
        if self.var is not None: nodelist.append(self.var)
        if self.value is not None: nodelist.append(self.value)
        return tuple(nodelist)

    def show(self, buf=sys.stdout, offset=0, attrnames=False, showcoord=False):
        lead = ' ' * offset
        buf.write(lead + 'Assignment: ')

        if attrnames:
            attrstr = ', '.join('%s=%s' % nv for nv in [("op", repr(self.op))])
        else:
            attrstr = ', '.join('%s' % v for v in [self.op])
        buf.write(attrstr)

        if showcoord:
            buf.write(' (at %s)' % self.coord)
        buf.write('\n')

        for c in self.children():
            c.show(buf, offset + 2, attrnames, showcoord)


class ExprList(Node):
    def __init__(self, exprs, coord=None):
        self.exprs = exprs
        self.coord = coord

    def children(self):
        nodelist = []
        if self.exprs is not None: nodelist.extend(self.exprs)
        return tuple(nodelist)

    def show(self, buf=sys.stdout, offset=0, attrnames=False, showcoord=False):
        lead = ' ' * offset
        buf.write(lead + 'ExprList: ')

        if showcoord:
            buf.write(' (at %s)' % self.coord)
        buf.write('\n')

        for c in self.children():
            c.show(buf, offset + 2, attrnames, showcoord)


class Break(Node):
    def __init__(self, coord=None):
        self.coord = coord

    def children(self):
        return ()

    def show(self, buf=sys.stdout, offset=0, attrnames=False, showcoord=False):
        lead = ' ' * offset
        buf.write(lead + 'Break: ')

        if showcoord:
            buf.write(' (at %s)' % self.coord)
        buf.write('\n')

        for c in self.children():
            c.show(buf, offset + 2, attrnames, showcoord)


class DoWhile(Node):
    def __init__(self, cond, stmt, coord=None):
        self.cond = cond
        self.stmt = stmt
        self.coord = coord

    def children(self):
        nodelist = []
        if self.cond is not None: nodelist.append(self.cond)
        if self.stmt is not None: nodelist.append(self.stmt)
        return tuple(nodelist)

    def show(self, buf=sys.stdout, offset=0, attrnames=False, showcoord=False):
        lead = ' ' * offset
        buf.write(lead + 'DoWhile: ')

        if showcoord:
            buf.write(' (at %s)' % self.coord)
        buf.write('\n')

        for c in self.children():
            c.show(buf, offset + 2, attrnames, showcoord)


class Varible(Node):
    def __init__(self, type, name, index, coord=None):
        self.type = type
        self.name = name
        self.index = index
        self.coord = coord

    def children(self):
        nodelist = []
        if self.name is not None: nodelist.append(self.name)
        if self.index is not None: nodelist.append(self.index)
        return tuple(nodelist)

    def show(self, buf=sys.stdout, offset=0, attrnames=False, showcoord=False):
        lead = ' ' * offset
        buf.write(lead + 'Varible: ')

        if attrnames:
            attrstr = ', '.join('%s=%s' % nv for nv in [("type", repr(self.type))])
        else:
            attrstr = ', '.join('%s' % v for v in [self.type])
        buf.write(attrstr)

        if showcoord:
            buf.write(' (at %s)' % self.coord)
        buf.write('\n')

        for c in self.children():
            c.show(buf, offset + 2, attrnames, showcoord)


class BinaryOp(Node):
    def __init__(self, op, left, right, coord=None):
        self.op = op
        self.left = left
        self.right = right
        self.coord = coord

    def children(self):
        nodelist = []
        if self.left is not None: nodelist.append(self.left)
        if self.right is not None: nodelist.append(self.right)
        return tuple(nodelist)

    def show(self, buf=sys.stdout, offset=0, attrnames=False, showcoord=False):
        lead = ' ' * offset
        buf.write(lead + 'BinaryOp: ')

        if attrnames:
            attrstr = ', '.join('%s=%s' % nv for nv in [("op", repr(self.op))])
        else:
            attrstr = ', '.join('%s' % v for v in [self.op])
        buf.write(attrstr)

        if showcoord:
            buf.write(' (at %s)' % self.coord)
        buf.write('\n')

        for c in self.children():
            c.show(buf, offset + 2, attrnames, showcoord)


class Compound(Node):
    def __init__(self, decls, stmts, coord=None):
        self.decls = decls
        self.stmts = stmts
        self.coord = coord

    def children(self):
        nodelist = []
        if self.decls is not None: nodelist.extend(self.decls)
        if self.stmts is not None: nodelist.extend(self.stmts)
        return tuple(nodelist)

    def show(self, buf=sys.stdout, offset=0, attrnames=False, showcoord=False):
        lead = ' ' * offset
        buf.write(lead + 'Compound: ')

        if showcoord:
            buf.write(' (at %s)' % self.coord)
        buf.write('\n')

        for c in self.children():
            c.show(buf, offset + 2, attrnames, showcoord)


class VaribleDecl(Node):
    def __init__(self, name, init, coord=None):
        self.name = name
        self.init = init
        self.coord = coord

    def children(self):
        nodelist = []
        if self.name is not None: nodelist.append(self.name)
        return tuple(nodelist)

    def show(self, buf=sys.stdout, offset=0, attrnames=False, showcoord=False):
        lead = ' ' * offset
        buf.write(lead + 'VaribleDecl: ')

        if attrnames:
            attrstr = ', '.join('%s=%s' % nv for nv in [("init", repr(self.init))])
        else:
            attrstr = ', '.join('%s' % v for v in [self.init])
        buf.write(attrstr)

        if showcoord:
            buf.write(' (at %s)' % self.coord)
        buf.write('\n')

        for c in self.children():
            c.show(buf, offset + 2, attrnames, showcoord)


class EllipsisParam(Node):
    def __init__(self, coord=None):
        self.coord = coord

    def children(self):
        return ()

    def show(self, buf=sys.stdout, offset=0, attrnames=False, showcoord=False):
        lead = ' ' * offset
        buf.write(lead + 'EllipsisParam: ')

        if showcoord:
            buf.write(' (at %s)' % self.coord)
        buf.write('\n')

        for c in self.children():
            c.show(buf, offset + 2, attrnames, showcoord)


class If(Node):
    def __init__(self, cond, iftrue, iffalse, coord=None):
        self.cond = cond
        self.iftrue = iftrue
        self.iffalse = iffalse
        self.coord = coord

    def children(self):
        nodelist = []
        if self.cond is not None: nodelist.append(self.cond)
        if self.iftrue is not None: nodelist.append(self.iftrue)
        if self.iffalse is not None: nodelist.append(self.iffalse)
        return tuple(nodelist)

    def show(self, buf=sys.stdout, offset=0, attrnames=False, showcoord=False):
        lead = ' ' * offset
        buf.write(lead + 'If: ')

        if showcoord:
            buf.write(' (at %s)' % self.coord)
        buf.write('\n')

        for c in self.children():
            c.show(buf, offset + 2, attrnames, showcoord)


