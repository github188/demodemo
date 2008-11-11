package org.le.Lex;

public class Token implements Cloneable{
    public final static int OK = 0;
    public final static int NOT_FOUND_NEXT = 1;
    public final static int INVALID_WORDS = 2;

    public final static int TYPE_EMPTY = 0;
    public final static int TYPE_IDENTIFIER = 1;
    public final static int TYPE_NUMBER = 2;
    public final static int TYPE_SIGN = 3;
    public final static int TYPE_STRING = 4;
    public final static int TYPE_KEYWORDS = 5;

    public int first;
    public int last;
    public int index;
    public int errorCode;
    public String symbol = null;
    public int type;
    public int line;        //���������С�
    public int column;      //���������С�
    public int lineOffset;  //�п�ʼ�����Ӧλ�á�

    public String toString(){
        return symbol + "\t" + typeString() + "\t" + errorCode;
    }

    public String typeString(){
        String _tmp = null;
        switch(type){
        case TYPE_EMPTY:_tmp = "�����ͣ�";break;
        case TYPE_IDENTIFIER:_tmp = "��ʶ����";break;
        case TYPE_NUMBER:_tmp = "���֣�";break;
        case TYPE_SIGN:_tmp = "�������ţ�";break;
        case TYPE_STRING:_tmp = "�ַ�����";break;
        case TYPE_KEYWORDS:_tmp = "�ؼ��֣�";break;
        }
        return _tmp;
    }

    public boolean equals(Object o){
        if(o instanceof String || symbol != null){
            return symbol.equals(o);
        }else {
            return super.equals(o);
        }
    }

    public Token copy(){
        try {
            return (Token) clone();
        } catch (CloneNotSupportedException ex) {
            Token t = new Token();
            t.first = first;
            t.last = last;
            t.index = index;
            t.symbol = symbol;
            t.errorCode = errorCode;
            t.type = type;
            return t;
        }
    }
}
