/* JFox, the OpenSource J2EE Application Server
 *
 * Copyright (C) 2002 huihoo.com
 * Distributable under GNU LGPL license
 * See the GNU Lesser General Public License for more details.
 */

package org.huihoo.jfox.jndi.url;

import java.net.URL;
import java.net.URLStreamHandler;
import java.net.URLConnection;
import java.net.URLStreamHandlerFactory;
import java.net.MalformedURLException;
import java.io.IOException;

import org.huihoo.jfox.jndi.JNDIProperties;
import javax.naming.Context;
import javax.naming.NamingException;

/**
 * 
 * @author <a href="mailto:young_yy@hotmail.com">Young Yang</a>
 */

public class JndiURL {

  static {
      URL.setURLStreamHandlerFactory(new URLStreamHandlerFactory() {
      public URLStreamHandler createURLStreamHandler(String protocol) {
        return !"java".equals(protocol) ? null : new URLStreamHandler(){
          protected URLConnection openConnection(URL u) throws IOException {
            throw new UnsupportedOperationException("openConnection");
          }
        };
      }
    });
  }
	
  public static URL getURL(Context ctx, String url) throws MalformedURLException{
    url = url.trim();
    if(url.endsWith("/")) { // ȥ������ /
      url = url.substring(0,url.length()-1);
    }

    if(url.indexOf(':') > 0) { // ����ľ��·�� java://host:port
      URL temp = new URL(url);
      if(temp.getPort() == -1) { // û������ Port
        return new URL(temp.getProtocol(),temp.getHost(),JNDIProperties.DEFAULT_PORT,temp.getPath());
      }
    }
    else {
      if(url.startsWith("/")){
        return new URL(JNDIProperties.DEFAULT_SCHEME,JNDIProperties.DEFAULT_HOST,JNDIProperties.DEFAULT_PORT,url);
      }
      else {
        try {
          String nameSpace = ctx.getNameInNamespace();

          String absUrl = nameSpace.endsWith("/") ? nameSpace + url : nameSpace + "/" + url;
          return new URL(JNDIProperties.DEFAULT_SCHEME,JNDIProperties.DEFAULT_HOST,JNDIProperties.DEFAULT_PORT,absUrl);
          
        }
        catch(NamingException e){

        }
      }
    }
    return new URL(url);
  }

  public static void main(String[] args) {

  }

}
