package org.le.Lex.node;

import org.le.Lex.*;

public class Number extends Node {

    public boolean accept(Token t, int i, CharSequence seq) {
        char ch = seq.charAt(i);
        t.last = i;
        if (isDigit(ch)) {
            t.index = i + 1;
            next = this;
        } else if(isLetter(ch)){
            return false;
        }else {
            next = START;
        }
        return true;
    }
}
