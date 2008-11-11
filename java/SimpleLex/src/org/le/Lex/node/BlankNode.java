package org.le.Lex.node;

import org.le.Lex.*;

public class BlankNode extends Node {

    public boolean accept(Token t, int i, CharSequence seq) {
        char ch = seq.charAt(i);
        t.index = i + 1;
        if (isBlank(ch)) {
            next = this;
        } else {
            t.index = i;
            next = START;
        }
        return true;
    }


    public void action(String word, Token t, CharSequence seq) {
    };
}
