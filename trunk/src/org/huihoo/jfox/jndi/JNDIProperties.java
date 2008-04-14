/* JFox, the OpenSource J2EE Application Server
 *
 * Copyright (C) 2002 huihoo.com
 * Distributable under GNU LGPL license
 * See the GNU Lesser General Public License for more details.
 */

package org.huihoo.jfox.jndi;

import java.util.Properties;
import java.util.Hashtable;
import javax.naming.Context;

/**
 * 
 * @author <a href="mailto:young_yy@hotmail.com">Young Yang</a>
 */

public class JNDIProperties {
  private static Properties DEFAULT_ENVIRONMENT = new Properties();

  public final static String DEFAULT_HOST = "localhost";
  public final static int DEFAULT_PORT = 1099;
  public final static String DEFAULT_SCHEME = "java";
  public final static String PROVIDER_URL = DEFAULT_SCHEME + "://" + DEFAULT_HOST + ":" + DEFAULT_PORT;
  public final static String URL_PKG_PREFIXES = "org.huihoo.jfox.jndi.url";
  public final static String INITIAL_CONTEXT_FACTORY = "org.huihoo.jfox.jndi.InitialContextFactoryImpl";

  static {
    DEFAULT_ENVIRONMENT.put(Context.INITIAL_CONTEXT_FACTORY, INITIAL_CONTEXT_FACTORY);
    DEFAULT_ENVIRONMENT.put(Context.PROVIDER_URL, PROVIDER_URL);
    DEFAULT_ENVIRONMENT.put(Context.URL_PKG_PREFIXES, URL_PKG_PREFIXES);
    DEFAULT_ENVIRONMENT.put(Context.OBJECT_FACTORIES, INITIAL_CONTEXT_FACTORY);
  };

  public static Hashtable getDefaultEnvironment(){
    return (Hashtable)DEFAULT_ENVIRONMENT.clone();
  }

  /**
   * merge env with the default environment 
   */
  public static Hashtable mergeEnvironment(Hashtable env){
    Hashtable _env = getDefaultEnvironment();
    _env.putAll(env);
    return _env;
  }

  public static Hashtable cloneEnvironment(Hashtable env){
    return (Hashtable)env.clone();
  }
}
