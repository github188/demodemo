package org.le.Lex.test;

import org.le.Lex.*;
import static org.le.Lex.Token.*;

public class TestLex {
    static String code = "/*int*/ a;" +
                  "if(a1) == 2 >=ac3> if a";
    public static void main(String[] args) {
        Lex lex = new Lex(code);
        //lex.parse(code);
        Token t = null;
        do{
            t = lex.nextToken();
            System.out.println(t);
        }while(t != null && t.errorCode == OK);
    }
}
