package org.le.Lex.node;

import org.le.Lex.Node;
import org.le.Lex.Token;

public class Identifier extends Node {
    public String[] keywords = {"for","int", "if", "else"};

    public boolean accept(Token t, int i, CharSequence seq) {
        char ch = seq.charAt(i);
        t.last = i;
        if (isLetter(ch) || isDigit(ch)) {
            t.index = i + 1;
            next = this;
        } else {
            next = START;
        }
        return true;
    }

    public void action(String word, Token t, CharSequence seq) {
        //   System.out.println(word + "\t" + word);
        for(String w : keywords){
            if(w.equals(word)){
                t.type = Token.TYPE_KEYWORDS;
            }
        }
    };

}
