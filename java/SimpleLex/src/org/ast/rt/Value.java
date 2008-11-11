package org.ast.rt;

public abstract class Value {
    public Value(int type,String value){
    }

    public abstract int integerValue();

    public abstract String stringValue();

    public abstract boolean booleanValue();

    public abstract void setValue(String v);
    public abstract void setValue(boolean v);
    public abstract void setValue(int v);
    public abstract void setValue(Value v);
}
