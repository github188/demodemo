/*
 * Copyright (c) 2006 Ubiquity Software Corporation plc. All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * - Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer. 
 *  
 * - Redistributions in binary form must reproduce the above copyright notice, 
 * this list of conditions and the following disclaimer in the documentation 
 * and/or other materials provided with the distribution. 
 * 
 * - The name of Ubiquity Software Corporation may not be used to endorse or 
 * promote products derived from this software without specific prior written 
 * permission.
 * 
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITH ALL FAULTS AND WITHOUT
 * ANY TECHNICAL SUPPORT OR WARRANTIES OF ANY KIND. YOU ASSUME ALL RISK OF USE
 * OF THIS SOFTWARE. TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE LAW,
 * UBIQUITY, ITS SUPPLIERS AND RESELLERS DISCLAIM ALL WARRANTIES WITH RESPECT TO
 * THE TO THE SOFTWARE WHETHER EXPRESS, IMPLIED, STATUTORY OR OTHERWISE,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * NONINFRINGEMENT, QUALITY OF PERFORMANCE AND FITNESS FOR A PARTICULAR PURPOSE.
 * TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE LAW, IN NO EVENT WILL UBIQUITY,
 * OR ITS SUPPLIERS OR RESELLERS BE LIABLE FOR ANY INDIRECT, SPECIAL, INCIDENTAL
 * OR CONSEQUENTIAL DAMAGES ARISING OUT OF OR RELATED TO THE USE OF OR INABILITY
 * TO USE THE SOFTWARE EVEN IF ADVISED OF THE POSSIBILITY THEREOF, AND
 * REGARDLESS OF THE LEGAL OR EQUITABLE THEORY (CONTRACT, TORT OR OTHERWISE)
 * UPON WHICH THE CLAIM IS BASED. TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE
 * LAW, IN NO EVENT WILL UBIQUITY, OR ITS SUPPLIERS OR RESELLERS BE LIABLE FOR
 * DAMAGES FOR LOSS OF GOODWILL, LOST DATA, WORK STOPPAGE, COMPUTER FAILURE OR
 * MALFUNCTION, OR ANY AND ALL OTHER COMMERCIAL DAMAGES OR LOSSES, EVEN IF
 * ADVISED OF THE POSSIBILITY THEREOF, AND REGARDLESS OF THE LEGAL OR EQUITABLE
 * THEORY (CONTRACT, TORT OR OTHERWISE) UPON WHICH THE CLAIM IS BASED.
 * UBIQUITY'S TOTAL LIABILITY FOR THE SOFTWARE WILL BE LIMITED TO THE LICENSE
 * FEES PAID BY YOU TO UBIQUITY FOR THE SOFTWARE.
 * 
 * Contact: Ubiquity Software Corporation plc, Building 3, Eastern Business
 * Park, Wern Fawr Lane, St Mellons, Cardiff CF3 5EA United Kingdom, Attention:
 * Technical Services and Operation Manager.
 */
package udn.buddiz.sip;

import java.io.IOException;
import java.io.InputStream;
import java.net.ConnectException;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;
import java.text.ParseException;
import java.util.ArrayList;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.Properties;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.ScheduledFuture;
import java.util.concurrent.TimeUnit;

import javax.sip.ClientTransaction;
import javax.sip.Dialog;
import javax.sip.DialogTerminatedEvent;
import javax.sip.IOExceptionEvent;
import javax.sip.InvalidArgumentException;
import javax.sip.ListeningPoint;
import javax.sip.RequestEvent;
import javax.sip.ResponseEvent;
import javax.sip.SipException;
import javax.sip.SipFactory;
import javax.sip.SipListener;
import javax.sip.SipProvider;
import javax.sip.SipStack;
import javax.sip.TimeoutEvent;
import javax.sip.TransactionTerminatedEvent;
import javax.sip.address.Address;
import javax.sip.address.AddressFactory;
import javax.sip.address.SipURI;
import javax.sip.address.URI;
import javax.sip.header.CSeqHeader;
import javax.sip.header.CallIdHeader;
import javax.sip.header.ContactHeader;
import javax.sip.header.ContentTypeHeader;
import javax.sip.header.EventHeader;
import javax.sip.header.ExpiresHeader;
import javax.sip.header.FromHeader;
import javax.sip.header.HeaderFactory;
import javax.sip.header.MaxForwardsHeader;
import javax.sip.header.SubscriptionStateHeader;
import javax.sip.header.ToHeader;
import javax.sip.header.ViaHeader;
import javax.sip.message.MessageFactory;
import javax.sip.message.Request;
import javax.sip.message.Response;
import javax.swing.JOptionPane;

import udn.buddiz.Buddiz;
import udn.buddiz.FatalBuddizException;
import udn.buddiz.events.BuddizEvent;
import udn.buddiz.events.BuddizEventListener;
import udn.buddiz.events.ErrorEvent;
import udn.buddiz.events.MessageEvent;
import udn.buddiz.events.NotifyEvent;
import udn.buddiz.events.RegisteredEvent;
import udn.buddiz.events.SubscribedEvent;
import udn.buddiz.events.UnregisteredEvent;
import udn.buddiz.events.UnsubscribedEvent;
import udn.buddiz.windows.Buddy;

public class SipEngine implements SipListener {

	/** Singleton instance of this class. */
	static private SipEngine instance;

	private SipStack sipStack;

	private SipProvider sipProvider;

	private MessageFactory messageFactory;

	private AddressFactory addressFactory;

	private HeaderFactory headerFactory;

	private int port = -1;

	ScheduledExecutorService timer = null;

	ScheduledFuture registrationTask = null;

	ScheduledFuture subscriptionTask = null;
	
	private EngineConfig config = null;

	private String username;
	
	private int expires = 0;
	private int subExpires = 0;
	
	private String registrar = null;


	private String registrationPayload;

	private BuddizEventListener listener;

	private boolean registered = false;

	private Hashtable listeners = new Hashtable();
	
	private ArrayList<Buddy> subscriptions = new ArrayList<Buddy>();

	private Hashtable<String, Dialog> dialogs = new Hashtable<String, Dialog>();
	
	/**
	 * Singleton's constructors must be private, only accessed using
	 * getInstance().
	 * @throws FatalBuddizException
	 */
	private SipEngine() throws FatalBuddizException {
		sipStack = null;

		SipFactory sipFactory = SipFactory.getInstance();
		sipFactory.setPathName("gov.nist");

		try {
			headerFactory = sipFactory.createHeaderFactory();
			addressFactory = sipFactory.createAddressFactory();
			messageFactory = sipFactory.createMessageFactory();

			// Create SipStack object
			InputStream is = Buddiz.class.getClassLoader().getResourceAsStream(
					"sipengine.properties");
			if (is == null)
				is = Buddiz.class.getResourceAsStream("sipengine.properties");
			if (is == null)
				throw new FatalBuddizException(
						"The file sipengine.properties isn't in the classpath.");

			config = new EngineConfig(is); 

			findAFreePort(config.getPort());
			if (port == -1) {
				throw new FatalBuddizException("No free ports from "
						+ config.getPort() + " nor next 9.");
			}
			String ipAddress = getIPAddress();
			
			config.setProperty("javax.sip.IP_ADDRESS", ipAddress);

			sipStack = sipFactory.createSipStack(config.properties());

			ListeningPoint lp = sipStack.createListeningPoint(ipAddress, port,
					"udp");
			//Default provider for sending stuff.
			sipProvider = sipStack.createSipProvider(lp);
			sipProvider.addSipListener(this);

			lp = sipStack.createListeningPoint(ipAddress, port, "tcp");
			SipProvider sipProvider2 = sipStack.createSipProvider(lp);
			sipProvider2.addSipListener(this);
			
			this.expires = config.getExpires();
			this.subExpires = config.getSubExpires();
			this.registrar = config.getRegistrar();			

		} catch (Throwable e) {
			e.printStackTrace();
			throw new FatalBuddizException("Can't start the SIP stack.", e);
		}
	}

	public void startup(BuddizEventListener newListener) {
		
		listener = newListener;
		if (config.getShouldRegister()) {
			scheduleRegistrations();
		}
		if (config.getShouldSubscribe()) {
			scheduleSubscriptions();
		}

	}

	private void scheduleRegistrations() {
		int delay = 0; 

		int period = (expires - 35);
		if (period < 0)
			period = expires; //less than 35 seconds of expiry.

		if ((registrationTask != null) && !registrationTask.isCancelled()) {
			//We're already registering; reschedule the task
			stopRegistrations();
			delay = period; //no need to re-register right away.
		}

		registrationTask = getTimer().scheduleAtFixedRate(new Runnable() {
			public void run() {
				try {
					sendRegisterMessage();
				} catch (Throwable e) {
					e.printStackTrace();
					stopRegistrations();
					sendEvent(new ErrorEvent(this, -1, e.getMessage()));
				}
			}
		}, delay, period, TimeUnit.SECONDS);
	}

	private void scheduleSubscriptions() {
		int period = (subExpires - 35);
		if (period < 0)
			period = subExpires; //less than 35 seconds of expiry.

		int delay = period; //Initial delay.

		if ((subscriptionTask != null) && !subscriptionTask.isCancelled()) {
			//We're already subscribing; reschedule the task
			stopSubscriptions();
		}

		subscriptionTask = getTimer().scheduleAtFixedRate(new Runnable() {
			public void run() {
				try {
					sendSubscribeMessages(expires);
				} catch (Throwable e) {
					e.printStackTrace();
					stopSubscriptions();
					sendEvent(new ErrorEvent(this, -1, e.getMessage()));
				}
			}
		}, delay, period, TimeUnit.SECONDS);
	}

	private void sendSubscribeMessages(int expires) throws ParseException,
			InvalidArgumentException, SipException, UnknownHostException {
		synchronized(subscriptions) {
			Iterator list = subscriptions.iterator();
			while (list.hasNext()) {
				Buddy buddy = (Buddy) list.next();
				sendSubscribeMessage(buddy.getSipAddress(), expires);
			}
		}
	}

	private void sendSubscribeMessage(String subscription, int newExpires) throws ParseException,
			InvalidArgumentException, SipException, UnknownHostException {

		Request subscribe;
		Address contactAddress = null;
		ExpiresHeader expiresHeader = null;
		EventHeader eventHeader = null;

		contactAddress = addressFactory.createAddress("sip:" + username + "@"
				+ getIPAddress() + ":" + getPort());
		ContactHeader contactHeader = headerFactory
			.createContactHeader(contactAddress);
		expiresHeader = headerFactory.createExpiresHeader(newExpires);
		eventHeader = headerFactory.createEventHeader("presence");
		
		Dialog dialog = dialogs.get(subscription);

		if(dialog != null) { //Re-subscribing
			subscribe = dialog.createRequest(Request.SUBSCRIBE);
			ClientTransaction subscribeTid = sipProvider.getNewClientTransaction(subscribe);

			subscribe.setHeader(contactHeader);
			subscribe.setHeader(expiresHeader);
			subscribe.setHeader(eventHeader);

			subscribeTid.sendRequest();
		}
		else { //NEW subscription...
			String address = "sip:" + username + "@" + registrar;
	
			URI requestURI = null;
			Address senderAddress = null;
			Address subscriptionAddress = null;
			FromHeader fromHeader = null;
			ToHeader toHeader = null;
			ViaHeader viaHeader = null;
			ArrayList viaHeaders = new ArrayList();
			CallIdHeader callIdHeader = null;
			CSeqHeader cSeqHeader = null;
	
			requestURI = addressFactory.createURI(subscription);
			senderAddress = addressFactory.createAddress(address);
			subscriptionAddress = addressFactory.createAddress(subscription);
			fromHeader = headerFactory.createFromHeader(senderAddress, null);
			fromHeader.setTag("1");
			toHeader = headerFactory.createToHeader(subscriptionAddress, null);
			callIdHeader = sipProvider.getNewCallId();
			viaHeader = headerFactory.createViaHeader(getIPAddress(), getPort(),
					"udp", null);
			viaHeaders.add(viaHeader);
			cSeqHeader = headerFactory.createCSeqHeader(1, Request.SUBSCRIBE);
			MaxForwardsHeader maxForwardsHeader = headerFactory
					.createMaxForwardsHeader(70);
	
			subscribe = messageFactory.createRequest(requestURI,
					Request.SUBSCRIBE, callIdHeader, cSeqHeader, fromHeader,
					toHeader, viaHeaders, maxForwardsHeader);
			subscribe.setHeader(contactHeader);
			subscribe.setHeader(expiresHeader);
			subscribe.setHeader(eventHeader);

			ClientTransaction trans = sipProvider.getNewClientTransaction(subscribe);

			dialog = trans.getDialog();

			dialogs.put(subscription, dialog);
			
			trans.sendRequest();
		}

	}

	private void sendRegisterMessage() throws ParseException,
			InvalidArgumentException, SipException, UnknownHostException {
		sendRegisterMessage(expires);
	}

	private void sendRegisterMessage(int newExpires) throws ParseException,
			InvalidArgumentException, SipException, UnknownHostException {

		String address = "sip:" + username + "@" + registrar;

		SipURI requestURI = null;
		Address senderAddress = null;
		Address contactAddress = null;
		FromHeader fromHeader = null;
		ToHeader toHeader = null;
		ViaHeader viaHeader = null;
		ArrayList viaHeaders = new ArrayList();
		CallIdHeader callIdHeader = null;
		CSeqHeader cSeqHeader = null;
		ExpiresHeader expiresHeader = null;
		
		requestURI = addressFactory.createSipURI(null, registrar);
		senderAddress = addressFactory.createAddress(address);
		fromHeader = headerFactory.createFromHeader(senderAddress, null);
		fromHeader.setTag("1");
		toHeader = headerFactory.createToHeader(senderAddress, null);
		callIdHeader = sipProvider.getNewCallId();
		viaHeader = headerFactory.createViaHeader(getIPAddress(), getPort(),
				"udp", "1");
		viaHeaders.add(viaHeader);
		cSeqHeader = headerFactory.createCSeqHeader(1, Request.REGISTER);
		MaxForwardsHeader maxForwardsHeader = headerFactory
				.createMaxForwardsHeader(70);
		expiresHeader = headerFactory.createExpiresHeader(newExpires);

		contactAddress = addressFactory.createAddress("sip:" + username + "@"
				+ getIPAddress() + ":" + getPort());
		ContactHeader contactHeader = headerFactory
				.createContactHeader(contactAddress);

		Request register = messageFactory.createRequest(requestURI,
				Request.REGISTER, callIdHeader, cSeqHeader, fromHeader,
				toHeader, viaHeaders, maxForwardsHeader);
		register.setHeader(contactHeader);

		register.setHeader(expiresHeader);

		if ( (newExpires != 0) && (registrationPayload!=null) ) {
			ContentTypeHeader contentTypeHeader = headerFactory
					.createContentTypeHeader("text", "plain");
			register.setContent(registrationPayload, contentTypeHeader);
		}

		sipProvider.sendRequest(register);
	}

	private ScheduledExecutorService getTimer() {
		if (timer == null)
			timer = Executors.newScheduledThreadPool(1);

		return timer;
	}

	private int getPort() {
		return port;
	}

	private void findAFreePort(int startPort) {
		for (int i = startPort; i < (startPort + 10); i++) {
			Socket s = null;
			try {
				s = new Socket(getIPAddress(), i);
			} catch (ConnectException e) {
				port = i;
				return;
			} catch (IOException e) {
				e.printStackTrace();
			} finally {
				if (s != null) {
					try {
						s.close();
					} catch (IOException e) {
						e.printStackTrace();
					}
				}
			}
		}
	}

	private String getIPAddress() throws UnknownHostException {
		return InetAddress.getLocalHost().getHostAddress();
	}

	public void shutdown() throws Exception {
		if (timer != null)
			timer.shutdownNow();

		if (sipStack == null) {
			return; // not yet created!
		}

		Iterator points = sipStack.getListeningPoints();
		if (points == null) {
			return; // No listening points
		}
		while (points.hasNext()) {
			ListeningPoint lp = (ListeningPoint) points.next();
			sipStack.deleteListeningPoint(lp);
		}
		sipStack.stop();
	}

	/**
	 * Accessor method for the Singleton.
	 * 
	 * @return The only instance of this class.
	 * @throws FatalBuddizException
	 */
	synchronized static public SipEngine getInstance() {
		if (instance == null) {
			try {
				instance = new SipEngine();
			} catch (FatalBuddizException e) {
				e.printStackTrace();
			}
		}

		return instance;
	}

	public void processRequest(RequestEvent event) {
		String method = event.getRequest().getMethod();
		Response response;
		try {
			response = messageFactory.createResponse(200, event.getRequest());
			ToHeader toHeader = (ToHeader) response.getHeader(ToHeader.NAME);
			toHeader.setTag("1");
			
			Address contactAddress = addressFactory.createAddress("sip:" + username + "@"
					+ getIPAddress() + ":" + getPort());
			ContactHeader contactHeader = headerFactory
					.createContactHeader(contactAddress);
			response.setHeader(contactHeader);

			sipProvider.sendResponse(response);
		} catch (Throwable e) {
			e.printStackTrace();
			return;
		}
		
		if(method.equals(Request.MESSAGE)) {
			String from = ((FromHeader)event.getRequest().getHeader("From")).getAddress().getURI().toString();
			Buddy buddy = new Buddy(from);
			byte [] raw = event.getRequest().getRawContent();
			String text = new String(raw);
			BuddizEventListener window = (BuddizEventListener) listeners.get(buddy.getSipAddress());
			MessageEvent message = new MessageEvent(this, from, text);
			if(window == null) { //no window to process this
				listener.processEvent(message);
				return;
			}
			window.processEvent(message);
		}
		
		if(method.equals(Request.NOTIFY)) {
			String subscription = ((FromHeader)event.getRequest().getHeader("From")).getAddress().getURI().toString();
			SubscriptionStateHeader state = (SubscriptionStateHeader)event.getRequest().getHeader("Subscription-State");
			if( (state==null) || 
				(state.getState().indexOf(SubscriptionStateHeader.TERMINATED) != -1) ) {
				//Subscription terminated!
				Dialog dialog = dialogs.get(subscription);
				dialog.delete();
				dialogs.remove(subscription);
				return;
			}
			//Active subscription:
			byte [] raw = event.getRequest().getRawContent();
            String status;
            if(raw==null) {
                status = null;
            }
            else {
    			status = new String(raw).trim();
            }
			NotifyEvent notify = new NotifyEvent(this, subscription, status);
			listener.processEvent(notify);
			return;
		}
	}

	public void processResponse(ResponseEvent event) {
		Response response = event.getResponse();
		CSeqHeader cSeqHeader = (CSeqHeader) response
				.getHeader(CSeqHeader.NAME);
		String method = cSeqHeader.getMethod();

		if (method.equals(Request.REGISTER)) {
			processRegisterResponse(response);
			return;
		}
		
		if (method.equals(Request.SUBSCRIBE)) {
			processSubscribeResponse(response);
			return;
		}

	}

	private void processSubscribeResponse(Response response) {
		int status = response.getStatusCode();
		
		ToHeader to = (ToHeader) response.getHeader(ToHeader.NAME);
		Buddy buddy = new Buddy(to.getAddress().toString());
		
		synchronized(subscriptions) {
			if (status == Response.OK) {
				ExpiresHeader expiresHeader = response.getExpires();
				if (expiresHeader != null) {
					int currentExpires = expiresHeader.getExpires();
					if (currentExpires == 0) //unregistered
					{
						sendEvent(new UnsubscribedEvent(this, buddy));
						subscriptions.remove(buddy);
						return;
					}
					if (currentExpires != expires) { //expiry changed, re-schedule registrations.
						expires = currentExpires;
						scheduleRegistrations();
					}
				}
				subscriptions.add(buddy);
				sendEvent(new SubscribedEvent(this, buddy));
				return;
			}
	
			//Error status!
			if (status >= Response.BAD_REQUEST) {
				System.out.println("Bad Request! " + status + ": "
						+ response.getReasonPhrase());
				subscriptions.remove(buddy);
				sendEvent(new ErrorEvent(this, status, response.getReasonPhrase()));
			}
		}
	}

	private void processRegisterResponse(Response response) {
		int status = response.getStatusCode();
		if (status == Response.OK) {
			ExpiresHeader expiresHeader = response.getExpires();
			if (expiresHeader != null) {
				int currentExpires = expiresHeader.getExpires();
				if (currentExpires == 0) //unregistered
				{
					registered = false;
					sendEvent(new UnregisteredEvent(this));
					return;
				}
				if (currentExpires != expires) { //expiry changed, re-schedule registrations.
					expires = currentExpires;
					scheduleRegistrations();
				}
			}
			registered = true;
			sendEvent(new RegisteredEvent(this, registrationPayload));
			return;
		}

		//Error status!
		if (status >= Response.BAD_REQUEST) {
			System.out.println("Bad Request! " + status + ": "
					+ response.getReasonPhrase());
			stopRegistrations();
			sendEvent(new ErrorEvent(this, status, response.getReasonPhrase()));
		}
	}

	private void sendEvent(BuddizEvent event) {
		if (listener != null)
			listener.processEvent(event);
	}

	public void processTimeout(TimeoutEvent event) {
		String method = event.getClientTransaction().getRequest().getMethod();
		if (method.equals(Request.REGISTER)) {
			System.out.println("Registration request timeout! ");
			stopRegistrations();
			sendEvent(new ErrorEvent(this, -1,
					"Registration timed out. Check if the server is running."));
		}

		if (method.equals(Request.SUBSCRIBE)) {
			System.out.println("Subscription request timeout! ");
			sendEvent(new ErrorEvent(this, -1,
					"Subscription timed out. Check if the server is running. ") );
		}
	}

	public void processIOException(IOExceptionEvent event) {
		System.out.println("IO Exception! ");
		stopSubscriptions();
		stopRegistrations();
		sendEvent(new ErrorEvent(this, -1, "I/O error!"));
	}

	public void processTransactionTerminated(TransactionTerminatedEvent event) {
	}

	public void processDialogTerminated(DialogTerminatedEvent event) {
	}

	private void stopRegistrations() {
		if(registrationTask==null) {
			return;
		}
		registrationTask.cancel(false);
		registrationTask = null;
		registered = false;
	}

	public void stopSubscriptions() {
		if(subscriptionTask==null) {
			return;
		}
		subscriptionTask.cancel(false);
		subscriptionTask = null;
	}

	/** 
	 * Getter for registrationPayload. 
	 * @return The current value of registrationPayload.
	 */
	public String getRegistrationPayload() {
		return registrationPayload;
	}

	/**
	 * The setter for registrationPayload.
	 * @param newRegistrationPayload The new value for registrationPayload.
	 */
	public void setRegistrationPayload(String newRegistrationPayload) {
		registrationPayload = newRegistrationPayload;
	}

	public void unsubscribe(Buddy buddy) {
		/*
		try {
			if( ! config.getShouldSubscribe() ) {
				return;
			}
			sendSubscribeMessage(buddy.getSipAddress(), 0);
		} catch (Throwable e) {
			sendEvent(new ErrorEvent(this, -1, "Error while unsubscribing " + buddy.getSipAddress() + ". "
					+ e.getMessage()));
			e.printStackTrace();
		}*/
	}

	public void subscribe(Buddy buddy) {
		try {
			if( ! config.getShouldSubscribe() ) {
				return;
			}
			sendSubscribeMessage(buddy.getSipAddress(), subExpires);
		} catch (Throwable e) {
			sendEvent(new ErrorEvent(this, -1, "Error while subscribing! " + buddy.getSipAddress() + ". "
					+ e.getMessage()));
			e.printStackTrace();
		}
	}

	public void unregister() {
		try {
			if( ! config.getShouldRegister() ) {
				return;
			}
			stopRegistrations();
			sendRegisterMessage(0);
		} catch (Throwable e) {
			sendEvent(new ErrorEvent(this, -1, "Error while unregistering! "
					+ e.getMessage()));
			e.printStackTrace();
		}
	}

	public void register() {
		try {
			if( !config.getShouldRegister() ) {
				return;
			}
			sendRegisterMessage();
			scheduleRegistrations(); //this will send the fisrst message
		} catch (Throwable e) {
			sendEvent(new ErrorEvent(this, -1, "Error while registering! "
					+ e.getMessage()));
			e.printStackTrace();
		}
	}

	public boolean isRegistered() {
		return registered;
	}

	public void sendMessage(Buddy buddy, String text, BuddizEventListener messageListener) throws ParseException, UnknownHostException, InvalidArgumentException, SipException {
		addMessageListener(buddy, messageListener);
		String address = "sip:" + username + "@" + registrar;

		URI requestURI = null;
		Address senderAddress = null;
		Address receiverAddress = null;
		Address contactAddress = null;
		FromHeader fromHeader = null;
		ToHeader toHeader = null;
		ViaHeader viaHeader = null;
		ArrayList viaHeaders = new ArrayList();
		CallIdHeader callIdHeader = null;
		CSeqHeader cSeqHeader = null;
		
		requestURI = addressFactory.createURI(buddy.getSipAddress());
		
		senderAddress = addressFactory.createAddress(address);
		
		receiverAddress = addressFactory.createAddress(buddy.getSipAddress());
		fromHeader = headerFactory.createFromHeader(senderAddress, null);
		fromHeader.setTag("1");
		toHeader = headerFactory.createToHeader(receiverAddress, null);
		callIdHeader = sipProvider.getNewCallId();
		viaHeader = headerFactory.createViaHeader(getIPAddress(), getPort(),
				"udp", "1");
		viaHeaders.add(viaHeader);
		cSeqHeader = headerFactory.createCSeqHeader(1, Request.MESSAGE);
		MaxForwardsHeader maxForwardsHeader = headerFactory
				.createMaxForwardsHeader(70);

		contactAddress = addressFactory.createAddress("sip:" + username + "@"
				+ getIPAddress() + ":" + getPort());
		ContactHeader contactHeader = headerFactory
				.createContactHeader(contactAddress);

		Request message = messageFactory.createRequest(requestURI,
				Request.MESSAGE, callIdHeader, cSeqHeader, fromHeader,
				toHeader, viaHeaders, maxForwardsHeader);
		message.setHeader(contactHeader);

		ContentTypeHeader contentTypeHeader = headerFactory
				.createContentTypeHeader("text", "plain");
		message.setContent(text, contentTypeHeader);

		sipProvider.sendRequest(message);
	}

	public void addMessageListener(Buddy buddy, BuddizEventListener messageListener) {
		listeners.remove(buddy.getSipAddress());
		listeners.put(buddy.getSipAddress(), messageListener);
	}

	public void removeMessageListener(Buddy buddy) {
		listeners.remove(buddy.getSipAddress());
	}

	public void setUsername(String username) {
		this.username = username;
	}

}