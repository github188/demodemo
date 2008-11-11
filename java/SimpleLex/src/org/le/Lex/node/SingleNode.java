package org.le.Lex.node;

import org.le.Lex.*;

public class SingleNode extends Node {

    public boolean accept(Token t, int i, CharSequence seq) {
        char ch = seq.charAt(i);
        //t.index = i;
        t.last = i;
        next = START;   //单分界符不做任何处理，直接回到初始状态开始重新分析。
        return true;
    }
}
