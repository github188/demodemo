package com.sigma.demo.acl.security.auth;

import java.util.Map;

import javax.security.auth.Subject;
import javax.security.auth.callback.Callback;
import javax.security.auth.callback.CallbackHandler;
import javax.security.auth.callback.NameCallback;
import javax.security.auth.callback.PasswordCallback;
import javax.security.auth.callback.UnsupportedCallbackException;
import javax.security.auth.login.FailedLoginException;
import javax.security.auth.login.LoginException;
import javax.security.auth.spi.LoginModule;

public class DemoLoginModule implements LoginModule {
	
	private Subject subject;
	private CallbackHandler callbackHandler;
	private Map sharedState;
	private Map options;
	private boolean debug = false;
	private boolean succeeded = false;
	private boolean commitSucceeded = false;
	private String username;
	private char[] password;

	public boolean abort() throws LoginException {
		
		if (succeeded == false)
			return false;
						
		if(succeeded == true && commitSucceeded == false) {
			// login succeeded but overall authentication failed
			succeeded = false;
			username = null;
			password = null;
		} else {
			// overall authentication succeeded and commit 
			// succeeded, but someone else's commit failed.
			logout();
		}
		
		return true;
		
	}

	public boolean commit() throws LoginException {

		if(succeeded == false)
			return false;

		subject.getPrincipals().add(new UserPrincipal(username));
		subject.getPrincipals().add(new RolePrincipal("user"));
		
		username = null;
		password = null;

		commitSucceeded = true;
		return true;
	}

	public void initialize(Subject subject, CallbackHandler callbackHandler,
			Map<String, ?> sharedState, Map<String, ?> options) {
		this.subject = subject;
		this.callbackHandler = callbackHandler;
		this.sharedState = sharedState;
		this.options = options;

	}

	public boolean login() throws LoginException {
		
		if (callbackHandler == null)
			throw new LoginException("Error: CallbackHandler cannot be null");

		Callback[] callbacks = new Callback[2];
		callbacks[0] = new NameCallback("User Name: ");
		callbacks[1] = new PasswordCallback("Password: ", false);

		try {
			callbackHandler.handle(callbacks);
			username = ((NameCallback) callbacks[0]).getName();
			char[] tmpPassword = ((PasswordCallback) callbacks[1]).getPassword();
			
			if (tmpPassword == null) {
				tmpPassword = new char[0];
			}
			password = new char[tmpPassword.length];
			System.arraycopy(tmpPassword, 0, password, 0, tmpPassword.length);
			((PasswordCallback) callbacks[1]).clearPassword();

		} catch (java.io.IOException e) {
			throw new LoginException(e.getMessage());
		} catch (UnsupportedCallbackException e) {
			throw new LoginException("Error: " + e.getMessage());
		}

		if(debug) {
			System.out.println("LoginModuleExample: userid = " + username);
			String pwd = new String(password);
			System.out.println("LoginModuleExample: password = " + pwd);
		}

		// Check the userid and password
		if (isValidUser(username, password)) {
			// authentication succeeded
			if(debug)
				System.out.println("LoginModuleExample: authentication succeeded");
			succeeded = true;
			return true;
		} else {
			// authentication failed
			if(debug)
				System.out.println("LoginModuleExample: authentication failed");
			succeeded = false;
			// clear the values
			username = null;
			password = null;
			throw new FailedLoginException("Invalid userid or password");
		}
		
	}

	public boolean logout() throws LoginException {
		
		subject.getPrincipals().clear();
		succeeded = false;
		succeeded = commitSucceeded;
		username = null;
		password = null;
		
		return true;
	}
	
	private boolean isValidUser(String u, char[] p){
		
		if(u.equals("test")){
			return true;
		}
		
		return true;
	}

}
