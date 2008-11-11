package org.cc.rd.tree;

import org.cc.rd.SyntaxException;
import org.cc.rd.SyntaxTree;
import static org.le.Lex.Tokens.*;
import org.ast.rt.RunTime;

public class StatementList extends SyntaxTree {
    private SyntaxTree state = new StatementStat();

    public void setRunTime(RunTime rt) {
        super.setRunTime(rt);
        state.setRunTime(rt);
    }
    public void setIsEvaluate(boolean isEvaluate) {
        super.setIsEvaluate(isEvaluate);
        state.setIsEvaluate(isEvaluate);
    }

    /**
     * <语句序列> ::= <语句序列><语句>|<语句>
     * @throws SyntaxException
     * @todo Implement this org.cc.rd.SyntaxTree method
     */
    protected void parseAction() throws SyntaxException {
        //先预读一个符号，如果不是结束符，就开始处理表达式。
        while(!token.symbol.equals(BLOCK_END)){          //一直解析到语句块结束。
            pushToken();                                  //
            error = state.parse(currentLex());
            if(error != RET_OK){
                break;
            }else {
                nextToken();
            }
        }
        if(error == RET_OK){
            pushToken();
        }
    }
}
