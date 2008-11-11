package org.cc.rd.tree;

import org.cc.rd.SyntaxException;
import org.cc.rd.SyntaxTree;
import org.le.Lex.Token;
import static org.le.Lex.Token.*;
import static org.le.Lex.Tokens.*;
import org.le.Lex.Lex;
import org.ast.rt.syms.VarSymbol;
import org.ast.rt.RunTime;

public class StatementEXPR extends SyntaxTree {
    SyntaxTree boolExpr = new BoolExpr();
    SyntaxTree addExpr = new AdditvieExpr();
    SyntaxTree term = new Term();
    SyntaxTree factor = new Factor();
    SyntaxTree expr = null;
    public StatementEXPR(){
        expr = this;
    }

    public int parse(Lex lex) throws SyntaxException {
        reSetError();
        this.lex = lex;
        parseAction();
        return error;
    }

    public void setRunTime(RunTime rt) {
        super.setRunTime(rt);
        boolExpr.setRunTime(rt);
        addExpr.setRunTime(rt);
        term.setRunTime(rt);
        factor.setRunTime(rt);
    }


    public void setIsEvaluate(boolean isEvaluate) {
        super.setIsEvaluate(isEvaluate);
        boolExpr.setIsEvaluate(isEvaluate);
        addExpr.setIsEvaluate(isEvaluate);
        term.setIsEvaluate(isEvaluate);
        factor.setIsEvaluate(isEvaluate);
    }


    /**
     * <expr> ::= <ID> = <bool_expr>|<bool_expr>
     */
    protected void parseAction() throws SyntaxException {
        Token rollBack = nextToken().copy();        //����һ��������ԭ�㡣
        if(token.type == TYPE_IDENTIFIER){  //Ԥ�����������ж��Ǹ�ֵ�����Ǳ��ʽ�����������Ƶ���
            if(isEvaluate){
                error = rt.LoadVar(token); //���ر�������������ջ��
            }
            if(error != RET_OK) return;
            if(nextToken() != null && token.equals("=")){  //�����ֵ��䣬����������
                error = boolExpr.parse(currentLex());
                if(isEvaluate && error == RET_OK){
                    VarSymbol src = rt.popVar();
                    error = rt.AssignVar(rt.peekVar(), src); //ʵ�ָ�ֵ������
                }
            }else {    //���ص����ʽ��һ�����ţ���Ϊһ��boolean���ʽ������
                token = rollBack;
                pushToken();
                if(isEvaluate){
                    rt.popVar(); //����Ԥ�ȼ��ص�һ����������boolExpr�л����¼��ر�����
                }
                error = boolExpr.parse(currentLex());
            }
        }else {                          //���ʽ����(�����ֿ�ͷ��
            pushToken();
            error = boolExpr.parse(currentLex());
        }
    }

    private boolean isOperator(Token t, String ops){
        for (String op : ops.split(",")) {
            if (t.equals(op)) {
                return true;
            }
        }
        return false;
    }

    private abstract class NoPreTokenExpr extends SyntaxTree{
        public int parse(Lex lex) throws SyntaxException {
            reSetError();
            this.lex = lex;
            parseAction();
            return error;
        }
    }
    class BoolExpr extends NoPreTokenExpr {
        /**
         * <bool_expr> ::= <additive_expr> | <additive_expr>(bool_op)<additive_expr>
         */
        protected void parseAction() throws SyntaxException {
            error = addExpr.parse(currentLex());
            if(error == RET_OK){
                if(nextToken() != null && isOperator(token, BOOL_OPERATORS)){
                    Token op = token.copy();
                    error = addExpr.parse(currentLex());
                    if(isEvaluate && error == RET_OK){
                        error = rt.execute(op);
                    }
                }else {
                    pushToken();
                }
            }
        }
    }


    class AdditvieExpr extends NoPreTokenExpr {
        /**
         * <additive_expr> ::= <term>{(+|-)<term>}
         */
        protected void parseAction() throws SyntaxException {
            error = term.parse(currentLex());
            if(error == RET_OK){
                while(nextToken() != null && isOperator(token, ADDITIVE_OPERATORS)){
                    Token op = token.copy();
                    error = term.parse(currentLex());
                    if(error != RET_OK){
                        break;
                    }else if(isEvaluate){
                        error = rt.execute(op);
                    }
                }
                pushToken();
            }
        }
    }


    class Term extends NoPreTokenExpr {
        /**
         * <term> ::= <factor>{(*|/)<factor>}
         */
        protected void parseAction() throws SyntaxException {
            error = factor.parse(currentLex());
            if(error == RET_OK){
                while(nextToken() != null && isOperator(token, TERM_OPERATORS)){
                    Token op = token.copy();
                    error = factor.parse(currentLex());
                    if(error != RET_OK){
                        break;
                    }else if(isEvaluate) {
                        error = rt.execute(op);
                    }
                }
                pushToken();
            }
        }
    }

    //��Ҫ��һ��Ԥ��token,����û�м̳�StatementEXPR
    class Factor extends SyntaxTree {
        /**
         * <factor> ::= (<additive_expr>)|ID|NUM
         */
        protected void parseAction() throws SyntaxException {
            if(token.equals(EXPR_START)){ //������Ƕ�ױ��ʽ��
                error = expr.parse(currentLex());
                if(error == RET_OK){
                    if(nextToken () == null || !token.equals(EXPR_END)){
                        error = ERR_NO_EXPR_END;
                    }
                }
            } else if(token.type == TYPE_IDENTIFIER || token.type == TYPE_NUMBER){  //�������ղ����κβ�����
                if (isEvaluate) {
                    error = rt.LoadVar(token);
                }
            } else {      //ȱ�ٲ�������
                error = ERR_NO_OPT_DATA;
            }
        }
    }

}
