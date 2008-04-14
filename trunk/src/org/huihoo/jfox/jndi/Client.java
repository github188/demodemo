/* JFox, the OpenSource J2EE Application Server
 *
 * Copyright (C) 2002 huihoo.com
 * Distributable under GNU LGPL license
 * See the GNU Lesser General Public License for more details.
 */

package org.huihoo.jfox.jndi;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.Serializable;
import java.rmi.RemoteException;
import javax.naming.Context;
import javax.naming.InitialContext;
import javax.naming.Reference;
import javax.naming.StringRefAddr;
import javax.naming.NamingEnumeration;

/**
 *
 * @author <a href="mailto:young_yy@hotmail.com">Young Yang</a>
 */


public class Client {

	public static void main(String[] args) throws Exception{
    test();
 	}

	public static void referenceTest() throws Exception{
		System.setProperty("java.naming.factory.initial", JNDIProperties.INITIAL_CONTEXT_FACTORY);
		System.setProperty(Context.OBJECT_FACTORIES, JNDIProperties.INITIAL_CONTEXT_FACTORY);
		System.setProperty(Context.URL_PKG_PREFIXES, JNDIProperties.URL_PKG_PREFIXES);
		System.setProperty("java.naming.provider.url", JNDIProperties.PROVIDER_URL);

		Context ctx = new InitialContext();

		StringRefAddr addr = new StringRefAddr("URL", "file:/tmp");
		Reference fsRef = new Reference("java.io.BufferedInputStream", addr);
		ctx.bind("external", fsRef);
//		fsRef.getClassName()
		BufferedInputStream is = (BufferedInputStream)ctx.lookup("external");
		BufferedReader in = new BufferedReader(new InputStreamReader(is));
		System.out.println(in.readLine());
 	}


	public static void test() throws Exception{
		System.setProperty("java.naming.factory.initial", JNDIProperties.INITIAL_CONTEXT_FACTORY);
		System.setProperty("java.naming.factory.url.pkgs", "org.huihoo.jfox.jndi");
		System.setProperty("java.naming.provider.url", JNDIProperties.PROVIDER_URL);

    System.out.println("new InitialContext");
		Context ctx = new InitialContext();

    System.out.println("createSubcontext: yyy/zzz");
		ctx.createSubcontext("yyy/zzz");

    System.out.println("createSubcontext: yyy/zzz/aaa");
		ctx.createSubcontext("yyy/zzz/aaa");

    System.out.println("destroySubcontext: yyy/zzz/aaa");
    ctx.destroySubcontext("yyy/zzz/aaa");

    System.out.println("rebind: yyy/zzz/ccc");
    ctx.rebind("/yyy/zzz/ccc",new Test());

    System.out.println("lookup: yyy/zzz/ccc");
		Object ref = ctx.lookup("yyy/zzz/ccc");

    System.out.println("Invoke get method: " + ref);
//		Test tc = (Test)(ref);
    Test tc = (Test)javax.rmi.PortableRemoteObject.narrow(ref,Test.class);
		System.out.println(tc.get());

		listBindings(ctx, "yyy/zzz");

	}

  private static void listBindings(Context ctx, String name) throws Exception{
    System.out.println("ListBindings: " + name);
    NamingEnumeration enu = ctx.listBindings(name);
    while(enu.hasMore()){
      System.out.println(enu.next());
    }
  }
}

class Test implements Serializable{

/*
  public TestClass() throws RemoteException {
    PortableRemoteObject.exportObject(this);
  }
*/

	public String get() throws RemoteException {
		return "Hello,World!";
	}
}



