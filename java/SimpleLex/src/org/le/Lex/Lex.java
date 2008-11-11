package org.le.Lex;

import org.le.Lex.node.*;
import static org.le.Lex.Token.*;
import java.io.*;
/**
 * 一个简单的词法分析程序。
 *
 * @author not attributable
 * @version 1.0
 */
public class Lex {
    private Token token = new Token();
    private Node node = Node.START;
    private Node pre = null;
    private CharSequence seq = null;

    public Lex(StringNode program){
        seq = program;
        initToken();
    }

    public Lex(InputStream stream){
        this(new InputStreamReader(stream));
    }

    public Lex(Reader reader){
        StringBuffer buffer = new StringBuffer(1024 * 8);
        try {
            char[] _buf = new char[1024];
            for(int len = _buf.length; len == _buf.length; ){
                len = reader.read(_buf,0, _buf.length);
                buffer.append(_buf,0,len);
            }
        } catch (IOException ex) {
            ex.printStackTrace();
        }
        seq = buffer;
        initToken();
    }

    public Token peek(){
        return token;
    }

    /**
     * 回退Token， 比较回退的token的index比当前的index小，则回退。到first重新开始做词
     * 法分析。
     * @param t Token
     */
    public void pushToken(Token t){
        if(t != null   //&& t.first < token.index   在解析for句语时有可能向前跳。
        ){
            token.index = t.first;
            token.line = t.line;
            token.lineOffset = t.lineOffset;
            node = Node.START;
        }
        //this.pushed = t;
    }

    /**
     * 从代码中分析出一个词。由语法分析程序调用。
     *
     * 使用一个有穷自动机分析代码，每一种词法都是一个自动机的一种状态。在状态变化时，已接受
     * 完一个词法。完成一个词法分析后又回到开始状态。重新开始分析输入代码。
     *
     * @return Token
     */
    public Token nextToken() {
        //token.errorCode =
        token.errorCode = OK;
        token.symbol = null;
        token.type = TYPE_EMPTY;
        token.first = 0;
        token.last = 0;
        if(token.index >= seq.length()){
            return null;
        }

        while (token.index < seq.length()) {
            if(seq.charAt(token.index) == '\n'){
                token.line++;
                token.lineOffset = token.index;
            }
            //index有回退的情况，不能只通过加1来计算。
            token.column = token.index - token.lineOffset;
            if (node.accept(token, token.index, seq)) {
                //node.action(token,seq);
                pre = node;
                node = node.next();
                if (pre != node &&              //状态发生变化。
                    token.last > token.first &&   //有匹配的输入串
                    token.type != TYPE_EMPTY       //不是空串（不需要解析的串，如：注释）
                    ) {
                    break;
                }
                if (node == null) {
                    token.errorCode = NOT_FOUND_NEXT;
                    break;
                }
            } else {
                token.errorCode = INVALID_WORDS;
                break;
            }
        }
        //已解析到程序尾。
        if (token.errorCode == OK && token.index >= seq.length()) {
            token.last = token.index;
        }

        if(token.last > token.first){
            token.symbol = seq.subSequence(token.first, token.last).toString();
            //符号的后继处理。例如分析是否是保留字。
            if(pre != null){
                pre.action(token.symbol, token, seq);
            }
        }

        return token;
    }


    private void initToken(){
        token.first = 0;
        token.last = 0;
        token.index = 0;
        token.line = 0;
        token.lineOffset = 0;
    }

}
