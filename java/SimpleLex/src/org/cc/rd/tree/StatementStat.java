package org.cc.rd.tree;

import org.cc.rd.SyntaxException;
import org.cc.rd.SyntaxTree;
import java.util.*;
import static org.le.Lex.Token.*;
import static org.le.Lex.Tokens.*;
import org.ast.rt.RunTime;
import org.le.Lex.Token;

public class StatementStat extends SyntaxTree {
    private Map<String, SyntaxTree> statMap = new HashMap<String, SyntaxTree>();
    //���֡����ʽ���롰���ʽ��䡱�Ĳ�ͬ������û������������
    private SyntaxTree expr = new StatementEXPR();
    private SyntaxTree exprState = new StatementEXPRState();
    private SyntaxTree state = null;
    public StatementStat(){
        statMap.put(STATE_IF, new StatementIF());
        statMap.put(STATE_WHILE, new StatementWHILE());
        statMap.put(STATE_FOR, new StatementFOR());
        statMap.put(STATE_READ, new StatementREAD());
        statMap.put(STATE_WRITE, new StatementWRITE());
        statMap.put(STATE_COMP, new StatementCOMP());

        state = this;
    }

    public void setRunTime(RunTime rt) {
        super.setRunTime(rt);
        exprState.setRunTime(rt);
        expr.setRunTime(rt);
        for (SyntaxTree syn : statMap.values()) {
            syn.setRunTime(rt);
        }
    }


    public void setIsEvaluate(boolean isEvaluate) {
        super.setIsEvaluate(isEvaluate);
        exprState.setIsEvaluate(isEvaluate);
        expr.setIsEvaluate(isEvaluate);
        for(SyntaxTree syn :statMap.values()){
            syn.setIsEvaluate(isEvaluate);
        }
    }


    /**
     *
     * <���> ::= <if>|<while>|<for>|<read>|<write>|<�������>|<���ʽ���>
     * @throws SyntaxException
     * @todo Implement this org.cc.rd.SyntaxTree method
     */
    protected void parseAction() throws SyntaxException {
        SyntaxTree syn = statMap.get(token.symbol);
        if(syn != null){
            error = syn.parse(currentLex());
        }else //if (token.type == TYPE_IDENTIFIER)
        {
            pushToken();         //����Ԥ������,�����ŷŵ�exprState��ȥ����
            error = exprState.parse(currentLex());
        }
    }

    class StatementIF extends SyntaxTree {
        /**
         * <if> ::= if(<���ʽ>)<���>[else<���>]
         */
        protected void parseAction() throws SyntaxException {
            if (token.equals(EXPR_START)) {
                error = expr.parse(currentLex());
                if (error == RET_OK) {
                    if (nextToken() != null && token.equals(EXPR_END)) {
                        boolean exprValue = true;
                        if(isEvaluate){
                            exprValue = rt.peekVar().booleanValue();
                            rt.clearVar();  //��ձ��ʽ��ֵ������ݡ�
                        }
                        boolean eval = isEvaluate;
                        state.setIsEvaluate(exprValue && isEvaluate);
                        error = state.parse(currentLex());
                        state.setIsEvaluate(eval);
                        //Ԥ����һ�������"else"�ٴ�����ʽ�������˻�Ԥ����
                        if (nextToken() != null && token.equals(STATE_ELSE)) {
                            state.setIsEvaluate(!exprValue && isEvaluate);
                            error = state.parse(currentLex());
                            state.setIsEvaluate(eval);
                        } else {
                            pushToken();
                        }
                    } else {
                        error = ERR_NO_EXPR_END;
                    }
                }
            } else {
                error = ERR_NO_EXPR_START;
            }
        }
    }


    class StatementWHILE extends SyntaxTree {
        /**
         * <while> ::= while(<���ʽ>) ���
         */
        protected void parseAction() throws SyntaxException {
            if (token.equals(EXPR_START)) {
                boolean exprValue = false;
                do{
                    Token whileStart = token.copy();
                    error = expr.parse(currentLex());
                    if (error == RET_OK) {
                        if (nextToken() != null && token.equals(EXPR_END)) {
                            if(isEvaluate){
                                exprValue = rt.peekVar().booleanValue();
                                rt.clearVar();  //��ձ��ʽ��ֵ������ݡ�
                            }
                            boolean eval = isEvaluate;
                            state.setIsEvaluate(exprValue && isEvaluate);
                            error = state.parse(currentLex());
                            state.setIsEvaluate(eval);
                            //error = state.parse(currentLex());
                        } else {
                            error = ERR_NO_EXPR_END;
                        }
                    }
                    if(exprValue){      //���while���ʽΪ�棬�������¶Ա��ʽ��ֵ��
                        token = whileStart;
                        pushToken();
                    }
                }while(isEvaluate && exprValue);
            } else {
                error = ERR_NO_EXPR_START;
            }
        }
    }

    class StatementFOR extends SyntaxTree {
        /**
         * <for> ::= for(<expr>;<expr>;<expr>)<���>
         */
        protected void parseAction() throws SyntaxException {
            Token forStart = null, forAfter = null, forBody = null, forEnd = null;
            //for���ʽ�ĵ�һ�������ֻ���ҳ��������ʽ�Ŀ�ʼlabel���������ʽ������ɺ��ٽ�����ֵ����
            //Ŀǰ����֧��ѭ��������
            boolean eval = isEvaluate;
            if (token.equals(EXPR_START)) {
                error = expr.parse(currentLex()); //��ʼ�����ʽ��
                if(isEvaluate){
                    rt.clearVar();
                }
                expr.setIsEvaluate(false);
                state.setIsEvaluate(false);
                if (error == RET_OK) {
                    if (nextToken() != null && token.equals(STATE_END)) {
                        forStart = token.copy(); //�������ʽ��ʼ��
                        error = expr.parse(currentLex());
                        if(error == RET_OK){
                            if (nextToken() != null && token.equals(STATE_END)) {
                                forAfter = token.copy();
                                error = expr.parse(currentLex()); //ѭ���������ʽ��
                                if(error == RET_OK){
                                    if (nextToken() != null &&
                                        token.equals(EXPR_END)) {
                                        forBody = token.copy();
                                        error = state.parse(currentLex()); //ѭ���塣
                                        forEnd = token.copy();
                                    } else {
                                        error = ERR_NO_EXPR_END;
                                    }
                                }
                            } else {
                                error = ERR_NO_STATE_END;
                            }
                        }
                    } else {
                        error = ERR_NO_STATE_END;
                    }
                }
            } else {
                error = ERR_NO_EXPR_START;
            }
            if(error == RET_OK && eval){   //��forѭ����ֵ.
                expr.setIsEvaluate(true);
                state.setIsEvaluate(true);
                boolean forCondition = false;
                do{
                    jump(forStart);
                    //nextToken();
                    error = expr.parse(currentLex());  //���������ʽ��ֵ��
                    if (error == RET_OK) {
                        forCondition = rt.peekVar().booleanValue();
                        rt.clearVar();
                        if(forCondition){
                            jump(forBody);
                            //nextToken();
                            error = state.parse(currentLex()); //����ѭ���塣
                            if (error == RET_OK) {
                                jump(forAfter);
                                //nextToken();
                                error = expr.parse(currentLex()); //��ѭ���������ʽ��ֵ��
                                rt.clearVar();
                                //if(error != RET_OK)break;
                            }
                        }
                    }
                }while(forCondition && error == RET_OK);
                jump(forEnd);
            }
            expr.setIsEvaluate(eval);
            state.setIsEvaluate(eval);
        }
        private void jump(Token t) throws SyntaxException {
            token = t;
            pushToken();
            nextToken();
        }
    }


    class StatementWRITE extends SyntaxTree {
        /**
         * <write> ::= write <expr>;
         */
        protected void parseAction() throws SyntaxException {
            pushToken();
            error = expr.parse(currentLex());
            if(error == RET_OK){
                if(nextToken() == null || !token.equals(STATE_END)){
                    error = ERR_NO_STATE_END;
                }else if(isEvaluate){
                    rt.ioOut();
                    rt.clearVar();
                }
            }
        }
    }


    class StatementREAD extends SyntaxTree {
        /**
         * <read> :: = read <����>;
         */
        protected void parseAction() throws SyntaxException {
            Token id = token.copy();
            if(token.type == TYPE_IDENTIFIER){
                if(nextToken() == null || !token.equals(STATE_END)){
                    error = ERR_NO_STATE_END;
                }else if(isEvaluate && (error = rt.LoadVar(id)) == RET_OK){
                    rt.ioIn();
                    rt.clearVar();
                }
            }else {
                error = ERR_NO_IDENTIFIER;
            }
        }
    }


    class StatementCOMP extends SyntaxTree {
        /**
         * <�������> ::= {����б�}
         */
        protected void parseAction() throws SyntaxException {
            //ע����StatementList�еķ������������������Ų��˻ء�
            //������ʹ��StatementList���󣬻������칹������ѭ�����á�
            while (!token.symbol.equals(BLOCK_END)) { //һֱ���������������
                pushToken(); //
                error = state.parse(currentLex());
                if (error != RET_OK) {
                    break;
                } else {
                    nextToken();
                }
            }
        }
    }


    class StatementEXPRState extends SyntaxTree {
        /**
         * <expr_state> ::= <expr>;|;
         */
        protected void parseAction() throws SyntaxException {
            if(!token.equals(STATE_END)){
                pushToken();
                error = expr.parse(currentLex());
                if(error == RET_OK){
                    if(nextToken() == null || !token.equals(STATE_END)){
                        error = ERR_NO_STATE_END;
                    }else if(isEvaluate){
                        rt.clearVar();      //��ձ��ʽ��ֵ��ջ��
                    }
                }
            }
        }
    }

}
