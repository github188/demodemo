package org.task.queue.task;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

/**
 * 向task queue确认处理的结果.
 * @author deon
 *
 */
public class AckMessageTask implements Runnable {
	public transient static Log log = LogFactory.getLog("queue");


	@Override
	public void run() {
		// TODO Auto-generated method stub

	}

}
