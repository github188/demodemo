package org.cc.rd;

import org.le.Lex.Lex;
import org.le.Lex.Token;
import org.ast.rt.RunTime;

public abstract class SyntaxTree {
    public static final int RET_OK = 0;
    public static final int ERR_NO_BLOCK_START = 1;
    public static final int ERR_NO_BLOCK_END = 2;

    public static final int ERR_NO_IDENTIFIER = 3;
    public static final int ERR_NO_STATE_END = 4;

    public static final int ERR_NO_EXPR_START = 5;
    public static final int ERR_NO_EXPR_END = 6;
    public static final int ERR_NO_OPT_DATA = 7;

    public static final int ERR_LEX = 10;

    protected int error = 0;
    protected Token token = null;
    protected Lex lex = null;
    protected RunTime rt = null;
    protected boolean isEvaluate;  //�Ƿ�Ա��ʽ��ֵ��

    //public abstract int parse(Lex lex) throws SyntaxException;

    protected void reSetError(){
        error = RET_OK;
    }

    public int parse(Lex lex) throws SyntaxException {
        reSetError();
        this.lex = lex;
        nextToken();
        if (token == null)return RET_OK;
        parseAction();
        return error;
    }

    public Lex currentLex(){
        return lex;
    }
    public Token nextToken() throws SyntaxException{
        token = lex.nextToken();
        if(token.errorCode != Token.OK){
            error = ERR_LEX;
            throw new SyntaxException("�ʷ���������");
        }
        //System.out.println(token);
        return token;
    }

    /**
     * ����һ��token. ��ĳЩ�����Ԥ��һ�����š�
     */
    public void pushToken(){
        lex.pushToken(token);
        //System.out.println("push:" + token);
    }

    public void setRunTime(RunTime rt){
        this.rt = rt;
    }

    public void setIsEvaluate(boolean isEvaluate){
        this.isEvaluate = isEvaluate;
    }

    public String errorMessage(){
        StringBuffer buffer = new StringBuffer();
        String err = null;
        Token t = lex.peek();
        // Error(2):ȱ��{!, in line:1,col:20;
        buffer.append("Error(" + error +"):");
        switch (error) {
        case RET_OK: err = "�﷨�����ɹ�";         break;
        case ERR_NO_BLOCK_START: err = "ȱ��{!";  break;
        case ERR_NO_BLOCK_END: err = "ȱ��}!";    break;
        case ERR_NO_IDENTIFIER: err = "ȱ�ٱ�ʶ��!";      break;
        case ERR_NO_STATE_END:  err = "ȱ�ٷֺ�(;)";      break;
        case ERR_NO_EXPR_START: err = "ȱ��(!";           break;
        case ERR_NO_EXPR_END:   err = "ȱ��)!";           break;
        case ERR_NO_OPT_DATA:   err = "ȱ�ٲ�����!";       break;
        case ERR_LEX:   err = "�ʷ���������!";       break;
        default:
            if(rt != null){
                err = rt.errorMessage(error);
            }else {
                err = "δ֪�����룡(" + error + ")";
            }
        }
        buffer.append(err + ", in line:");
        buffer.append(t.line + 1);
        buffer.append(",col:");
        buffer.append(t.first - t.lineOffset);

        return buffer.toString();
    }

    protected abstract void parseAction()  throws SyntaxException;
}
