package org.le.Lex;

import org.le.Lex.node.*;
import static org.le.Lex.Token.*;
import java.io.*;
/**
 * һ���򵥵Ĵʷ���������
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
     * ����Token�� �Ƚϻ��˵�token��index�ȵ�ǰ��indexС������ˡ���first���¿�ʼ����
     * ��������
     * @param t Token
     */
    public void pushToken(Token t){
        if(t != null   //&& t.first < token.index   �ڽ���for����ʱ�п�����ǰ����
        ){
            token.index = t.first;
            token.line = t.line;
            token.lineOffset = t.lineOffset;
            node = Node.START;
        }
        //this.pushed = t;
    }

    /**
     * �Ӵ����з�����һ���ʡ����﷨����������á�
     *
     * ʹ��һ�������Զ����������룬ÿһ�ִʷ�����һ���Զ�����һ��״̬����״̬�仯ʱ���ѽ���
     * ��һ���ʷ������һ���ʷ��������ֻص���ʼ״̬�����¿�ʼ����������롣
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
            //index�л��˵����������ֻͨ����1�����㡣
            token.column = token.index - token.lineOffset;
            if (node.accept(token, token.index, seq)) {
                //node.action(token,seq);
                pre = node;
                node = node.next();
                if (pre != node &&              //״̬�����仯��
                    token.last > token.first &&   //��ƥ������봮
                    token.type != TYPE_EMPTY       //���ǿմ�������Ҫ�����Ĵ����磺ע�ͣ�
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
        //�ѽ���������β��
        if (token.errorCode == OK && token.index >= seq.length()) {
            token.last = token.index;
        }

        if(token.last > token.first){
            token.symbol = seq.subSequence(token.first, token.last).toString();
            //���ŵĺ�̴�����������Ƿ��Ǳ����֡�
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
