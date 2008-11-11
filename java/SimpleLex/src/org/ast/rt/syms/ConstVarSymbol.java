package org.ast.rt.syms;

import org.le.Lex.Token;
import org.ast.rt.Value;

public class ConstVarSymbol extends VarSymbol {
    public ConstVarSymbol(Token var) {
        super(var,var);
        if(Token.TYPE_NUMBER == var.type){
            this.setValue(new NumberValue(var.symbol));
        }else if(Token.TYPE_STRING == var.type){
            this.setValue(new StringValue(var.symbol));
        }
    }

    public void setValue(StringValue v){
    }
    public void setValue(boolean v){
    }
    public void setValue(int v){
    }
    public void setValue(Value v){
    }
}
