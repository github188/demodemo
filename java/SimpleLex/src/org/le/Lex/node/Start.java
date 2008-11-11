package org.le.Lex.node;

import org.le.Lex.Node;
import org.le.Lex.Token;
import static org.le.Lex.Token.*;

public class Start extends Node {
    private Node id = new Identifier();
    private Node number = new Number();
    private Node single = new SingleNode();
    private Node doubleword = new DoubleNode();
    private Node comment = new Comment();
    private Node blank = new BlankNode();
    private Node string = new StringNode();

    public Start(){
    }

    public boolean accept(Token t, int i, CharSequence seq) {
        char ch = seq.charAt(i);
        t.first = i;
        t.index = i + 1;
        if(isLetter(ch)){
            next = id;
            t.type = TYPE_IDENTIFIER;
        }else if(isDigit(ch)){
            next = number;
            t.type = TYPE_NUMBER;
        }else if(isCommentHeader(ch)){
            next = comment;
        }else if(isSingleWord(ch)){
            next = single;
            t.type = TYPE_SIGN;
        }else if(isDoubleword(ch)){
            next = doubleword;
            t.type = TYPE_SIGN;
        }else if(isBlank(ch)){
            next = blank;
        }else if(isStringDelimiter(ch)){
            next = string;
            t.type = TYPE_STRING;
        }

        //accepted, if have a differnet next node.
        return next != this;
    }

    public void action(StringNode word, Token t, CharSequence seq) {
    };




}
