package org.notebook.gui;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.Writer;
import java.util.Date;

import org.apache.commons.logging.impl.SimpleLog;

public class NoteSimpleLog extends SimpleLog{
	private static Writer writer = null;
	public NoteSimpleLog(String name) {
		super(name);
		if(writer == null){
			try {
				writer = new OutputStreamWriter(new FileOutputStream("application.log"));
			} catch (FileNotFoundException e) {
				e.printStackTrace();
			}
		}
	}

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	
    protected void log(int type, Object message, Throwable t) {
        // Use a string buffer for better performance
        StringBuffer buf = new StringBuffer();

        // Append date-time if so configured
        if(showDateTime) {
            buf.append(dateFormatter.format(new Date()));
            buf.append(" ");
        }

        // Append a readable representation of the log level
        switch(type) {
            case SimpleLog.LOG_LEVEL_TRACE: buf.append("*TRACE* "); break;
            case SimpleLog.LOG_LEVEL_DEBUG: buf.append("*DEBUG* "); break;
            case SimpleLog.LOG_LEVEL_INFO:  buf.append("*INFO* ");  break;
            case SimpleLog.LOG_LEVEL_WARN:  buf.append("*WARN* ");  break;
            case SimpleLog.LOG_LEVEL_ERROR: buf.append("*ERROR* "); break;
            case SimpleLog.LOG_LEVEL_FATAL: buf.append("*FATAL* "); break;
        }

        // Append the name of the log instance if so configured 
        buf.append(String.valueOf(logName)).append(" - ");

        // Append the message
        buf.append(String.valueOf(message));

        // Append stack trace if not null
        if(t != null) {
            buf.append(" <");
            buf.append(t.toString());
            buf.append(">");

            java.io.StringWriter sw= new java.io.StringWriter(1024);
            java.io.PrintWriter pw= new java.io.PrintWriter(sw);
            t.printStackTrace(pw);
            pw.close();
            buf.append(sw.toString());
        }

        // Print to the appropriate destination
        write(buf);

    }
	
    protected void write(StringBuffer buffer) {
    	if(writer != null){
    		try {
				writer.write(buffer.toString());
				writer.write("\n");
				writer.flush();
				//System.out.println(buffer);
			} catch (IOException e) {
				System.err.println("Error to write log:" + e.toString());
			}
    	}else {
    		System.err.println(buffer.toString());
    	}

    }    

}
