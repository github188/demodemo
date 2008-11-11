package org.le.Lex.node;

import org.le.Lex.*;

public class DoubleNode extends Node {
    public boolean accept(Token t, int i, CharSequence seq) {
        char ch = seq.charAt(i);
        if (ch == '=') {    //双分界符。
            t.index = i + 1;
            t.last = i+1;
            next = START;
        } else { //单分界符时，回退一个字符。
            t.last = i;
            next = START;
        }
        return true;
    }
}
