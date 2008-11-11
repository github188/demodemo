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
     * 赋值操作，将一个常量或变量赋值给另一个变量。
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
     * 加载一个变或常量到操作数堆栈。
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
     * 返回堆栈最顶的一个变量，但不删除变量。
     * @return VarSymbol
     */
    public VarSymbol peekVar() {
        return (dataStack.isEmpty()) ? null : dataStack.peek();
    }

    /**
     * 删除堆栈最最顶变量，并返回变量。
     * @return VarSymbol
     */
    public VarSymbol popVar() {
        return (dataStack.isEmpty()) ? null : dataStack.pop();
    }

    public void clearVar(){
        dataStack.clear();
    }

    /**
     * 弹出栈顶二个变量计算，将结果用常量存到栈顶。
     * 操作符有：
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
        case ERR_VAR_REDEFINE: err = "重复定义变量！"; break;
        case ERR_VAR_NOTFOUND: err = "变量没有找到！"; break;
        case ERR_VAR_UNKNOWTYPE: err = "未知的变量类型！"; break;
        case ERR_ASSIGN_NOTVAR: err = "赋值左边不是变量！"; break;
        default: err = "未知错误类型,(" + code + ")";
        }
        return err;
    }
}
