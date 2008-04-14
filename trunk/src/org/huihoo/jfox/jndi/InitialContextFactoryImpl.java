/* JFox, the OpenSource J2EE Application Server
 *
 * Copyright (C) 2002 huihoo.com
 * Distributable under GNU LGPL license
 * See the GNU Lesser General Public License for more details.
 */

package org.huihoo.jfox.jndi;

import javax.naming.Context;
import javax.naming.NamingException;
import javax.naming.Name;
import javax.naming.Reference;
import java.util.Hashtable;
import java.net.URL;

/**
 *
 * @author <a href="mailto:young_yy@hotmail.com">Young Yang</a>
 */


public class InitialContextFactoryImpl implements javax.naming.spi.InitialContextFactory,javax.naming.spi.ObjectFactory{

	public Context getInitialContext(Hashtable env) throws NamingException {
    if(env == null) {
      env = JNDIProperties.getDefaultEnvironment();
    }
    else {
      env = JNDIProperties.mergeEnvironment(env);
    }
    return new ContextSupport("/",env);
  }

	// will perform after urlContextFactory failed and if set Context.OBJECT_FACTORY
	public Object getObjectInstance(Object obj, Name name, Context ctx, Hashtable environment) throws Exception {
//		Context ctx = getInitialContext(environment);
		Reference ref = (Reference)obj;
		String url_string = (String)ref.get("URL").getContent();
		URL url = new URL(url_string);
		return url.getContent();
  }

	public static void main(String[] args) {

	}
}
