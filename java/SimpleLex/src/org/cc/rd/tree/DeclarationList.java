package org.cc.rd.tree;

import org.cc.rd.SyntaxException;
import org.cc.rd.SyntaxTree;
import static org.le.Lex.Tokens.*;
import org.le.Lex.Token;
import org.ast.rt.RunTime;

public class DeclarationList extends SyntaxTree {
    private SyntaxTree declStat = new DeclarationStat();

    public void setRunTime(RunTime rt) {
        super.setRunTime(rt);
        declStat.setRunTime(rt);
    }

    public void setIsEvaluate(boolean isEvaluate) {
        super.setIsEvaluate(isEvaluate);
        declStat.setIsEvaluate(isEvaluate);
    }


    /**
     * <声明序列> ::= <声明序列><声明语句>|<声明语句>
     * 例如：
     * int a;
     * int b;
     * int c;
     * @throws SyntaxException
     */
    protected void parseAction() throws SyntaxException {
        while(token.symbol.equals(DECLARATION_INT)){
            //Token type = token.copy();
            if(isEvaluate){
                rt.pushToken(token.copy());
            }
            error = declStat.parse(currentLex());
            if(isEvaluate){
                rt.popToken();
            }
            if(error != RET_OK){
                break;
            }else {
                nextToken();
            }
        }
        if(error == RET_OK){  //本次解析中向前多读了一个符号，如果解析正确。将多读的符号push到Lex中。
            pushToken();
        }
    }
}
