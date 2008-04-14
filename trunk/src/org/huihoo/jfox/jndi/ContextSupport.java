/* JFox, the OpenSource J2EE Application Server
*
* Copyright (C) 2002 huihoo.com
* Distributable under GNU LGPL license
* See the GNU Lesser General Public License for more details.
*/

package org.huihoo.jfox.jndi;

import java.io.IOException;
import java.io.Serializable;
import java.net.MalformedURLException;
import java.net.URL;
import java.rmi.MarshalledObject;
import java.rmi.Remote;
import java.rmi.RemoteException;
import java.rmi.server.RemoteStub;
import java.util.Hashtable;
import java.util.List;

import javax.naming.Binding;
import javax.naming.CompoundName;
import javax.naming.Context;
import javax.naming.ContextNotEmptyException;
import javax.naming.InitialContext;
import javax.naming.InvalidNameException;
import javax.naming.LinkRef;
import javax.naming.Name;
import javax.naming.NameAlreadyBoundException;
import javax.naming.NameClassPair;
import javax.naming.NameNotFoundException;
import javax.naming.NameParser;
import javax.naming.NamingEnumeration;
import javax.naming.NamingException;
import javax.naming.NotContextException;
import javax.naming.OperationNotSupportedException;
import javax.naming.Reference;
import javax.naming.Referenceable;
import javax.naming.directory.Attributes;
import javax.naming.directory.DirContext;
import javax.naming.directory.InvalidAttributesException;
import javax.naming.spi.NamingManager;

import org.huihoo.jfox.jndi.url.JndiURL;

/**
 * 
 * @author <a href="mailto:young_yy@hotmail.com">Young Yang</a>
 */

public class ContextSupport implements Context, Serializable {
  /**
   * environment of this Context
   */
  private Hashtable env = new Hashtable();

  /**
   * �� context ���ֿռ䣬�磺/ , /test
   */
  private Name prefix;

  private NameParserImpl parser = NameParserImpl.getInstance();

  private NamingService server;

  public ContextSupport(Name prefix, Hashtable env) throws NamingException {
    init(prefix,env);
  }

  public ContextSupport(String prefix, Hashtable env) throws NamingException  {
    Name _prefix = null;
    if(prefix == null || prefix.trim().length() == 0){
      _prefix = parser.parse("/");
    }
    else {
      _prefix = parser.parse(prefix.trim());
    }

    init(_prefix,env);

  }

  private void init(Name prefix, Hashtable env) throws NamingException {
    if(env == null) {
      env = JNDIProperties.getDefaultEnvironment();
    }
    this.env = (Hashtable)env.clone();

    URL providerUri = null;
    try {
      providerUri = JndiURL.getURL(this,(String)env.get(Context.PROVIDER_URL));
    }
    catch(MalformedURLException e){
      e.printStackTrace();
      throw new NamingException("Conext initializing error ! Context.PROVIDER_URL = " + env.get(Context.PROVIDER_URL));
    }
    server = ServiceLocator.getNamingService(providerUri.getHost(),
                                             providerUri.getPort());
    if(prefix == null || prefix.toString().trim().length() == 0){
      this.prefix = parser.parse("/");
    }
    else {
      this.prefix = prefix;
    }

    // bound root Context as "/";
    try {
      if(this.prefix.toString().equals("/") && !server.isBound(prefix)){
        this.bind(prefix,this);
      }
    }
    catch(RemoteException e){
      throw new NamingException(e.getMessage());
    }
  }


  /**
   * Retrieves the named object.
   * If <tt>name</tt> is empty, returns a new instance of this context
   * (which represents the same naming context as this context, but its
   * environment may be modified independently and it may be accessed
   * concurrently).
   *
   * @param name  the name of the object to look up
   * @return	the object bound to <tt>name</tt>
   * @throws	NamingException if a naming exception is encountered
   *
   * @see #lookup(String)
   * @see #lookupLink(Name)
   */
  public Object lookup(Name name) throws NamingException {
    if(name.isEmpty()){ // empty name will return a new context
      return new ContextSupport(this.getNameInNamespace(),this.getEnvironment());
    }

    try{
      Name absoluteName = getAbsoluteName(name);
      Object obj = server.lookup(absoluteName);
      Object ret = obj;
      if(obj instanceof MarshalledObject){
        ret = ((MarshalledObject)obj).get();
      }
      else if (obj instanceof Reference) { // common reference
        // Dereference object
        if(obj instanceof LinkRef){ // link ref
          String ref = ((LinkRef)obj).getLinkName();
          return new InitialContext(env).lookup(ref);
        }
        else{ // other Reference, must use ObjectFactory.getObjectInstance
          ret = NamingManager.getObjectInstance(obj,absoluteName,this,env);
        }
      }

      return ret;
    }
    catch(NamingException e){
      throw e;
    }
    catch(Exception ex){
      ex.printStackTrace();
      throw new NamingException(ex.toString());
    }

  }

  /**
   * Retrieves the named object.
   * See {@link #lookup(Name)} for details.
   * @param name
   *		the name of the object to look up
   * @return	the object bound to <tt>name</tt>
   * @throws	NamingException if a naming exception is encountered
   */
  public Object lookup(String name) throws NamingException {
    return lookup(parser.parse(name));
  }

  /**
   * Binds a name to an object.
   * All intermediate contexts and the target context (that named by all
   * but terminal atomic component of the name) must already exist.
   *
   * @param name
   *		the name to bind; may not be empty
   * @param obj
   *		the object to bind; possibly null
   * @throws	NameAlreadyBoundException if name is already bound
   * @throws	InvalidAttributesException
   *	 	if object did not supply all mandatory attributes
   * @throws	NamingException if a naming exception is encountered
   *
   * @see #bind(String, Object)
   * @see #rebind(Name, Object)
   * @see DirContext#bind(Name, Object,
         *		Attributes)
   */
  public void bind(Name name, Object obj) throws NamingException {
    if(name.isEmpty()){
      throw new InvalidNameException(name.toString());
    }

    String classname = null;
    if(obj instanceof Referenceable){ // get the reference first
      obj = ((Referenceable)obj).getReference();
    }
    if(obj instanceof Reference){
      classname = ((Reference)obj).getClassName();
    }
    else {
      classname = obj.getClass().getName();
      //The server may be in the same JVM.
      if(server instanceof RemoteStub) {
//      if(!(obj instanceof Remote)){ // not a remote class , let it be
      try {
        // if the object is a remote object, MarshalledObject is the stub
        obj = new MarshalledObject(obj);
      }
      catch(IOException e){
        throw new NamingException(e.toString());
      }      
//      }
      }
    }
    try {
      server.bind(this,getAbsoluteName(name),obj,classname);
    }
    catch(RemoteException e){
      throw new NamingException(e.toString());
    }

  }

  /**
   * Binds a name to an object.
   * See {@link #bind(Name, Object)} for details.
   *
   * @param name
   *		the name to bind; may not be empty
   * @param obj
   *		the object to bind; possibly null
   * @throws	NameAlreadyBoundException if name is already bound
   * @throws	InvalidAttributesException
   *	 	if object did not supply all mandatory attributes
   * @throws	NamingException if a naming exception is encountered
   */
  public void bind(String name, Object obj) throws NamingException {
    bind(parser.parse(name),obj);
  }

  /**
   * Binds a name to an object, overwriting any existing binding.
   * All intermediate contexts and the target context (that named by all
   * but terminal atomic component of the name) must already exist.
   *
   * <p> If the object is a <tt>DirContext</tt>, any existing attributes
   * associated with the name are replaced with those of the object.
   * Otherwise, any existing attributes associated with the name remain
   * unchanged.
   *
   * @param name
   *		the name to bind; may not be empty
   * @param obj
   *		the object to bind; possibly null
   * @throws	InvalidAttributesException
   *	 	if object did not supply all mandatory attributes
   * @throws	NamingException if a naming exception is encountered
   *
   * @see #rebind(String, Object)
   * @see #bind(Name, Object)
   * @see DirContext#rebind(Name, Object,
         *		Attributes)
   * @see DirContext
   */
  public void rebind(Name name, Object obj) throws NamingException {
    if(name.isEmpty() || name.toString().trim().equals("/")){ // Empty names are not allowed
      throw new InvalidNameException(name.toString());
    }

    String classname = null;
    if(obj instanceof Referenceable){ // get the reference first
      obj = ((Referenceable)obj).getReference();
    }
    if(obj instanceof Reference){
      classname = ((Reference)obj).getClassName();
    }
    else{
      classname = obj.getClass().getName();
      try {
        if(!(obj instanceof MarshalledObject)){
          obj = new MarshalledObject(obj);
        }
//        System.out.println("rebind ok");
      }
      catch(IOException e){
        throw new NamingException(e.toString());
      }
//      }
    }

    try {
      server.rebind(this,getAbsoluteName(name),obj,classname);
    }
    catch(RemoteException e){
      throw new NamingException(e.toString());
    }

  }

  /**
   * Binds a name to an object, overwriting any existing binding.
   * See {@link #rebind(Name, Object)} for details.
   *
   * @param name
   *		the name to bind; may not be empty
   * @param obj
   *		the object to bind; possibly null
   * @throws	InvalidAttributesException
   *	 	if object did not supply all mandatory attributes
   * @throws	NamingException if a naming exception is encountered
   */
  public void rebind(String name, Object obj) throws NamingException {
    rebind(parser.parse(name),obj);
  }

  /**
   * Unbinds the named object.
   * Removes the terminal atomic name in <code>name</code>
   * from the target context--that named by all but the terminal
   * atomic part of <code>name</code>.
   *
   * <p> This method is idempotent.
   * It succeeds even if the terminal atomic name
   * is not bound in the target context, but throws
   * <tt>NameNotFoundException</tt>
   * if any of the intermediate contexts do not exist.
   *
   * <p> Any attributes associated with the name are removed.
   * Intermediate contexts are not changed.
   *
   * @param name
   *		the name to unbind; may not be empty
   * @throws	NameNotFoundException if an intermediate context does not exist
   * @throws	NamingException if a naming exception is encountered
   * @see #unbind(String)
   */
  public void unbind(Name name) throws NamingException {
    if (name.isEmpty() || name.toString().trim().equals("/")) {
      throw new InvalidNameException(name.toString());
    }
    try{
      Name absoluteName = getAbsoluteName(name);
      server.unbind(this,absoluteName);
    }
    catch(RemoteException ex){
      throw new NamingException(ex.toString());
    }

  }

  /**
   * Unbinds the named object.
   * See {@link #unbind(Name)} for details.
   *
   * @param name
   *		the name to unbind; may not be empty
   * @throws	NameNotFoundException if an intermediate context does not exist
   * @throws	NamingException if a naming exception is encountered
   */
  public void unbind(String name) throws NamingException {
    unbind(parser.parse(name));
  }

  /**
   * Binds a new name to the object bound to an old name, and unbinds
   * the old name.  Both names are relative to this context.
   * Any attributes associated with the old name become associated
   * with the new name.
   * Intermediate contexts of the old name are not changed.
   *
   * @param oldName
   *		the name of the existing binding; may not be empty
   * @param newName
   *		the name of the new binding; may not be empty
   * @throws	NameAlreadyBoundException if <tt>newName</tt> is already bound
   * @throws	NamingException if a naming exception is encountered
   *
   * @see #rename(String, String)
   * @see #bind(Name, Object)
   * @see #rebind(Name, Object)
   */
  public void rename(Name oldName, Name newName) throws NamingException {
    if(oldName.isEmpty() || oldName.toString().trim().equals("/") ||
           newName.isEmpty() || newName.toString().trim().equals("/")){
      throw new InvalidNameException("oldName: " + oldName.toString() + ", newName: " + newName);
    }
    try {
      Object obj = server.lookup(oldName);
      bind(newName,obj);
    }
    catch(RemoteException e){
      throw new NamingException(e.toString());
    }

    unbind(oldName);
  }

  /**
   * Binds a new name to the object bound to an old name, and unbinds
   * the old name.
   * See {@link #rename(Name, Name)} for details.
   *
   * @param oldName
   *		the name of the existing binding; may not be empty
   * @param newName
   *		the name of the new binding; may not be empty
   * @throws	NameAlreadyBoundException if <tt>newName</tt> is already bound
   * @throws	NamingException if a naming exception is encountered
   */
  public void rename(String oldName, String newName) throws NamingException {
    rename(parser.parse(oldName),parser.parse(newName));
  }

  /**
   * Enumerates the names bound in the named context, along with the
   * class names of objects bound to them.
   * The contents of any subcontexts are not included.
   *
   * <p> If a binding is added to or removed from this context,
   * its effect on an enumeration previously returned is undefined.
   *
   * @param name
   *		the name of the context to list
   * @return	an enumeration of the names and class names of the
   *		bindings in this context.  Each element of the
   *		enumeration is of type <tt>NameClassPair</tt>.
   * @throws	NamingException if a naming exception is encountered
   *
   * @see #list(String)
   * @see #listBindings(Name)
   * @see NameClassPair
   */
  public NamingEnumeration list(Name name) throws NamingException {
    try{
      List list = server.list(this,getAbsoluteName(name));
      return new NamingEnumerationImpl(list);
    }
    catch(RemoteException ex){
      throw new NamingException(ex.toString());
    }
  }

  /**
   * Enumerates the names bound in the named context, along with the
   * class names of objects bound to them.
   * See {@link #list(Name)} for details.
   *
   * @param name
   *		the name of the context to list
   * @return	an enumeration of the names and class names of the
   *		bindings in this context.  Each element of the
   *		enumeration is of type <tt>NameClassPair</tt>.
   * @throws	NamingException if a naming exception is encountered
   */
  public NamingEnumeration list(String name) throws NamingException {
    return list(parser.parse(name));
  }

  /**
   * Enumerates the names bound in the named context, along with the
   * objects bound to them.
   * The contents of any subcontexts are not included.
   *
   * <p> If a binding is added to or removed from this context,
   * its effect on an enumeration previously returned is undefined.
   *
   * @param name
   *		the name of the context to list
   * @return	an enumeration of the bindings in this context.
   *		Each element of the enumeration is of type
   *		<tt>Binding</tt>.
   * @throws	NamingException if a naming exception is encountered
   *
   * @see #listBindings(String)
   * @see #list(Name)
   * @see Binding
   */
  public NamingEnumeration listBindings(Name name) throws NamingException {
    try{
      List list = server.listBindings(this,getAbsoluteName(name));
      return new NamingEnumerationImpl(list);
    }
    catch(RemoteException ex){
      throw new NamingException(ex.toString());
    }

  }

  /**
   * Enumerates the names bound in the named context, along with the
   * objects bound to them.
   * See {@link #listBindings(Name)} for details.
   *
   * @param name
   *		the name of the context to list
   * @return	an enumeration of the bindings in this context.
   *		Each element of the enumeration is of type
   *		<tt>Binding</tt>.
   * @throws	NamingException if a naming exception is encountered
   */
  public NamingEnumeration listBindings(String name) throws NamingException {
    return listBindings(parser.parse(name));
  }

  /**
   * Destroys the named context and removes it from the namespace.
   * Any attributes associated with the name are also removed.
   * Intermediate contexts are not destroyed.
   *
   * <p> This method is idempotent.
   * It succeeds even if the terminal atomic name
   * is not bound in the target context, but throws
   * <tt>NameNotFoundException</tt>
   * if any of the intermediate contexts do not exist.
   *
   * <p> In a federated naming system, a context from one naming system
   * may be bound to a name in another.  One can subsequently
   * look up and perform operations on the foreign context using a
   * composite name.  However, an attempt destroy the context using
   * this composite name will fail with
   * <tt>NotContextException</tt>, because the foreign context is not
   * a "subcontext" of the context in which it is bound.
   * Instead, use <tt>unbind()</tt> to remove the
   * binding of the foreign context.  Destroying the foreign context
   * requires that the <tt>destroySubcontext()</tt> be performed
   * on a context from the foreign context's "native" naming system.
   *
   * @param name
   *		the name of the context to be destroyed; may not be empty
   * @throws	NameNotFoundException if an intermediate context does not exist
   * @throws	NotContextException if the name is bound but does not name a
   *		context, or does not name a context of the appropriate type
   * @throws	ContextNotEmptyException if the named context is not empty
   * @throws	NamingException if a naming exception is encountered
   *
   * @see #destroySubcontext(String)
   */
  public void destroySubcontext(Name name) throws NamingException {
    if(name.isEmpty() || name.toString().trim().equals("/")){ // Empty names are not allowed
      throw new InvalidNameException(name.toString());
    }
    try{
      server.destroySubcontext(this,getAbsoluteName(name));
    }
    catch(RemoteException e){
      throw new NamingException(e.toString());
    }

  }

  /**
   * Destroys the named context and removes it from the namespace.
   * See {@link #destroySubcontext(Name)} for details.
   *
   * @param name
   *		the name of the context to be destroyed; may not be empty
   * @throws	NameNotFoundException if an intermediate context does not exist
   * @throws	NotContextException if the name is bound but does not name a
   *		context, or does not name a context of the appropriate type
   * @throws	ContextNotEmptyException if the named context is not empty
   * @throws	NamingException if a naming exception is encountered
   */
  public void destroySubcontext(String name) throws NamingException {
    destroySubcontext(parser.parse(name));
  }

  /**
   * Creates and binds a new context.
   * Creates a new context with the given name and binds it in
   * the target context (that named by all but terminal atomic
   * component of the name).  All intermediate contexts and the
   * target context must already exist.
   *
   * @param name
   *		the name of the context to create; may not be empty
   * @return	the newly created context
   *
   * @throws	NameAlreadyBoundException if name is already bound
   * @throws	InvalidAttributesException
   *		if creation of the subcontext requires specification of
   *		mandatory attributes
   * @throws	NamingException if a naming exception is encountered
   *
   * @see #createSubcontext(String)
   * @see DirContext#createSubcontext
   */
  public Context createSubcontext(Name name) throws NamingException {
    if(name.isEmpty() || name.toString().trim().equals("/")){
      throw new InvalidNameException(name.toString());
    }
    try{
      name = getAbsoluteName(name);
      return server.createSubcontext(this,name);
    }
    catch(RemoteException ex){
      throw new NamingException(ex.toString());
    }

  }

  /**
   * Creates and binds a new context.
   * See {@link #createSubcontext(Name)} for details.
   *
   * @param name
   *		the name of the context to create; may not be empty
   * @return	the newly created context
   *
   * @throws	NameAlreadyBoundException if name is already bound
   * @throws	InvalidAttributesException
   *		if creation of the subcontext requires specification of
   *		mandatory attributes
   * @throws	NamingException if a naming exception is encountered
   */
  public Context createSubcontext(String name) throws NamingException {
    return createSubcontext(parser.parse(name));
  }

  /**
   * Retrieves the named object, following links except
   * for the terminal atomic component of the name.
   * If the object bound to <tt>name</tt> is not a link,
   * returns the object itself.
   *
   * @param name
   *		the name of the object to look up
   * @return	the object bound to <tt>name</tt>, not following the
   *		terminal link (if any).
   * @throws	NamingException if a naming exception is encountered
   *
   * @see #lookupLink(String)
   */
  public Object lookupLink(Name name) throws NamingException {
    return lookup(name);
  }

  /**
   * Retrieves the named object, following links except
   * for the terminal atomic component of the name.
   * See {@link #lookupLink(Name)} for details.
   *
   * @param name
   *		the name of the object to look up
   * @return	the object bound to <tt>name</tt>, not following the
   *		terminal link (if any)
   * @throws	NamingException if a naming exception is encountered
   */
  public Object lookupLink(String name) throws NamingException {
    return lookupLink(parser.parse(name));
  }

  /**
   * Retrieves the parser associated with the named context.
   * In a federation of namespaces, different naming systems will
   * parse names differently.  This method allows an application
   * to get a parser for parsing names into their atomic components
   * using the naming convention of a particular naming system.
   * Within any single naming system, <tt>NameParser</tt> objects
   * returned by this method must be equal (using the <tt>equals()</tt>
   * test).
   *
   * @param name
   *		the name of the context from which to get the parser
   * @return	a name parser that can parse compound names into their atomic
   *		components
   * @throws	NamingException if a naming exception is encountered
   *
   * @see #getNameParser(String)
   * @see CompoundName
   */
  public NameParser getNameParser(Name name) throws NamingException {
    return parser;
  }

  /**
   * Retrieves the parser associated with the named context.
   * See {@link #getNameParser(Name)} for details.
   *
   * @param name
   *		the name of the context from which to get the parser
   * @return	a name parser that can parse compound names into their atomic
   *		components
   * @throws	NamingException if a naming exception is encountered
   */
  public NameParser getNameParser(String name) throws NamingException {
    return parser;
  }

  /**
   * Composes the name of this context with a name relative to
   * this context.
   * Given a name (<code>name</code>) relative to this context, and
   * the name (<code>prefix</code>) of this context relative to one
   * of its ancestors, this method returns the composition of the
   * two names using the syntax appropriate for the naming
   * system(s) involved.  That is, if <code>name</code> names an
   * object relative to this context, the result is the name of the
   * same object, but relative to the ancestor context.  None of the
   * names may be null.
   * <p>
   * For example, if this context is named "wiz.com" relative
   * to the initial context, then
   * <pre>
   *	composeName("east", "wiz.com")	</pre>
   * might return <code>"east.wiz.com"</code>.
   * If instead this context is named "org/research", then
   * <pre>
   *	composeName("user/jane", "org/research")	</pre>
   * might return <code>"org/research/user/jane"</code> while
   * <pre>
   *	composeName("user/jane", "research")	</pre>
   * returns <code>"research/user/jane"</code>.
   *
   * @param name
   *		a name relative to this context
   * @param prefix
   *		the name of this context relative to one of its ancestors
   * @return	the composition of <code>prefix</code> and <code>name</code>
   * @throws	NamingException if a naming exception is encountered
   *
   * @see #composeName(String, String)
   */
  public Name composeName(Name name, Name prefix) throws NamingException {
    Name newName = (Name)(prefix.clone());
    newName.addAll(name);
    return newName;
  }

  /**
   * Composes the name of this context with a name relative to
   * this context.
   * See {@link #composeName(Name, Name)} for details.
   *
   * @param name
   *		a name relative to this context
   * @param prefix
   *		the name of this context relative to one of its ancestors
   * @return	the composition of <code>prefix</code> and <code>name</code>
   * @throws	NamingException if a naming exception is encountered
   */
  public String composeName(String name, String prefix) throws NamingException {
    Name result = composeName(parser.parse(name),	parser.parse(prefix));
    return result.toString();
  }

  /**
   * Adds a new environment property to the environment of this
   * context.  If the property already exists, its value is overwritten.
   * See class description for more details on environment properties.
   *
   * @param propName
   *		the name of the environment property to add; may not be null
   * @param propVal
   *		the value of the property to add; may not be null
   * @return	the previous value of the property, or null if the property was
   *		not in the environment before
   * @throws	NamingException if a naming exception is encountered
   *
   * @see #getEnvironment()
   * @see #removeFromEnvironment(String)
   */
  public Object addToEnvironment(String propName, Object propVal)
         throws NamingException {

    return env.put(propName,propVal);
  }

  /**
   * Removes an environment property from the environment of this
   * context.  See class description for more details on environment
   * properties.
   *
   * @param propName
   *		the name of the environment property to remove; may not be null
   * @return	the previous value of the property, or null if the property was
   *		not in the environment
   * @throws	NamingException if a naming exception is encountered
   *
   * @see #getEnvironment()
   * @see #addToEnvironment(String, Object)
   */
  public Object removeFromEnvironment(String propName)
         throws NamingException {
    return env.remove(propName);
  }

  /**
   * Retrieves the environment in effect for this context.
   * See class description for more details on environment properties.
   *
   * <p> The caller should not make any changes to the object returned:
   * their effect on the context is undefined.
   * The environment of this context may be changed using
   * <tt>addToEnvironment()</tt> and <tt>removeFromEnvironment()</tt>.
   *
   * @return	the environment of this context; never null
   * @throws	NamingException if a naming exception is encountered
   *
   * @see #addToEnvironment(String, Object)
   * @see #removeFromEnvironment(String)
   */
  public Hashtable getEnvironment() throws NamingException {
    return JNDIProperties.cloneEnvironment(env);
  }

  /**
   * Closes this context.
   * This method releases this context's resources immediately, instead of
   * waiting for them to be released automatically by the garbage collector.
   *
   * <p> This method is idempotent:  invoking it on a context that has
   * already been closed has no effect.  Invoking any other method
   * on a closed context is not allowed, and results in undefined behaviour.
   *
   * @throws	NamingException if a naming exception is encountered
   */
  public void close() throws NamingException {
    try{
      server.closeSubcontext(this,prefix);
//      server = null;
//      env = null;
    }
    catch(RemoteException e){
      throw new NamingException(e.toString());
    }
  }

  /**
   * Retrieves the full name of this context within its own namespace.
   *
   * <p> Many naming services have a notion of a "full name" for objects
   * in their respective namespaces.  For example, an LDAP entry has
   * a distinguished name, and a DNS record has a fully qualified name.
   * This method allows the client application to retrieve this name.
   * The string returned by this method is not a JNDI composite name
   * and should not be passed directly to context methods.
   * In naming systems for which the notion of full name does not
   * make sense, <tt>OperationNotSupportedException</tt> is thrown.
   *
   * @return	this context's name in its own namespace; never null
   * @throws	OperationNotSupportedException if the naming system does
   *		not have the notion of a full name
   * @throws	NamingException if a naming exception is encountered
   *
   * @since 1.3
   */
  public String getNameInNamespace() throws NamingException {
    return prefix.toString();
  }

  /**
   * get the absolute path in name
   */
  Name getAbsoluteName(Name name) throws NamingException{
//    if(name.isEmpty()) return name;

//    Name absName = null;
    try {
      URL url = JndiURL.getURL(this,name.toString());
      return parser.parse(url.getPath());

/*
      URI uri = new URI(name.toString());
      String path = uri.getPath();

      // trip last "/"
      while(path.length() > 1 && path.endsWith("/")) {
        path = path.substring(0,path.length()-1);
      }

      if(!path.startsWith("/")) { // �����·��
        if(!prefix.toString().startsWith("/")){
          path = prefix + "/" + path;
        }
        absName = composeName(parser.parse(path),prefix);
      }
      else { // �Ѿ��Ǿ��·����absName �����path
//        System.out.println("Path: " + path);
        absName = parser.parse(path);
      }
*/
    }
    catch(MalformedURLException e){
      throw new NamingException(e.getMessage());
    }

//    return absName;
  }

}
