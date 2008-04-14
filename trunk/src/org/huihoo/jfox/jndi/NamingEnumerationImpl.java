/* JFox, the OpenSource J2EE Application Server
 *
 * Copyright (C) 2002 huihoo.com
 * Distributable under GNU LGPL license
 * See the GNU Lesser General Public License for more details.
 */

package org.huihoo.jfox.jndi;

import javax.naming.NamingEnumeration;
import javax.naming.NamingException;
import java.util.Iterator;
import java.util.List;

/**
 *
 * @author <a href="mailto:young_yy@hotmail.com">Young Yang</a>
 */


public class NamingEnumerationImpl implements NamingEnumeration {
   Iterator name_class_pair_iterator;

   public NamingEnumerationImpl(List list) {
     name_class_pair_iterator = list.iterator();
   }

	public Object next() throws NamingException {
		try{
			return name_class_pair_iterator.next();
		}catch(Exception e){
			throw new NamingException(e.toString());
		}
	}

	public boolean hasMoreElements() {
//		try{
//			System.out.println("[NameEnumerationImpl] WARNING: Please use NameEnueration.hashMore() instead!");
			return name_class_pair_iterator.hasNext();
//		}catch(Exception e){
//			e.printStackTrace();
//		}
	}

	public boolean hasMore() throws NamingException {
		try{
			return name_class_pair_iterator.hasNext();
		}catch(Exception e){
			throw new NamingException(e.toString());
		}
	}

	public Object nextElement() {
//		try{
//			System.out.println("[NameEnumerationImpl] WARNING: Please use NameEnueration.next() instead!");
			return name_class_pair_iterator.next();
//		}catch(Exception e){
//			e.printStackTrace();
//		}
	}

	public void close() throws NamingException {
		try{
			name_class_pair_iterator = null;
		}catch(Exception e){
			throw new NamingException(e.toString());
		}
	}
}