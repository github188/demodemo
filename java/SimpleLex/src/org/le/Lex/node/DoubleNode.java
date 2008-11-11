package org.le.Lex.node;

import org.le.Lex.*;

public class DoubleNode extends Node {
    public boolean accept(Token t, int i, CharSequence seq) {
        char ch = seq.charAt(i);
        if (ch == '=') {    //˫�ֽ����
            t.index = i + 1;
            t.last = i+1;
            next = START;
        } else { //���ֽ��ʱ������һ���ַ���
            t.last = i;
            next = START;
        }
        return true;
    }
}
