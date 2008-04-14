package org.apache.juli.logging;

import org.apache.log4j.LogManager;
import org.apache.log4j.Logger;
import org.apache.log4j.Priority;

public class DirectJDKLog implements Log{
	
	private Logger log = null;
	
	private DirectJDKLog(Logger log){
		this.log = log;
	}
	
	public static Log getInstance(String name){
		return new DirectJDKLog(LogManager.getLogger(name));
	}
	
	public static void release(){
		
	}

	public void debug(Object message) {
		log.debug(message);
	}

	public void debug(Object message, Throwable t) {
		log.debug(message, t);
	}

	public void error(Object message) {
		log.error(message);
	}

	public void error(Object message, Throwable t) {
		log.error(message, t);
	}

	public void fatal(Object message) {
		log.fatal(message);
	}

	public void fatal(Object message, Throwable t) {
		log.fatal(message, t);
	}

	public void info(Object message) {
		log.info(message);
	}

	public void info(Object message, Throwable t) {
		log.info(message, t);	
	}

	public boolean isDebugEnabled() {
		return log.isDebugEnabled();
	}

	public boolean isErrorEnabled() {
		return log.isEnabledFor(Priority.ERROR);
	}

	public boolean isFatalEnabled() {
		return log.isEnabledFor(Priority.FATAL);
	}

	public boolean isInfoEnabled() {
		return log.isInfoEnabled();
	}

	public boolean isTraceEnabled() {
		return log.isEnabledFor(Priority.DEBUG);
	}

	public boolean isWarnEnabled() {
		return log.isEnabledFor(Priority.WARN);
	}

	public void trace(Object message) {
		log.debug(message);
	}

	public void trace(Object message, Throwable t) {
		log.debug(message, t);
	}

	public void warn(Object message) {
		log.warn(message);
	}

	public void warn(Object message, Throwable t) {
		log.warn(message, t);
	}
}
