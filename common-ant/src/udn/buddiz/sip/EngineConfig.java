package udn.buddiz.sip;

import java.io.IOException;
import java.io.InputStream;
import java.util.Properties;

import udn.buddiz.utils.Parameter;

public class EngineConfig {
	
	private PropertiesParameter param = null;
	
	public EngineConfig(InputStream is){
		param = new PropertiesParameter(is);
	}
	
	public Properties properties(){
		return this.param.properties;
	}
	
	public void setProperty(String name, String value){
		this.param.properties.setProperty(name, value);
	}
	
	
	class PropertiesParameter extends Parameter{
		Properties properties = new Properties();
		public PropertiesParameter(InputStream is) {
			try {
				properties.load(is);
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		
		public String getStringParameter(String name, String def){
			String value = properties.getProperty(name); 
			if(value == null || "".equals(value)){
				value = def;
			}
			return value;
		}
	}


	/**
	 * 注册域
	 * @return
	 */
	public String getRegistrar() {
		return this.param.getStringParameter("registrar", null);
	}

	/**
	 * SIP本地监听端口
	 * @return
	 */
	public int getPort() {
		return this.param.getIntParameter("port", 1200);
	}

	/**
	 * 是否在程序启动时注册客户端。
	 * @return
	 */	
	public boolean getShouldRegister(){
		return this.param.getBooleanParameter("register", true);
		
	}
	
	/**
	 * 是否在程序启动时注册客户端。
	 * @return
	 */	
	public boolean getShouldSubscribe(){
		return this.param.getBooleanParameter("subscribe", true);
		
	}	

	public int getExpires() {
		return this.param.getIntParameter("expires", 1200);
	}

	public int getSubExpires() {
		return this.param.getIntParameter("subExpires", 1200);
	}	
	
	public void setRegistrar(String registrar) {
		this.param.properties.setProperty("registrar", registrar);
	}

	public void setExpires(int expires) {
		this.param.properties.setProperty("expires", expires + "");
	}

	public void setSubExpires(int subExpires) {
		this.param.properties.setProperty("subExpires", subExpires + "");

	}
	
	
}
