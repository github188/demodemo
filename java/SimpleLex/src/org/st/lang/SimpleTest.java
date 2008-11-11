package org.st.lang;

import org.cc.rd.tree.Program;
import org.le.Lex.Lex;
import org.cc.rd.SyntaxException;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import org.ast.rt.RunTime;

public class SimpleTest {
    public static void main(String[] args) throws FileNotFoundException,
            SyntaxException {
        int error = 0;
        if(args.length != 1){
            usage();
            return;
        }
        File file = new File(args[0]);
        if(file.isFile() && file.canRead()){
            System.out.println(file.getName() + " loading....");
            Program program = new Program();
            Lex lex = new Lex(new FileInputStream(file));
            program.setIsEvaluate(true);
            program.setRunTime(new RunTime());
            error = program.parse(lex);
            System.out.println(program.errorMessage());
        }else {
            System.out.println("Fail to load '" +file.getAbsolutePath() + "'");
        }
    }

    private static void usage(){
        System.out.println("SimpleTest filename");
    }
}
