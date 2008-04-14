/* JFox, the OpenSource J2EE Application Server
 *
 * Copyright (C) 2002 huihoo.com
 * Distributable under GNU LGPL license
 * See the GNU Lesser General Public License for more details.
 */

package org.huihoo.jfox.jndi;

import java.net.Socket;
import java.io.ObjectInputStream;
import java.io.BufferedInputStream;
import java.util.Map;
import java.util.HashMap;
import java.rmi.MarshalledObject;
import javax.naming.CommunicationException;
//import javax.rmi.PortableRemoteObject;
//import javax.rmi.CORBA.Stub;

//import org.omg.CORBA.ORB;

/**
 * cache and get NamingService
 *
 * @author <a href="mailto:young_yy@hotmail.com">Young Yang</a>
 */

//class ServiceLocator {
public class ServiceLocator {
  private static Map services = new HashMap();
//  private static ORB orb = ORB.init(new String[0],null);

  /**
   * get NamingService Remote Reference from the host:port
   */
  public static NamingService getNamingService(String host, int port)
         throws CommunicationException {
    if(services.containsKey(genKey(host,port))) { // return from cache
      try {
//        return (NamingService)PortableRemoteObject.narrow(((MarshalledObject)services.get(genKey(host,port))).get(),NamingService.class);
    	  if(services.get(genKey(host,port)) instanceof MarshalledObject){
        return (NamingService)(((MarshalledObject)services.get(genKey(host,port))).get());
    	  }else {
    		  return (NamingService)services.get(genKey(host,port));
    	  }
      }
      catch(Exception e){
        services.remove(genKey(host,port));
        e.printStackTrace();
        throw new CommunicationException(e.getMessage());
      }
    }
    else {
      try {
        Socket csocket = new Socket(host,port);
        ObjectInputStream in = new ObjectInputStream(new BufferedInputStream(csocket.getInputStream()));
        Object obj = in.readObject();
        csocket.close();
        MarshalledObject mobj = (MarshalledObject)obj;
        NamingService stub = (NamingService)mobj.get();
//        NamingService stub = (NamingService)PortableRemoteObject.narrow(mobj.get(),NamingService.class);
//        ((Stub)stub).connect(orb);
        services.put(genKey(host,port), mobj);
        return stub;
      }
      catch(Exception e){
        e.printStackTrace();
        services.remove(genKey(host,port));
        throw new CommunicationException(e.getMessage());
      }
    }
  }

  private static String genKey(String host, int port){
    return host + ":" + port;
  }
  
  /**
   * bind NamingService, 
   * @param host
   * @param port
   * @param service
   * </code></pre>
   */
  public static void bindNamingService(String host, int port, NamingService service) {
	  services.put(genKey(host,port), service);
  }

}
