package com.sigma.demo.test;

//import org.apache.log4j.LogManager;
//import org.apache.log4j.Logger;
import org.snmp4j.PDU;
import org.snmp4j.smi.VariableBinding;

//import com.sigma.demo.util.DemoSeriesProducer;
import com.sigma.demo.util.SnmpRequest;

public class SnmpTest {
	//private static final Logger log = LogManager.getLogger(SnmpTest.class);
	/**
	 * @param args
	 */
	public static void main(String[] args) { 
		// TODO Auto-generated method stub
		try{
			SnmpRequest snmp = null;
			String ip = args[0];
			String pass = args[1];
			String oid = args[2];
			System.out.println("Init Snmp IP:"+ip+", Community:"+pass+", OID:"+oid);
			// PDU.GET
			
			snmp = new SnmpRequest(ip, pass, PDU.GET, oid);
			snmp.sendPDU();
			if(snmp.getErrorIndex() != 0){
				System.out.println("Init Snmp Error Index:" +snmp.getErrorStatus()+ ", Error Text:" + snmp.getErrorStatusText());
				System.out.println("A random number will instead of the snmp data.(only in debug mode)");
				snmp = null;
			}else {
				VariableBinding[] result = snmp.getVarBindings();
				System.out.println("snmp result..." + result[0].getVariable().toLong());
			}
			
		}catch(Throwable e){
			e.printStackTrace();
		}		
	}

}
