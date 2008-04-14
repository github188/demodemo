/* JFox, the OpenSource J2EE Application Server
 *
 * Copyright (C) 2002 huihoo.com
 * Distributable under GNU LGPL license
 * See the GNU Lesser General Public License for more details.
 */

package org.huihoo.jfox.jndi;

//import java.net.URI;
//import java.net.URISyntaxException;

/**
 * 
 * @author <a href="mailto:young_yy@hotmail.com">Young Yang</a>
 */

public class URLUtil {
/*
   public static String getHost(URI uri){
    String host = uri.getHost();
    if(host.trim().length() ==0 ) host = JNDIProperties.DEFAULT_HOST;
    return host;
  }

  public static int getPort(URI uri){
    int port = uri.getPort();
    if(port < 0) port = JNDIProperties.DEFAULT_PORT;
    return port;
  }

  public static String getScheme(URI uri){
    String scheme = uri.getScheme();
    if(scheme.trim().length() == 0) scheme = JNDIProperties.DEFAULT_SCHEME;
    return scheme;
  }

  public static String getProviderURL (URI uri) {
    return getScheme(uri) + "://" + getHost(uri) + ":" + getPort(uri);
  }

  public static URI getCompleteURL(URI uri) {
    try {
      if(uri.getPath().trim().length() != 0){
        return new URI(getProviderURL(uri) + "/" + uri.getPath());
      }
      else {
        return new URI(getProviderURL(uri));
      }
    }
    catch(URISyntaxException e){
      e.printStackTrace();
      return uri;
    }
  }
*/
}
