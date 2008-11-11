package org.le.Lex.node;

import org.le.Lex.*;

public class SingleNode extends Node {

    public boolean accept(Token t, int i, CharSequence seq) {
        char ch = seq.charAt(i);
        //t.index = i;
        t.last = i;
        next = START;   //���ֽ�������κδ���ֱ�ӻص���ʼ״̬��ʼ���·�����
        return true;
    }
}
