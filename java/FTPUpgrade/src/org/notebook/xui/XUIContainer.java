package org.notebook.xui;

import java.awt.BorderLayout;
import java.beans.Beans;
import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.net.URL;
import java.util.HashMap;
import java.util.Map;
import java.util.Stack;

import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

/**
 * 解析一个XML配置文件，组装成一个GUI对象。
 * 
 * <win name='' class='' title='信息'>
 *     <menu class=''/>
 *     
 *     <layout class=''>
 *     <rightComponent ref='name' />
 *     <component class=''>
 *     </component>
 * </win>
 * @author deon
 */
public class XUIContainer {
	private Log log = LogFactory.getLog("xui");
	private Map<String, Object> cache = new HashMap<String, Object>();
	
	public void load(URL resource){
		SAXParserFactory saxfac = SAXParserFactory.newInstance();
		SAXParser parser = null;
		InputStream in = null;
		try {
			parser = saxfac.newSAXParser();
			in = resource.openStream();
			Context root = new Context();
			root.parent = this;
			parser.parse(in, new XUISAXHandler(root));
		}catch (Exception e) {
			log.error(e.toString(), e);
		}finally{
			if(in != null){
				try {
					in.close();
				} catch (IOException e) {
					log.warn(e.toString(), e);
				}
			}
		}
	}

	public Object getByName(String name){
		return cache.get(name);
	}
	
	public Object[] getAllComponents(){
		return cache.values().toArray();
	}
	
	private void addComponent(String name, Object o){
		log.info("add component, name:" + name + ", " + o.toString());
		this.cache.put(name, o);
	}
	
	static class Context{
		Object parent = null;
		//当前节点创建的对象。
		Object obj = null;
		//当前正在处理的tag名称
		String tag = null;
		
		//调用设置操作的参数列表。
		Object[] args = null;
		//参数的类型列表
		Class[] argClass = null;
	}
	
	class XUISAXHandler extends DefaultHandler{
		private Stack<Context> context = new Stack<Context>();
		public XUISAXHandler(Context c){this.context.push(c);};
		
		public void startElement(String uri, String localName, 
								 String qName, 
								 Attributes attributes){
			//System.out.println("start uri:" + uri + ", localName:"+ localName + ", qName:" + qName);
			if(log.isDebugEnabled()){
				log.debug("start tag:" + qName);
			}
			Context newContext = new Context();
			newContext.tag = qName;
			Context pc = context.peek();
			newContext.parent = pc.obj != null ? pc.obj : pc.parent;
			context.push(newContext);
			
			processAttributes(newContext, attributes); 
			processTag(newContext);
			String name = attributes.getValue("name");
			if(name != null && newContext.obj != null){
				addComponent(name, newContext.obj);
			}
		}
		
		public void endElement(String uri, String localName,
                			   String qName) throws SAXException{			
			if(log.isDebugEnabled()){
				log.debug("end tag:" + qName);
			}
			context.pop();
		}
		
		private void sizeHandler(Attributes attributes){
			
		}
		
		/**
		 * 处理tag, 根据参数选择个合适的上级对象方法进行调用。
		 * 
		 * 1. 没有参数则调用get＊方法
		 * 2. 有参数需要自动查找合适的方法. set/add/*
		 * 
		 * @param context
		 */
		private void processTag(Context context){
			//context.parent
			Method callMethod = null;
			if(context.args == null){
				String first = context.tag.substring(0, 1); 
				String suffix = context.tag.substring(1);				
				try {
					try{
						callMethod = context.parent.getClass().getMethod("get" + first.toUpperCase() + suffix, new Class[]{});
					}catch(NoSuchMethodException e){}
					if(callMethod == null){
						try{
							callMethod = context.parent.getClass().getMethod(context.tag, new Class[]{});
						}catch(NoSuchMethodException e){}
					}
					if(callMethod != null){
						context.obj = callMethod.invoke(context.parent, new Object[]{});
					}
				}catch (Exception e) {
					log.error(e.toString(), e);
				}
			}else {
				Class[] args = null;
				String tag = context.tag.toLowerCase();
				for(Method m : context.parent.getClass().getMethods()){
					if(!Modifier.isPublic(m.getModifiers()))continue;
					//if(!m.isAccessible())continue;
					args = m.getParameterTypes();
					if(args.length != context.args.length)continue;
					if(!m.getName().toLowerCase().endsWith(tag)) continue;
					if(m.getName().startsWith("get"))continue;
					if(!isAssignAble(args, context.argClass)) continue;
					try {
						m.invoke(context.parent, context.args);
						callMethod = m;
						break;
					} catch(Exception e) {
						log.error(e.toString(), e);
					}
				}
			}
			if(callMethod == null){
				log.warn(String.format("Not found method for tag '%s'", context.tag));
			}else {
				log.info(String.format("tag '%s' --> '%s'", context.tag, callMethod.getName()));
			}
		}
		
		private boolean isAssignAble(Class[] args, Class[] apply){
			for(int i = 0; i < args.length; i++){
				if(apply[i] == null) continue;
				if(!args[i].isAssignableFrom(apply[i])) return false;
			}
			return true;
		}
		
		/**
		 * 处理XML的属性:
		 * 1. 创建相关对象
		 * 2. 处理参数列表
		 * @param context
		 * @param attributes
		 */
		private void processAttributes(Context context, Attributes attributes){
			String clazz = attributes.getValue("class");
			String ref = attributes.getValue("ref");
			//attributes.getLength()
			if(clazz != null){
				try {
					context.obj = Beans.instantiate(null, clazz);
				} catch (Exception e) {
					log.error("fail to instantiate class:" + clazz + ", error:" + e.toString());
					return;
				}			
				String align = attributes.getValue("align");
				if(align != null){
					context.args = new Object[]{context.obj, align};
				}else {
					context.args = new Object[]{context.obj};
				}
			}else if(ref != null){
				context.obj = getByName(ref);
				context.args = new Object[]{context.obj};
			}			
			String x = attributes.getValue("x");
			String y = attributes.getValue("y");
			if(x != null && y != null){
				int ix = Integer.parseInt(x);
				int iy = Integer.parseInt(y);
				context.args = new Object[]{ix, iy};
				context.argClass = new Class[]{int.class, int.class};
			}
			if(context.args != null && context.argClass == null){
				context.argClass = new Class[context.args.length];
				for(int i = 0; i < context.args.length; i++){
					if(context.args[i] != null){
						context.argClass[i] = context.args[i].getClass();
					}
				}
			}
		}
	}
	
	private Context createContext(Context parent, String tag, Attributes attributes){
		if(tag.equals("size")){
			
		}
		return null;
	}
	
}
