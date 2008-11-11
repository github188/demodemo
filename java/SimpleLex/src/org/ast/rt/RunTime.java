package org.ast.rt;

import org.le.Lex.Token;
import java.util.*;
import org.ast.rt.syms.*;
import static org.ast.rt.Symbol.*;
import static org.cc.rd.SyntaxTree.*;
import static org.le.Lex.Token.*;
import java.io.*;

public class RunTime {
    public static final int ERR_VAR_REDEFINE = 22;
    public static final int ERR_VAR_NOTFOUND = 23;
    public static final int ERR_VAR_UNKNOWTYPE = 99;

    public static final int ERR_ASSIGN_NOTVAR = 100;

    private static Token dummy = new Token();

    private Stack<Token> tokenStack = new Stack<Token>();
    private Stack<VarSymbol> dataStack = new Stack<VarSymbol>();

    private Map<String,Symbol> symbols = new HashMap<String,Symbol>();

    static {
        dummy.symbol = "0";
        dummy.type = TYPE_NUMBER;
    }

    public void pushToken(Token t){
        tokenStack.push(t);
    }

    public Token peekToken(){
        return tokenStack.peek();
    }

    public Token popToken() {
        return tokenStack.pop();
    }

    public int declaration(Token type, Token id){
        Symbol var = symbols.get(id.symbol);
        if(var == null || var.type != VARIABLE_ID){
            var = new VarSymbol(type,id);
            System.out.println("Decl Var: type:" + type + ", id:" + id);
            symbols.put(var.name, var);
            return RET_OK;
        }
        return ERR_VAR_REDEFINE;
    }

    public int ioOut(){
        if(peekVar() != null){
            System.out.println("ioOut var:" + peekVar().toString());
            System.out.print(peekVar().stringValue());
        }
        return 0;
    }

    public int ioIn(){
        if(peekVar() != null){
            try {
                System.out.println("ioIn var:" + peekVar().toString());
                peekVar().setValue(System.in.read());
            } catch (IOException ex) {
            }
        }
        return 0;
    }

    /**
     * ��ֵ��������һ�������������ֵ����һ��������
     * @param dsc VarSymbol
     * @param src VarSymbol
     * @return int
     */
    public int AssignVar(VarSymbol dsc, VarSymbol src){
        System.out.println("AssignVar: dsc:" + dsc + ", src:" + src);
        if(dsc.type == TYPE_IDENTIFIER){
            dsc.setValue(src.integerValue());
            return 0;
        }
        return ERR_ASSIGN_NOTVAR;
    }

    /**
     * ����һ�����������������ջ��
     * @param var Token
     * @return int
     */
    public int LoadVar(Token var) {
        int err = 0;
        //ERR_VAR_NOTFOUND
        System.out.println("LoadVar Var:" + var.toString());
        if(var.type == TYPE_IDENTIFIER){
            VarSymbol _var = (VarSymbol)symbols.get(var.symbol);
            if(_var != null){
                dataStack.push(_var);
            }else {
                err = ERR_VAR_NOTFOUND;
            }
        }else if(var.type == TYPE_NUMBER){
            dataStack.push(new ConstVarSymbol(var));
        }else if(var.type == TYPE_NUMBER){
            dataStack.push(new ConstVarSymbol(var));
        }else {
             err= ERR_VAR_UNKNOWTYPE;
        }

        return err;
    }

    /**
     * ���ض�ջ���һ������������ɾ��������
     * @return VarSymbol
     */
    public VarSymbol peekVar() {
        return (dataStack.isEmpty()) ? null : dataStack.peek();
    }

    /**
     * ɾ����ջ��������������ر�����
     * @return VarSymbol
     */
    public VarSymbol popVar() {
        return (dataStack.isEmpty()) ? null : dataStack.pop();
    }

    public void clearVar(){
        dataStack.clear();
    }

    /**
     * ����ջ�������������㣬������ó����浽ջ����
     * �������У�
     * ">,>=,<,<=,==,!="
     * "+,-"
     * "*,/"
     * @param op Token
     * @return int
     */
    public int execute(Token op){
        VarSymbol var1=null,var2=null;
        VarSymbol result = new VarSymbol(dummy,dummy);
        var2 = popVar();
        var1 = popVar();
        int err = 0;
        System.out.println("execute OP:" + op.toString() + " var1:" + var1.toString() + ",var2:" + var2.toString());
        if (var2 != null && var1 != null) {
            if (op.symbol.length() == 1) {
                char opt = op.symbol.charAt(0);
                switch (opt) {
                case '+':
                    result.setValue(var1.integerValue() + var2.integerValue());
                    break;
                case '-':
                    result.setValue(var1.integerValue() - var2.integerValue());
                    break;
                case '*':
                    result.setValue(var1.integerValue() * var2.integerValue());
                    break;
                case '/':
                    result.setValue(var1.integerValue() / var2.integerValue());
                    break;
                case '>':
                    result.setValue(var1.integerValue() > var2.integerValue());
                    break;
                case '<':
                    result.setValue(var1.integerValue() < var2.integerValue());
                    break;
                }
            } else if (op.symbol.equals(">=")) {
                result.setValue(var1.integerValue() >= var2.integerValue());
            } else if (op.symbol.equals("<=")) {
                result.setValue(var1.integerValue() <= var2.integerValue());
            } else if (op.symbol.equals("==")) {
                result.setValue(var1.integerValue() == var2.integerValue());
            } else if (op.symbol.equals("!=")) {
                result.setValue(var1.integerValue() != var2.integerValue());
            }
            dataStack.push(result);
        }else {
            err = ERR_NO_OPT_DATA;
        }
        return err;
    }

    public String errorMessage(int code){
        String err = "";
        switch(code){
        case ERR_VAR_REDEFINE: err = "�ظ����������"; break;
        case ERR_VAR_NOTFOUND: err = "����û���ҵ���"; break;
        case ERR_VAR_UNKNOWTYPE: err = "δ֪�ı������ͣ�"; break;
        case ERR_ASSIGN_NOTVAR: err = "��ֵ��߲��Ǳ�����"; break;
        default: err = "δ֪��������,(" + code + ")";
        }
        return err;
    }
}
