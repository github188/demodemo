package com.nsn.bugTracker;

import java.io.*;

public abstract class ProcessFilter {
	protected static final Logger log = new Logger();

	public static final int RETURN = 0;
	public static final int NEXT = -1;
	//public boolean accept(String line);
	private String keyword = null;
	private String[] character = null;
	private boolean any = false;

	public ProcessFilter(String keyword, String character) {
		this(keyword, new String[] { character });
	}

	public ProcessFilter(String keyword, String[] character) {
		this.keyword = keyword;
		this.character = character;
		if (keyword == null || keyword.equals("*")) {
			any = true;
		}
	}

	public int filter(String line, BufferedReader reader, PrintWriter out)
			throws IOException {
		if (any) {
			log.debug("Fliter any, msg:" + line);
			return handle(line, reader, out);
		} else if (line != null && line.indexOf(keyword) >= 0) {
			//log.
			for (String c : character) {
				if (line.indexOf(c) < 0) {
					log.debug("Find key(" + keyword
							+ "), but have not character words '" + c
							+ "' in line \"" + line + "\"");
					return NEXT;
				}
			}
			String msg = line.substring(line.indexOf(keyword)
					+ keyword.length());
			log.debug("Fliter '" + keyword + "', msg:" + msg);
			return handle(msg, reader, out);
		}
		return NEXT;
	}

	protected abstract int handle(String line, BufferedReader reader,
			PrintWriter out) throws IOException;
}
