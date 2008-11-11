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
     * <�������> ::= <�������><���>|<���>
     * @throws SyntaxException
     * @todo Implement this org.cc.rd.SyntaxTree method
     */
    protected void parseAction() throws SyntaxException {
        //��Ԥ��һ�����ţ�������ǽ��������Ϳ�ʼ������ʽ��
        while(!token.symbol.equals(BLOCK_END)){          //һֱ���������������
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
