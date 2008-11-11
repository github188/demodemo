package org.cc.rd.tree;

import org.cc.rd.SyntaxException;
import org.cc.rd.SyntaxTree;
import static org.le.Lex.Token.*;
import static org.le.Lex.Tokens.*;
import org.le.Lex.Token;

public class DeclarationStat extends SyntaxTree {
    /**
     * <声明语句> ::= int <变量>;
     *
     */
    protected void parseAction() throws SyntaxException {
        if(token.type == TYPE_IDENTIFIER){      //声明关键字在 Declaration中已被解析。
            //rt.declaration(rt.peekToken(),token);
            Token id = token.copy();
            if(nextToken() == null || !token.symbol.equals(STATE_END)){
                error = ERR_NO_STATE_END;
            }else if(isEvaluate){
                error = rt.declaration(rt.peekToken(), id);
            }
        }else {
            error = ERR_NO_IDENTIFIER;
        }
    }


}
