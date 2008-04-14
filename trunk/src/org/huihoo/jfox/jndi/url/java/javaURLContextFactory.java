/* JFox, the OpenSource J2EE Application Server
 *
 * Copyright (C) 2002 huihoo.com
 * Distributable under GNU LGPL license
 * See the GNU Lesser General Public License for more details.
 */

package org.huihoo.jfox.jndi.url.java;

import java.util.Hashtable;
//import java.net.URI;
import java.net.URL;

import javax.naming.Context;
import javax.naming.Name;
import javax.naming.spi.ObjectFactory;

import org.huihoo.jfox.jndi.ContextSupport;
//import org.huihoo.jfox.jndi.URLUtil;
import org.huihoo.jfox.jndi.JNDIProperties;
import org.huihoo.jfox.jndi.url.JndiURL;

/**
 * performed when use Reference with it's RefAddr is StringRefAddr,type is URL, content is "java://.."
 * 
 * @author <a href="mailto:young_yy@hotmail.com">Young Yang</a>
 */

public class javaURLContextFactory implements ObjectFactory {
   public Object getObjectInstance(Object obj,
                                Name name,
                                Context nameCtx,
                                Hashtable environment)
                         throws Exception {
     if(obj == null) obj = JNDIProperties.PROVIDER_URL;
     if(obj instanceof String){
       String url = (String)obj;
//       URI uri = new URI(url);
       URL _url = JndiURL.getURL(nameCtx,url);
       Hashtable env = (Hashtable)environment.clone();
//       env.put(Context.PROVIDER_URL, URLUtil.getProviderURL(uri));
       env.put(Context.PROVIDER_URL, _url.getProtocol() + "://" + _url.getHost() + ":" + _url.getPort());
       ContextSupport context = new ContextSupport("/",env);
       return context;
     }
     else {
       return null;
     }
   }

}
