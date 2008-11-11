package org.ast.rt;

import org.le.Lex.Token;
import org.ast.rt.syms.NullValue;

public abstract class Symbol {
    public static final int VARIABLE_ID = 1;

    public int type;
    public String name;
    public Token token;
    public Value value = new NullValue();
    //public

    public Symbol(Token t, int type){
        this.token = t;
        this.type = type;
        this.name = t.symbol;
    }
    public int integerValue(){
        return value.integerValue();
    }

    public String stringValue() {
        return value.stringValue();
    }

    public boolean booleanValue() {
        return value.booleanValue();
    }

}
