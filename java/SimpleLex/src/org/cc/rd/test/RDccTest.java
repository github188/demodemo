package org.cc.rd.test;

import org.cc.rd.tree.Program;
import org.le.Lex.Lex;
import org.cc.rd.SyntaxException;

public class RDccTest {
    static String[] testCases = {"1a2",
                                "{int a;}",
                                "{a=10;}",
                                "{a=a+10;}",
                                "{a=a+a+c*1;}",
                                "{if(a==b){test;}}",
    };
    //static String[] testCases = {"{if(a==b){test=2;}}"};

    public static void main(String[] args) {
        int error = 0;
        Program program = new Program();
        for(String code : testCases){
            Lex lex = new Lex(code);
            try{
                System.out.println("���﷨������ʼ��������������");
                System.out.println("CODE:" + code);
                System.out.println("--�ʷ������б�---");
                error = program.parse(lex);
                System.out.println("--�﷨��������---");
                if (error != Program.RET_OK) {
                    System.out.println("ERROR:" + program.errorMessage());
                } else {
                    System.out.println("�����ɹ�!");
                    System.out.println("CODE:" + code);
                }
            }catch(SyntaxException e){
                e.printStackTrace();
            }
        }
    }
}
