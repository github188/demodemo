#-----------------------------------------------------------------
# pycparser: cparse.py
#
# CParser class: Parser and AST builder for the C language
#
# Copyright (C) 2008, Eli Bendersky
# License: LGPL
#-----------------------------------------------------------------

import re
import sys
from types import StringType

import ply.yacc

import c_ast
from c_lexer import CLexer
from plyparser import PLYParser, Coord, ParseError


class CParser(PLYParser):    
    def __init__(
            self, 
            lex_optimize=True,
            lextab='pytel.ast.lextab',
            yacc_optimize=True,
            yacctab='pytel.ast.yacctab',
            yacc_debug=False):
        """ Create a new CParser.
        
            Some arguments for controlling the debug/optimization
            level of the parser are provided. The defaults are 
            tuned for release/performance mode. 
            The simple rules for using them are:
            *) When tweaking CParser/CLexer, set these to False
            *) When releasing a stable parser, set to True
            
            lex_optimize:
                Set to False when you're modifying the lexer.
                Otherwise, changes in the lexer won't be used, if
                some lextab.py file exists.
                When releasing with a stable lexer, set to True
                to save the re-generation of the lexer table on 
                each run.
            
            lextab:
                Points to the lex table that's used for optimized
                mode. Only if you're modifying the lexer and want
                some tests to avoid re-generating the table, make 
                this point to a local lex table file (that's been
                earlier generated with lex_optimize=True)
            
            yacc_optimize:
                Set to False when you're modifying the parser.
                Otherwise, changes in the parser won't be used, if
                some parsetab.py file exists.
                When releasing with a stable parser, set to True
                to save the re-generation of the parser table on 
                each run.
            
            yacctab:
                Points to the yacc table that's used for optimized
                mode. Only if you're modifying the parser, make 
                this point to a local yacc table file
                        
            yacc_debug:
                Generate a parser.out file that explains how yacc
                built the parsing table from the grammar.
        """
        self.clex = CLexer(
            error_func=self._lex_error_func, )
            
        self.clex.build(
            optimize=lex_optimize,
            lextab=lextab)
        self.tokens = self.clex.tokens
        
        rules_with_opt = [
            'expression',
            'parameter_list',
            'constant_expression',
        ]
        
        for rule in rules_with_opt:
            self._create_opt_rule(rule)
        
        self.cparser = ply.yacc.yacc(
            module=self, 
            start='translation_unit',
            debug=yacc_debug,
            optimize=yacc_optimize,
            tabmodule=yacctab)
        
        # A table of identifiers defined as typedef types during
        # parsing.
        #
        self.typedef_table = set([])
    
    def parse(self, text, filename='', debuglevel=0):
        """ Parses C code and returns an AST.
        
            text:
                A string containing the C source code
            
            filename:
                Name of the file being parsed (for meaningful
                error messages)
            
            debuglevel:
                Debug level to yacc
        """
        self.clex.filename = filename
        self.clex.reset_lineno()
        self.typedef_table = set([])
        return self.cparser.parse(text, lexer=self.clex, debug=debuglevel)
    
    ######################--   PRIVATE   --######################
    
    def _lex_error_func(self, msg, line, column):
        self._parse_error(msg, self._coord(line, column))
    ##
    ## Precedence and associativity of operators
    ##
    precedence = (
        ('left', 'NOT'),
        ('left', 'BOR'),
        ('left', 'BXOR'),
        ('left', 'BAND'),
        ('left', 'EQ', 'NE'),
        ('left', 'GT', 'GE', 'LT', 'LE'),
        ('left', 'RSHIFT', 'LSHIFT'),
        ('left', 'PLUS', 'MINUS'),
        ('left', 'TIMES', 'DIVIDE', 'MOD'),
        ('left', 'AND'),
        ('left', 'OR'),
    )
    
    def p_translation_unit(self, p):
        """ translation_unit : external_decl_list
                             | translation_unit external_decl_list
        """
        # Note: external_declaration is already a list
        #
        if len(p) == 2:
            p[0] = c_ast.FileAST(p[1])
        else:
            p[1].ext.extend(p[2])
            p[0] = p[1]
    
    def p_external_decl_list(self, p):
        """ external_decl_list : external_decl 
                               | external_decl_list external_decl
        """
        if len(p) == 2:
            p[0] = p[1] if isinstance(p[1], list) else [p[1]]
        else:
            p[0] = p[1] + (p[2] if isinstance(p[2], list) else [p[2]])
            
    def p_external_decl_1(self, p):
        """ external_decl : external_declaration
                          | declaration_specifiers
        """
        p[0] = p[1]
        
    def p_external_decl_2(self, p):
        """ external_decl : function_definition
        """
        p[0] = p[1]

    def p_external_declaration(self, p):
        """ external_declaration : pp_directive """
        p[0] = p[1]

    def p_pp_directive(self, p):
        """ pp_directive : PPHASH """
        p[0] = p[1]

    def p_function_definition(self, p):
        """ function_definition : function_type_specifier FUNCTION identifier LPAREN parameter_list_opt RPAREN compound_statement ENDFUNCTION """
        
        decl = c_ast.FunctionDecl(p[1], p[3], p[5], p[7], self._coord(p.lineno(1)))
        p[0] = decl
    
    def p_statement(self, p):
        """ statement   : labeled_statement
                        | expression_statement
                        | selection_statement
                        | iteration_statement    
                        | jump_statement
        """
        p[0] = p[1]
        
    def p_declaration_specifiers(self, p):
        """ declaration_specifiers  : type_qualifier type_specifier init_declarator_list SEMI
                                    | type_specifier init_declarator_list SEMI
        """
        if len(p) == 5:
            decl = c_ast.Decl(p[1], p[2], p[3], coord=self._coord(p.lineno(1)))
        else:
            decl = c_ast.Decl(None, p[1], p[2], coord=self._coord(p.lineno(1)))
            
        p[0] = decl
    
    def p_declaration_specifiers_list(self, p):
        """ declaration_specifiers_list : declaration_specifiers
                                        | declaration_specifiers_list declaration_specifiers
        """
        p[0] = p[1] + [p[2]] if len(p) == 3 else [p[1]]
    
    def p_function_type_specifier(self, p):
        """ function_type_specifier : VOID
                                    | type_specifier
        """
        p[0] = p[1]
    
    def p_type_specifier(self, p):
        """ type_specifier : INTEGER
                           | FLOAT
                           | STRING
        """
        p[0] = p[1]
        
    def p_type_qualifier(self, p):
        """ type_qualifier  : CONST """
        p[0] = p[1]
    
    def p_init_declarator_list(self, p):
        """ init_declarator_list : init_declarator
                                 | init_declarator_list COMMA init_declarator
        """
        p[0] = p[1] + [p[3]] if len(p) == 4 else [p[1]]
    
    def p_init_declarator(self, p):
        """ init_declarator : variable_declarator
                            | variable_declarator EQUALS initializer
        """
        p[0] = c_ast.VaribleDecl(p[1], p[3] if len(p) > 2 else None)
    
    def p_variable_declarator(self, p):
        """ variable_declarator : identifier LBRACKET constant_expression_opt RBRACKET
                                | identifier
        """
        if len(p) == 5:
            var = c_ast.Varible('array', p[1], p[3], p[1].coord)
        else:
            var = c_ast.Varible('var', p[1], None, p[1].coord)
        
        p[0] = var
            
    def p_parameter_list(self, p):
        """ parameter_list : parameter_declaration
                           | parameter_list COMMA parameter_declaration
        """
        if len(p) == 2: # single parameter
            p[0] = c_ast.ParamList([p[1]], p[1].coord)
        else:
            p[1].params.append(p[3])
            p[0] = p[1]
                
    def p_parameter_declaration(self, p):
        # string abc
        """ parameter_declaration : type_specifier variable_declarator
                                  | type_specifier BAND variable_declarator
        """
        
        if len(p) == 3:
            param = c_ast.Param(False, p[1], p[2], self._coord(p.lineno(1)))
        else:
            param = c_ast.Param(True, p[1], p[3], self._coord(p.lineno(1)))
        
        out = False if len(p) == 3 else True
        
        p[0] = param
    
    def p_initializer_1(self, p):
        """ initializer : assignment_expression 
        """
        p[0] = p[1]
    
    def p_initializer_2(self, p):
        """ initializer : LBRACE initializer_list RBRACE
                        | LBRACE initializer_list COMMA RBRACE
        """
        p[0] = p[2]
    
    def p_initializer_list(self, p):
        """ initializer_list : initializer
                             | initializer_list COMMA initializer
        """
        p[0] = p[1] + [p[3]] if len(p) == 4 else [p[1]]      
    
    def p_compound_statement_1(self, p):
        """ compound_statement : statement_list """
        p[0] = c_ast.Compound(
            decls=None, 
            stmts=p[1], 
            coord=self._coord(p.lineno(1)))
        
    def p_compound_statement_2(self, p):
        """ compound_statement : declaration_specifiers_list """
        p[0] = c_ast.Compound(
            decls=p[1], 
            stmts=None, 
            coord=self._coord(p.lineno(1)))
        
    def p_compound_statement_3(self, p):
        """ compound_statement : declaration_specifiers_list statement_list """
        p[0] = c_ast.Compound(
            decls=p[1], 
            stmts=p[2], 
            coord=self._coord(p.lineno(1)))
     
    def p_statement_list(self, p):
        """ statement_list  : statement 
                            | statement_list statement
        """
        if len(p) == 2: # single expr
            p[0] = [p[1]] if p[1] else [] 
        else:
            p[0] = p[1] + ([p[2]] if p[2] else [])
        
    def p_labeled_statement_1(self, p):
        """ labeled_statement : ID COLON statement """
        p[0] = c_ast.Label(p[1], p[3], self._coord(p.lineno(1)))

    def p_labeled_statement_2(self, p):
        """ labeled_statement : CASE expression COLON statement """
        p[0] = c_ast.Case(p[2], p[4], self._coord(p.lineno(1)))
    
    def p_labeled_statement_3(self, p):
        """ labeled_statement : DEFAULT COLON statement """
        p[0] = c_ast.Default(p[3], self._coord(p.lineno(1)))

    def p_selection_statement_1(self, p):
        """ selection_statement : IF LPAREN expression RPAREN compound_statement ENDIF """
        p[0] = c_ast.If(p[3], p[5], None, self._coord(p.lineno(1)))

    def p_selection_statement_2(self, p):
        """ selection_statement : IF LPAREN expression RPAREN compound_statement ELSE compound_statement ENDIF """
        p[0] = c_ast.If(p[3], p[5], p[7], self._coord(p.lineno(1)))

    def p_selection_statement_3(self, p):
        """ selection_statement : SWITCH LPAREN expression RPAREN compound_statement ENDSWITCH """
        p[0] = c_ast.Switch(p[3], p[5], self._coord(p.lineno(1)))
    
    def p_iteration_statement_1(self, p):
        """ iteration_statement : WHILE LPAREN expression RPAREN compound_statement ENDWHILE """
        p[0] = c_ast.While(p[3], p[5], self._coord(p.lineno(1)))

    def p_iteration_statement_2(self, p):
        """ iteration_statement : DO compound_statement WHILE LPAREN expression RPAREN """
        p[0] = c_ast.DoWhile(p[5], p[2], self._coord(p.lineno(1)))

    def p_iteration_statement_3(self, p):
        """ iteration_statement : FOR LPAREN expression_opt SEMI expression_opt SEMI expression_opt RPAREN compound_statement ENDFOR """
        p[0] = c_ast.For(p[3], p[5], p[7], p[9], self._coord(p.lineno(1)))

    def p_jump_statement_1(self, p):
        """ jump_statement  : GOTO ID SEMI """
        p[0] = c_ast.Goto(p[2], self._coord(p.lineno(1)))

    def p_jump_statement_2(self, p):
        """ jump_statement  : BREAK SEMI """
        p[0] = c_ast.Break(self._coord(p.lineno(1)))
    
    def p_jump_statement_3(self, p):
        """ jump_statement  : CONTINUE SEMI """
        p[0] = c_ast.Continue(self._coord(p.lineno(1)))    

    def p_jump_statement_4(self, p):
        """ jump_statement  : RETURN expression SEMI  
                            | RETURN SEMI 
        """
        p[0] = c_ast.Return(p[2] if len(p) == 4 else None, self._coord(p.lineno(1)))  
    
    def p_expression_statement(self, p): 
        """ expression_statement : expression SEMI
                                 | SEMI
        """
        if len(p) == 2:
            p[0] = c_ast.ExprList([], self._coord(p.lineno(1)))
        else:
            p[0] = p[1] 
    
    def p_expression(self, p):
        """ expression  : assignment_expression 
                        | expression COMMA assignment_expression
        """
        if len(p) == 2:
            p[0] = p[1]
        else:
            if not isinstance(p[1], c_ast.ExprList):
                p[1] = c_ast.ExprList([p[1]], p[1].coord)
            
            p[1].exprs.append(p[3])
            p[0] = p[1]
    
    def p_assignment_expression(self, p):
        """ assignment_expression : conditional_expression
                                  | variable_declarator assignment_operator conditional_expression
        """
        if len(p) == 2:
            p[0] = p[1]
        else:
            p[0] = c_ast.Assignment(p[2], p[1], p[3], p[1].coord)        
    
    def p_assignment_operator(self, p):
        """ assignment_operator : EQUALS """
        p[0] = p[1]
    
    def p_constant_expression(self, p):
        """ constant_expression : conditional_expression """
        p[0] = p[1]
    
    def p_conditional_expression(self, p):
        """ conditional_expression  : binary_expression """
        p[0] = p[1]
    
    def p_binary_expression(self, p):
        """ binary_expression   : unary_expression
                                | binary_expression TIMES binary_expression
                                | binary_expression DIVIDE binary_expression
                                | binary_expression MOD binary_expression
                                | binary_expression PLUS binary_expression
                                | binary_expression MINUS binary_expression
                                | binary_expression RSHIFT binary_expression
                                | binary_expression LSHIFT binary_expression
                                | binary_expression LT binary_expression
                                | binary_expression LE binary_expression
                                | binary_expression GE binary_expression
                                | binary_expression GT binary_expression
                                | binary_expression EQ binary_expression
                                | binary_expression NE binary_expression
                                | binary_expression BAND binary_expression
                                | binary_expression BOR binary_expression
                                | binary_expression BXOR binary_expression
                                | binary_expression AND binary_expression
                                | binary_expression OR binary_expression
        """
        if len(p) > 2:
            p[0] = c_ast.BinaryOp(p.slice[2], p[1], p[3], p[1].coord)
        else:
            p[0] = p[1]
    
    def p_unary_expression_1(self, p):
        """ unary_expression : function_expression """
        p[0] = p[1]
    
    def p_unary_expression_2(self, p):
        """ unary_expression : primary_expression """
        p[0] = p[1]
    
    def p_unary_expression_3(self, p):
        """ unary_expression : PLUSPLUS unary_expression 
                             | MINUSMINUS unary_expression
                             | unary_operator unary_expression
        """
        p[0] = c_ast.UnaryOp(p.slice[1], p[2], self._coord(p.lineno(1)))
    
    def p_unary_operator(self, p):
        """ unary_operator  : PLUS
                            | MINUS
                            | NOT
                            | BNOT
        """
        p[0] = p.slice[1]
    
    def p_function_expression(self, p):
        """ function_expression  : identifier LPAREN argument_expression_list RPAREN
                                 | identifier LPAREN RPAREN
        """
        if len(p) == 5:
            fcall = c_ast.FuncCall(p[1], p[3], p[1].coord)
        else:
            fcall = c_ast.FuncCall(p[1], None, p[1].coord)
            
        p[0] = fcall
    
    def p_primary_expression_1(self, p):
        """ primary_expression  : variable_declarator """
        p[0] = p[1]
    
    def p_primary_expression_2(self, p):
        """ primary_expression  : constant """
        p[0] = p[1]

    def p_primary_expression_3(self, p):
        """ primary_expression  : STRING_LITERAL """
        p[0] = c_ast.Constant('string', p[1], self._coord(p.lineno(1)))
    
    def p_primary_expression_4(self, p):
        """ primary_expression  : LPAREN expression RPAREN """
        p[0] = p[2]
    
    def p_argument_expression_list(self, p):
        """ argument_expression_list : assignment_expression 
                                     | argument_expression_list COMMA assignment_expression
        """
        if len(p) == 2: # single expr
            p[0] = c_ast.ExprList([p[1]], p[1].coord)
        else:
            p[1].exprs.append(p[3])
            p[0] = p[1]
    
    def p_identifier(self, p):
        """ identifier  : ID """
        p[0] = c_ast.ID(p[1], self._coord(p.lineno(1)))
    
    def p_constant_1(self, p):
        """ constant : INT_CONST_DEC
                     | INT_CONST_OCT
                     | INT_CONST_HEX
        """
        p[0] = c_ast.Constant("integer", p[1], self._coord(p.lineno(1)))
        
    def p_constant_2(self, p):
        """ constant : FLOAT_CONST """
        p[0] = c_ast.Constant("float", p[1], self._coord(p.lineno(1)))
    
    def p_constant_3(self, p):
        """ constant : CHAR_CONST """
        p[0] = c_ast.Constant("char", p[1], self._coord(p.lineno(1)))
    
    def p_empty(self, p):
        'empty : '
        p[0] = None
        
    def p_error(self, p):
        if p:
            self._parse_error(
                'before: %s' % p.value, 
                self._coord(p.lineno))
        else:
            self._parse_error('At end of input', '')


if __name__ == "__main__":
    import pprint
    import time
    
    t1 = time.time()
    parser = CParser(lex_optimize=True, yacc_debug=True, yacc_optimize=False)
    print time.time() - t1
    
    buf = ''' 
        string var;
        const string var2;
        string function a()
        const string var2;
         ++a;
        endfunction
    '''
    
    # set debuglevel to 2 for debugging
    t = parser.parse(buf, 'x.c', debuglevel=0)
    t.show(showcoord=True)
