package org.le.Lex.node;

import org.le.Lex.*;

public class Comment extends Node {
    private boolean inBlock = false;
    public boolean accept(Token t, int i, CharSequence seq) {
        char ch = seq.charAt(i);
        if(ch == '*' && !inBlock){  //ע�Ϳ顣
            t.index = i + 1;
            inBlock = true;  //��ʼ����ע�Ϳ�
            next = this;
        } else if( inBlock ){  //
            t.index = i + 1;
            if( ch == '*' &&
                seq.length() >= t.index &&
                seq.charAt(t.index) == '/'){  //ע��β��
                t.last = i + 2;
                t.index = i + 2;
                inBlock = false;
                next = START;
            }else {  //����ע�Ϳ��У�
                next = this;
            }
        } else {  //���뵥�ֽ����
            t.last = i;
            next = START;
        }

        return true;
    }

}
