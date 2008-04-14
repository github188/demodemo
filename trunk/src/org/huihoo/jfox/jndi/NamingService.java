/* JFox, the OpenSource J2EE Application Server
 *
 * Copyright (C) 2002 huihoo.com
 * Distributable under GNU LGPL license
 * See the GNU Lesser General Public License for more details.
 */

package org.huihoo.jfox.jndi;

import javax.naming.Name;
import javax.naming.NamingException;
import javax.naming.Context;
import java.rmi.RemoteException;
import java.rmi.Remote;
import java.util.List;

/**
 *
 * @author <a href="mailto:young_yy@hotmail.com">Young Yang</a>
 */


public interface NamingService extends Remote{
	public void bind(Context ctx, Name name, Object obj, String className) throws NamingException, RemoteException;
	public void rebind(Context ctx, Name name, Object obj, String className) throws NamingException, RemoteException;
	public void unbind(Context ctx, Name name) throws NamingException, RemoteException;

	public Object lookup(Name name) throws NamingException, RemoteException;
	public Object lookupLink(Name name) throws NamingException, RemoteException;

	public List list(Context ctx, Name name) throws NamingException, RemoteException;
	public List listBindings(Context ctx, Name name) throws NamingException, RemoteException;

	public Context createSubcontext(Context ctx, Name name) throws NamingException, RemoteException;
	// destory empty sub context
	public void destroySubcontext(Context ctx, Name name) throws NamingException, RemoteException;
	// close context and destory all it's sub name and context
	public void closeSubcontext(Context ctx, Name name) throws NamingException, RemoteException;

  public boolean isBound(Name name) throws RemoteException;
}
