package org.le.Lex;

import org.le.Lex.node.Start;

public class Node {
    public static final Node START = new Start();
    protected Node next = null;
    private static final String SINGLE = "+-*(){};,:\"";
    private static final String DOUBLE = "><=!";
    private static final String BLANK = "\n\r\t ";
    private static final char COMMENT = '/';

    public Node(){
        next = START;
    }

    public Node next(){
        return next;
    }
    public boolean accept(Token t,int i, CharSequence seq){
        return false;
    }

    public void action(String word, Token t, CharSequence seq) {
     //   System.out.println(word + "\t" + word);
    };


    public boolean isLetter(char ch){
        return ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z');
    }

    public boolean isDigit(char ch){
        return '0' <= ch && ch <= '9';
    }

    public boolean isCommentHeader(char ch){
        return ch == COMMENT;
    }

    public boolean isSingleWord(char ch){
        return SINGLE.indexOf(ch) >= 0;
    }

    public boolean isDoubleword(char ch){
        return DOUBLE.indexOf(ch) >= 0;
    }

    public boolean isBlank(char ch){
        return BLANK.indexOf(ch) >= 0;
    }

    public boolean isStringDelimiter(char ch){
        return ch == '"';
    }
}
