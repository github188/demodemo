package org.ast.rt.syms;

import org.ast.rt.Value;

public class AbstractValue extends Value {

    public AbstractValue(int type, String value) {
        super(type, value);
    }

    /**
     * booleanValue
     *
     * @return boolean
     * @todo Implement this org.ast.rt.Value method
     */
    public boolean booleanValue() {
        return false;
    }

    /**
     * integerValue
     *
     * @return int
     * @todo Implement this org.ast.rt.Value method
     */
    public int integerValue() {
        return 0;
    }

    /**
     * setValue
     *
     * @param v boolean
     * @todo Implement this org.ast.rt.Value method
     */
    public void setValue(boolean v) {
    }

    /**
     * setValue
     *
     * @param v String
     * @todo Implement this org.ast.rt.Value method
     */
    public void setValue(String v) {
    }

    /**
     * setValue
     *
     * @param v int
     * @todo Implement this org.ast.rt.Value method
     */
    public void setValue(int v) {
    }

    /**
     * setValue
     *
     * @param v Value
     * @todo Implement this org.ast.rt.Value method
     */
    public void setValue(Value v) {
    }

    /**
     * stringValue
     *
     * @return String
     * @todo Implement this org.ast.rt.Value method
     */
    public String stringValue() {
        return "";
    }

}
