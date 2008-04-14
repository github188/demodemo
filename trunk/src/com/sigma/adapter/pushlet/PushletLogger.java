package com.sigma.adapter.pushlet;
import org.apache.log4j.LogManager;
import org.apache.log4j.Logger;

import nl.justobjects.pushlet.util.DefaultLogger;

public class PushletLogger extends DefaultLogger {
	private static int LEVEL_FATAL = 1;
	private static int LEVEL_ERROR = 2;
	private static int LEVEL_WARN = 3;
	private static int LEVEL_INFO = 4;
	private static int LEVEL_DEBUG = 5;
	private static int LEVEL_TRACE = 6;

	/**
	 * Level intialized with default.
	 */
	private static int level = LEVEL_INFO;
	private Logger log = null;

	public PushletLogger() {
		log = LogManager.getLogger(DefaultLogger.class.getName());
	}

	/**
	 * Log message for trace level.
	 *
	 * @param aMessage the message to be logged
	 */
	public void trace(String aMessage) {
		if (level < LEVEL_TRACE) {
			return;
		}
		log.debug(aMessage);
	}

	/**
	 * Log message for debug level.
	 *
	 * @param aMessage the message to be logged
	 */
	public void debug(String aMessage) {
		if (level < LEVEL_DEBUG) {
			return;
		}
		log.debug(aMessage);		
	}

	/**
	 * Log message for info level.
	 *
	 * @param aMessage the message to be logged
	 */
	public void info(String aMessage) {
		if (level < LEVEL_INFO) {
			return;
		}
		log.info(aMessage);
	}

	/**
	 * Log message for warning level.
	 *
	 * @param aMessage the message to be logged
	 */
	public void warn(String aMessage) {
		if (level < LEVEL_WARN) {
			return;
		}
		log.warn(aMessage);
	}

	/**
	 * Log message for warning level with exception.
	 *
	 * @param aMessage   the message to be logged
	 * @param aThrowable the exception
	 */
	public void warn(String aMessage, Throwable aThrowable) {
		log.warn(aMessage, aThrowable);
	}

	/**
	 * Log message for error level.
	 *
	 * @param aMessage the message to be logged
	 */
	public void error(String aMessage) {
		if (level < LEVEL_ERROR) {
			return;
		}
		log.error(aMessage);
	}

	/**
	 * Log message (error level with exception).
	 *
	 * @param aMessage   the message to be logged
	 * @param aThrowable the exception
	 */
	public void error(String aMessage, Throwable aThrowable) {		
		log.error(aMessage, aThrowable);
	}

	/**
	 * Log message for fatal level.
	 *
	 * @param aMessage the message to be logged
	 */
	public void fatal(String aMessage) {
		if (level < LEVEL_FATAL) {
			return;
		}
		log.fatal(aMessage);
	}

	/**
	 * Log message (fatal level with exception).
	 *
	 * @param aMessage   the message to be logged
	 * @param aThrowable the exception
	 */
	public void fatal(String aMessage, Throwable aThrowable) {
		log.fatal(aMessage, aThrowable);
	}

	/**
	 * Set log level
	 *
	 * @param aLevel the message to be logged
	 */
	public void setLevel(int aLevel) {
		level = aLevel;
	}

}
