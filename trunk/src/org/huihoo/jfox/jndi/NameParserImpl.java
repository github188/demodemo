/* JFox, the OpenSource J2EE Application Server
 *
 * Copyright (C) 2002 huihoo.com
 * Distributable under GNU LGPL license
 * See the GNU Lesser General Public License for more details.
 */

package org.huihoo.jfox.jndi;

import javax.naming.*;
import java.util.Properties;

/**
 *
 * @author <a href="mailto:young_yy@hotmail.com">Young Yang</a>
 */


class NameParserImpl implements NameParser,java.io.Serializable {
//  private static Properties syntax = PropertiesHolder.getNSProperties();
	private static Properties syntax = new Properties();
	static {
		syntax.setProperty("jndi.syntax.direction", "left_to_right");
		syntax.setProperty("jndi.syntax.ignorecase", "false");
		syntax.setProperty("jndi.syntax.separator", "/");
    syntax.setProperty("jndi.syntax.trimblanks","true");
	}

	private static NameParserImpl me = new NameParserImpl();

	private NameParserImpl() {

	}

	/**
	 * parse a name to javax.naming.Name
	 */

	public Name parse(String name) throws NamingException {
		return new CompoundName(name, syntax);
	}

	public static synchronized NameParserImpl getInstance() {
		return me;
	}

  public static void main(String[] args) throws Exception {
    System.out.println(me.parse("/").toString());
  }
}
