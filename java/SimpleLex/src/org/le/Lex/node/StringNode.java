package org.le.Lex.node;

import org.le.Lex.Node;
import org.le.Lex.Token;

public class StringNode extends Node {
    public boolean accept(Token t, int i, CharSequence seq) {
        char ch = seq.charAt(i);
        t.index = i + 1;
        if(ch != '"' && ch != '\\' && ch != '\n'){  //ע�Ϳ顣
            next = this;
        }else if(ch == '\n'){
            return false;
        }else if(ch == '"'){
             t.last = i -1;
             next = START;
        }
        return true;
    }
}
