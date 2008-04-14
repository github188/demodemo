package com.sigma.demo.util;

import java.io.IOException;

import org.snmp4j.CommunityTarget;
import org.snmp4j.PDU;
import org.snmp4j.Snmp;
import org.snmp4j.TransportMapping;
import org.snmp4j.event.ResponseEvent;
import org.snmp4j.mp.SnmpConstants;
import org.snmp4j.smi.Address;
import org.snmp4j.smi.GenericAddress;
import org.snmp4j.smi.OID;
import org.snmp4j.smi.OctetString;
import org.snmp4j.smi.VariableBinding;
import org.snmp4j.transport.DefaultUdpTransportMapping;

public class SnmpRequest {
    private Snmp snmp = null;
    
    private TransportMapping transport = null;
    
    private CommunityTarget target = null;
    
    private PDU pdu = null;
    
    private PDU responsePDU = null;
    
    private Address peerAddress = null;
    private int errorIndex = -1;
    private int errorStatus = -1;
    private String errorStatusText = null;
    private VariableBinding[] variableBindings = null;
    
    public SnmpRequest(String ipAddr, String community,int pduType, String oid) {     
    	 this(ipAddr,community,pduType, new OID[]{new OID(oid)});
    }

    /**
     * Constructor 
     * 
     * @param ipAddr  The agent address 
     * @param community  The community used to control the access
     * @param pduType  The PDU type : get, getnext, set, getrequest, trap
     * @param oids   The Object Identifier
     */
    public SnmpRequest(String ipAddr, String community,int pduType, OID[] oids) {     
        try {
            transport = new DefaultUdpTransportMapping();
            
            snmp = new Snmp(transport);
            transport.listen();
            
            createTarget(ipAddr, community);
            
            createPDU(pduType, oids);
        }
        catch(IOException e) {
            e.printStackTrace();
        }    
    }
    
    /**
     * Create the target of the agent
     *
     * @param ipAddr  The agent address
     * @param community   The community used to control the access
     */
    private void createTarget(String ipAddr, String community) {
        if (ipAddr == null || community == null) {
            System.err.println("The ipAddr and community should not be null");
            return ;
        }
        
        Address targetAddress = GenericAddress.parse("udp:"+ ipAddr.trim() + "/161");      
        target = new CommunityTarget();
        target.setCommunity(new OctetString(community));
        target.setAddress(targetAddress);
        target.setRetries(2);
        target.setTimeout(1500);
        target.setVersion(SnmpConstants.version1);
    }
    
    /**
     * Create a snmp pdu
     *
     * @param pduType  The PDU type : get, getnext, set, getrequest, trap
     * @param oids  The Object Identifier
     */
    private void createPDU(int pduType, OID[] oids) {
       VariableBinding[] vbs = createVariableBindings(oids);
       
       pdu = new PDU();
       
       for (VariableBinding vb : vbs) {
           pdu.add(vb);
       }
       pdu.setType(pduType);   
    }
    
    /**
     *
     * @param oids   The Object Identifier
     * @return The variableBindings build by the this oids
     * 
     */
    private VariableBinding[] createVariableBindings(OID[] oids) {
        if (oids == null) {
            System.out.println("The oids shoud not be null");
            return null;
        }
        VariableBinding[] vbs = new VariableBinding[oids.length]; 
       
        for (int i = 0; i < oids.length; i++) {
            vbs[i] = new VariableBinding(oids[i]);
        }
        
        return vbs;
    } 
          
    /**
     * Send a pdu to the agent
     */
    public void sendPDU() {
        ResponseEvent event = null;
          try {
              event = snmp.send(pdu, target);
              //snmp.
        }
        catch(IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        
        responsePDU = event.getResponse();
        
        if (responsePDU == null) {          
            clearStatement();
            return ;
        }
        
        setStatement(responsePDU);
    }
    
    private void clearStatement() {
        
        this.setErrorIndex(-1);
        this.setErrorStatus(-1);
        this.setErrorStatusText(null);
        this.setPeerAddress(null);
        this.setVariableBindings(null);
    }
    
    private void setStatement(PDU resPdu) {
        
        this.setErrorIndex(resPdu.getErrorIndex());
        this.setErrorStatus(resPdu.getErrorStatus());
        this.setErrorStatusText(resPdu.getErrorStatusText());    
        
        int size = resPdu.getVariableBindings().size();
        VariableBinding[] resVbs = new VariableBinding[size];
        for (int i = 0; i <size; i++) {
            resVbs[i] = (VariableBinding)resPdu.getVariableBindings().get(i);
        }
        
        
        this.setVariableBindings(resVbs);
    }
    
    public void printLastResponse() {
        System.out.println("************Start****************");
        if (peerAddress != null)
            System.out.print("     peer Address:" + peerAddress.toString());
        System.out.print("  errorStatus:" + getErrorStatus());
        System.out.print("  errorIndex:" + getErrorIndex());
        System.out.println("  errorStatus:" + getErrorStatusText());
        
        VariableBinding[] vbs;
        if ((vbs = getVarBindings()) != null) {
            for (VariableBinding vb : vbs) {
               printVariableBinding(vb);
            }
        }
        
        System.out.println("       -----End------");
    }
    
    public void printVariableBinding(VariableBinding vb) {
        if (vb == null) {
            System.err.println("The VariableBinding shoud not be null");
            return ;
        }
        
        System.out.print("    " + vb.getOid().toString());
        System.out.print("    " + vb.getSyntax());
        System.out.println("    " + vb.getVariable().toString());
    }
    
    /**
     *
     * @param args
     * 
     */
    public static void main(String[] args) {
        // TODO Auto-generated method stub
        OID[] oids = new OID[2];
        oids[0] = new OID("1.3.6.1.2.1.1.1");
        oids[1] = new OID("1.3.6.1.2.1.1.4.0");
        
        SnmpRequest request = new SnmpRequest("10.11.21.2", "public", PDU.GETNEXT, oids);
        request.sendPDU();
        request.printLastResponse();
    }
    
    

    public int getErrorIndex() {
        return errorIndex;
    }

    public void setErrorIndex(int errorIndex) {
        this.errorIndex = errorIndex;
    }

    public int getErrorStatus() {
        return errorStatus;
    }

    public void setErrorStatus(int errorStatus) {
        this.errorStatus = errorStatus;
    }

    public String getErrorStatusText() {
        return errorStatusText;
    }

    public void setErrorStatusText(String errorStatusText) {
        this.errorStatusText = errorStatusText;
    }

    public Address getPeerAddress() {
        return peerAddress;
    }

    public void setPeerAddress(Address peerAddress) {
        this.peerAddress = peerAddress;
    }

    public VariableBinding[] getVarBindings() {
        return variableBindings;  
    }
    
    public void setVariableBindings(VariableBinding[] vbs) {
        this.variableBindings = vbs;
    }

}

