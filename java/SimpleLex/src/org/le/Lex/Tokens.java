package org.le.Lex;

public interface Tokens {
    public String BLOCK_START = "{";
    public String BLOCK_END = "}";

    public String EXPR_START = "(";
    public String EXPR_END = ")";

    public String DECLARATION_INT = "int";

    public String STATE_IF = "if";
    public String STATE_WHILE = "while";
    public String STATE_FOR = "for";
    public String STATE_READ = "read";
    public String STATE_WRITE = "write";
    public String STATE_COMP = "{";

    public String STATE_ELSE = "else";

    public String STATE_END = ";";

    public String BOOL_OPERATORS = ">,>=,<,<=,==,!=";
    public String ADDITIVE_OPERATORS = "+,-";
    public String TERM_OPERATORS = "*,/";
}
