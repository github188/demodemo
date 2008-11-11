package org.ast.rt.syms;

import org.ast.rt.Symbol;
import org.le.Lex.Token;
import static org.le.Lex.Tokens.*;
import org.ast.rt.Value;

public class VarSymbol extends Symbol {
    public static final int TYPE_INT = 1;
    public static final int TYPE_UNKNOW = 1;

    private int varType = 0;

    public VarSymbol(Token type,Token id) {
        super(id,VARIABLE_ID);

        if(type.equals(DECLARATION_INT)){
            varType = TYPE_INT;
        }else {
            varType = TYPE_UNKNOW;
        }
    }

    public void setValue(String v){
        value.setValue(v);
    }
    public void setValue(boolean v){
        value.setValue(v);
    }
    public void setValue(int v){
        value.setValue(v);
    }
    public void setValue(Value v){
        value.setValue(v);
    }

    public String toString(){
        return name + "=" + stringValue();
    }
}
