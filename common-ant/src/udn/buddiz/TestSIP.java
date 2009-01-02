package udn.buddiz;

import udn.buddiz.sip.SipEngine;

public class TestSIP {

	/**
	 * @param args
	 * @throws FatalBuddizException 
	 */
	public static void main(String[] args) throws Exception {
		SipEngine engine = SipEngine.getInstance();
		engine.setUsername("test");
		engine.startup(null);
		System.out.println("done");
	}

}
