/* JFox, the OpenSource J2EE Application Server
 *
 * Copyright (C) 2002 huihoo.com
 * Distributable under GNU LGPL license
 * See the GNU Lesser General Public License for more details.
 */

package org.huihoo.jfox.jndi;


import java.rmi.RemoteException;
import java.rmi.MarshalledObject;
import java.io.IOException;
import java.io.Serializable;
import java.util.Map;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.Hashtable;

import javax.naming.Context;
import javax.naming.Name;
import javax.naming.NamingException;
import javax.naming.InvalidNameException;
import javax.naming.NameAlreadyBoundException;
import javax.naming.NameNotFoundException;
import javax.naming.NotContextException;
import javax.naming.Binding;
import javax.naming.NameClassPair;
import javax.naming.ContextNotEmptyException;

import org.huihoo.jfox.jndi.NamingService;

/**
 *
 * @author <a href="mailto:young_yy@hotmail.com">Young Yang</a>
 */

 // NOTE: all name was absolute name
//public class NamingServiceSupport extends PortableRemoteObject implements NamingService,Serializable {
//public class NamingServiceSupport extends UnicastRemoteObject implements NamingService,Serializable {
public class NamingServiceSupport implements NamingService, Serializable {
	/**
	 * the bindings hold all the naming
	 */
	private static Map bindings = Collections.synchronizedMap(new HashMap());

//	public NamingServiceSupport() throws RemoteException {
//	}

	public void bind(Context ctx, Name name, Object obj, String className) throws NamingException, RemoteException {
    if(name.isEmpty()){
			throw new InvalidNameException();
    }
		// doCreate needed subcontext
    if(name.size() > 1) {
      Name prefix = name.getPrefix(name.size() -1 );
      if(!prefix.toString().equals("/")) {
		    this.createSubcontext(ctx,name.getPrefix(name.size()-1));
      }
    }

		if(this.isBound(name)){
			throw new NameAlreadyBoundException(name.toString());
		}
		// so bind it
		setBinding(name,obj,className);
	}

	public void rebind(Context ctx,Name name, Object obj, String className) throws NamingException, RemoteException {
		if(name.isEmpty()){
			throw new InvalidNameException();
    }
		// doCreate needed subcontext
		this.createSubcontext(ctx,name.getPrefix(name.size()-1));

		// can't rebind a context
		if(this.isBound(name) && (this.getBinding(name).getObject() instanceof Context)){
      throw new NamingException("Cannot rebind a context!");
		}

		// so bind it
		setBinding(name,obj,className);
	}

	public void unbind(Context ctx,Name name) throws NamingException, RemoteException {
		if(!this.isBound(name)){ // not bound,throw Exception
      throw new NameNotFoundException("name: " + name.toString());
		}

		if(this.getBinding(name).getObject() instanceof Context){ // is a context
			this.destroySubcontext(ctx,name);
		}else{ // is a normal object
			removeBinding(name);
		}
	}

	public Object lookup(Name name) throws NamingException, RemoteException {
		if(this.isBound(name)){
			Object obj =  this.getBinding(name).getObject();
      return obj;
		}
    else{
			throw new NameNotFoundException(name.toString());
		}
	}

	public Object lookupLink(Name name) throws NamingException, RemoteException {
		return null;
	}

	public List list(Context ctx,Name name) throws NamingException, RemoteException {
		if(!this.isBound(name)){ // not bound
			throw new NameNotFoundException(name.toString());
		}
//		if(!(this.getBinding(name).getObject() instanceof Context)){ // not context
//			throw new NotContextException("name: " + name.toString());
//		}
    isContext(name);

		List list = new ArrayList();
		Iterator it = bindings.keySet().iterator();
		while(it.hasNext()){
			String key = (String)it.next();
			Name namekey = ctx.getNameParser(key).parse(key);
			if(namekey.startsWith(name) && (namekey.size() == name.size() +1)){
				Binding bind = this.getBinding(namekey);
				list.add(new NameClassPair(bind.getName(),bind.getClassName(),false));
			}
		}
		return list;
	}

	public List listBindings(Context ctx, Name name) throws NamingException, RemoteException {
		if(!this.isBound(name)){ // not bound
			throw new NameNotFoundException(name.toString());
		}
//		if(!(this.getBinding(name).getObject() instanceof Context)){ // not context
//			throw new NotContextException("name: " + name.toString());
//		}
    isContext(name);

		List list = new ArrayList();
		Iterator it = bindings.keySet().iterator();
		while(it.hasNext()){
			String key = (String)it.next();
			Name namekey = ctx.getNameParser(key).parse(key);
			if(namekey.startsWith(name) && (namekey.size() == name.size() +1)){
				Binding bind = this.getBinding(namekey);
				list.add(bind);
			}
		}
		return list;
	}

	public synchronized Context createSubcontext(Context ctx, Name name) throws NamingException, RemoteException {
		if(name.isEmpty() || name.toString().trim().equals("/")){
			throw new InvalidNameException(name.toString());
    }
		Context lastContext = null;
		for(int i=2; i<=name.size(); i++){ // while i = Configure.ROOT_NAME_LENGTH ,will doCreate the root context
			Name subName = name.getPrefix(i);
			if(!this.isBound(subName)){
				lastContext = createNextSubcontext(subName,ctx.getEnvironment()); // name not binding,doCreate a sub context
			}
		}
		return lastContext;
	}

	/**
	 * destory a empty sub context
	 */
	public void destroySubcontext(Context ctx, Name name) throws NamingException, RemoteException {
    if(!bindings.containsKey(name.toString())){
			throw new NameNotFoundException("name: " + name.toString());
    }

    isContext(name);

		List list = this.list(ctx,name);
		if(list.isEmpty()){ // context is empty
			removeBinding(name);
		}
    else{ // context not empty
			throw new ContextNotEmptyException("name: " + name.toString());
		}
	}

	public void closeSubcontext(Context ctx,Name name) throws NamingException, RemoteException {
    if(!bindings.containsKey(name.toString())){
      throw new NameNotFoundException("name: " + name.toString());
    }

    isContext(name);

//    Object binding  = this.getBinding(name).getObject();
//    if(!(binding instanceof MarshalledObject)){
//      throw new NotContextException("name: " + name.toString() + ", " + binding);
//    }
//    Object obj = binding;
//    try {
//      obj = ((MarshalledObject)binding).get();
//    }
//    catch(Exception e){
//      throw new NamingException(e.toString());
//    }
//
//    if(!(obj instanceof Context)) {
//      throw new NotContextException("name: " + name.toString() + ", " + binding);
//    }

    List list = this.list(ctx,name);
    if(!list.isEmpty()){ // context is not empty
      for(int i=0;i<list.size();i++){
        String sub_name = ((NameClassPair)list.get(i)).getName();
        if(sub_name.trim().equals("/")) continue;
        removeBinding(ctx.getNameParser(sub_name).parse(sub_name));
      }
		}

    removeBinding(name);
	}

   public boolean isBound(Name name) throws RemoteException {
     return bindings.containsKey(name.toString()) ? true : false;
   }

	/**
	 * get the Binding object that bind with the key
	 */
	private Binding getBinding(Name key) {
		String s_key = key.toString();
		Object obj = bindings.get(s_key);
		if (obj == null) return null;
		Binding b = (Binding)bindings.get(s_key);
		return b;
	}

	private void setBinding(Name name, Object obj, String className) {
		String n = name.toString();
		bindings.put(n, new Binding(n, className, obj, false));
	}

	/**
	 * doCreate a sub context with the name
	 * return the context
	 */
	private Context createNextSubcontext(Name name,Hashtable env) throws NamingException {
		ContextSupport ctx = new ContextSupport(name,env);
    Object obj = ctx;
    try {
      obj = new MarshalledObject(ctx);
    }
    catch(IOException e){
      throw new NamingException(e.toString());
    }
		setBinding(name,obj,ctx.getClass().getName());
		return ctx;
	}

   private void isContext(Name name) throws NamingException {
     Object binding  = this.getBinding(name).getObject();
     if(!(binding instanceof MarshalledObject)){
       throw new NotContextException("name: " + name.toString() + ", " + binding);
     }
     Object obj = binding;
     try {
       obj = ((MarshalledObject)binding).get();
     }
     catch(Exception e){
       throw new NamingException(e.toString());
     }

     if(!(obj instanceof Context)) {
       throw new NotContextException("name: " + name.toString() + ", " + binding);
     }

   }

	/**
	 * remove from bindings
	 */
  private void removeBinding(Name name)	{
    bindings.remove(name.toString());
  }


}
