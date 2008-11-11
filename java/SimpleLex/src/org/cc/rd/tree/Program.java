package org.cc.rd.tree;

import org.cc.rd.SyntaxTree;
import org.le.Lex.Lex;
import org.cc.rd.SyntaxException;
import static org.le.Lex.Tokens.*;
import org.le.Lex.Token;
import org.ast.rt.RunTime;

/**
 * Test�����﷨���ĸ���㡣
 * @author not attributable
 * @version 1.0
 */
public class Program extends SyntaxTree {
    private SyntaxTree declList = new DeclarationList();
    private SyntaxTree stateList = new StatementList();

    public void setRunTime(RunTime rt) {
        super.setRunTime(rt);
        declList.setRunTime(rt);
        stateList.setRunTime(rt);
    }

    public void setIsEvaluate(boolean isEvaluate) {
        super.setIsEvaluate(isEvaluate);
        declList.setIsEvaluate(isEvaluate);
        stateList.setIsEvaluate(isEvaluate);
    }

    /**
     * <����> ::= {<��������><�������>}
     * @return int
     */
    protected void parseAction() throws SyntaxException{
        if (token.equals(BLOCK_START)) {
            //��ʼ�����������С�
            error = declList.parse(currentLex());
            if (error == RET_OK) { //����������н����ɹ����������顣
                error = stateList.parse(currentLex());
                if (error == RET_OK) {
                    token = nextToken();
                    if (token == null || !token.equals(BLOCK_END)) {
                        error = ERR_NO_BLOCK_END;
                    }
                }
            }
        } else {
            error = ERR_NO_BLOCK_START;
        }
    }

}
