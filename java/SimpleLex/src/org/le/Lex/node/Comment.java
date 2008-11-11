package org.le.Lex.node;

import org.le.Lex.*;

public class Comment extends Node {
    private boolean inBlock = false;
    public boolean accept(Token t, int i, CharSequence seq) {
        char ch = seq.charAt(i);
        if(ch == '*' && !inBlock){  //注释块。
            t.index = i + 1;
            inBlock = true;  //开始处理注释块
            next = this;
        } else if( inBlock ){  //
            t.index = i + 1;
            if( ch == '*' &&
                seq.length() >= t.index &&
                seq.charAt(t.index) == '/'){  //注释尾。
                t.last = i + 2;
                t.index = i + 2;
                inBlock = false;
                next = START;
            }else {  //正在注释块中，
                next = this;
            }
        } else {  //输入单分界符。
            t.last = i;
            next = START;
        }

        return true;
    }

}
