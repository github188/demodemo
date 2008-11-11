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
    //区分“表达式”与“表达式语句”的不同，后者没有语句结束符。
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
     * <语句> ::= <if>|<while>|<for>|<read>|<write>|<复合语句>|<表达式语句>
     * @throws SyntaxException
     * @todo Implement this org.cc.rd.SyntaxTree method
     */
    protected void parseAction() throws SyntaxException {
        SyntaxTree syn = statMap.get(token.symbol);
        if(syn != null){
            error = syn.parse(currentLex());
        }else //if (token.type == TYPE_IDENTIFIER)
        {
            pushToken();         //回退预读符号,将符号放到exprState中去处理。
            error = exprState.parse(currentLex());
        }
    }

    class StatementIF extends SyntaxTree {
        /**
         * <if> ::= if(<表达式>)<语句>[else<语句>]
         */
        protected void parseAction() throws SyntaxException {
            if (token.equals(EXPR_START)) {
                error = expr.parse(currentLex());
                if (error == RET_OK) {
                    if (nextToken() != null && token.equals(EXPR_END)) {
                        boolean exprValue = true;
                        if(isEvaluate){
                            exprValue = rt.peekVar().booleanValue();
                            rt.clearVar();  //清空表达式求值后的数据。
                        }
                        boolean eval = isEvaluate;
                        state.setIsEvaluate(exprValue && isEvaluate);
                        error = state.parse(currentLex());
                        state.setIsEvaluate(eval);
                        //预读下一个如果是"else"再处理表达式。否则退回预读。
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
         * <while> ::= while(<表达式>) 语句
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
                                rt.clearVar();  //清空表达式求值后的数据。
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
                    if(exprValue){      //如果while表达式为真，返回重新对表达式求值。
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
         * <for> ::= for(<expr>;<expr>;<expr>)<语句>
         */
        protected void parseAction() throws SyntaxException {
            Token forStart = null, forAfter = null, forBody = null, forEnd = null;
            //for表达式的第一遍解析，只是找出各个表达式的开始label，整个表达式解析完成后再进行求值处理。
            //目前还不支持循环变量。
            boolean eval = isEvaluate;
            if (token.equals(EXPR_START)) {
                error = expr.parse(currentLex()); //初始化表达式。
                if(isEvaluate){
                    rt.clearVar();
                }
                expr.setIsEvaluate(false);
                state.setIsEvaluate(false);
                if (error == RET_OK) {
                    if (nextToken() != null && token.equals(STATE_END)) {
                        forStart = token.copy(); //条件表达式开始。
                        error = expr.parse(currentLex());
                        if(error == RET_OK){
                            if (nextToken() != null && token.equals(STATE_END)) {
                                forAfter = token.copy();
                                error = expr.parse(currentLex()); //循环结束表达式。
                                if(error == RET_OK){
                                    if (nextToken() != null &&
                                        token.equals(EXPR_END)) {
                                        forBody = token.copy();
                                        error = state.parse(currentLex()); //循环体。
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
            if(error == RET_OK && eval){   //对for循环求值.
                expr.setIsEvaluate(true);
                state.setIsEvaluate(true);
                boolean forCondition = false;
                do{
                    jump(forStart);
                    //nextToken();
                    error = expr.parse(currentLex());  //对条件表达式求值。
                    if (error == RET_OK) {
                        forCondition = rt.peekVar().booleanValue();
                        rt.clearVar();
                        if(forCondition){
                            jump(forBody);
                            //nextToken();
                            error = state.parse(currentLex()); //计算循环体。
                            if (error == RET_OK) {
                                jump(forAfter);
                                //nextToken();
                                error = expr.parse(currentLex()); //对循环结束表达式求值。
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
         * <read> :: = read <变量>;
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
         * <复合语句> ::= {语句列表}
         */
        protected void parseAction() throws SyntaxException {
            //注意与StatementList中的方法区别。在于最后个符号不退回。
            //不能再使用StatementList对象，会引起造构方法的循环引用。
            while (!token.symbol.equals(BLOCK_END)) { //一直解析到语句块结束。
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
                        rt.clearVar();      //清空表达式求值堆栈。
                    }
                }
            }
        }
    }

}
