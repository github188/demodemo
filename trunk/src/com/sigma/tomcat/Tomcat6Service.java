package com.sigma.tomcat;

import static com.sigma.demo.mbean.notification.DeployerNotification.POST_DEMO_STARTED;
import static com.sigma.demo.mbean.notification.DeployerNotification.POST_DEMO_STOPED;

import java.io.File;
import java.io.IOException;
import java.util.Collection;

import javax.management.Attribute;
import javax.management.ObjectName;

import org.apache.catalina.Context;
import org.apache.catalina.Engine;
import org.apache.catalina.Host;
import org.apache.catalina.LifecycleException;
import org.apache.catalina.connector.Connector;
import org.apache.catalina.realm.MemoryRealm;
import org.apache.catalina.startup.Embedded;
import org.apache.tomcat.util.IntrospectionUtils;

import com.sigma.demo.BasicDemoServiceMBean;
import com.sigma.demo.deploy.DemoLifeCycleListener;
import com.sigma.demo.deploy.DeployEntity;
import com.sigma.demo.mbean.notification.TyppedNotificationFilter;
import com.sigma.demo.service.AbstractService;
import com.sigma.demo.service.FailServiceException;
import com.sigma.demo.web.SiteLoader;

public class Tomcat6Service extends AbstractService 
	implements Tomcat6ServiceMBean{
	
	private Embedded embedded = null;
	private Host host = null;
    private Context rootcontext;
    //private int listenPort = 8080;
    private Connector connector = null;
    private String address = "0.0.0.0";
    
    private DemoLifeCycleListener deployListener = null; 
    
	public void shutDown() throws FailServiceException {

		//this.context
		try {
			if(embedded != null){
				this.embedded.stop();
			}
		}catch(LifecycleException e) {
			log.error(e, e);
		}
		
		try{
			context.removeNotificationListener(context.getServerName(), deployListener);
		}catch(Exception e){
			log.error(e, e);
		}
	}

	public void start() throws FailServiceException {
		
		int port = context.getIntParameter("Listen", 8080);
		address = context.getStringParameter("Address", "0.0.0.0");
		String home = context.getStringParameter("Home", new File(".").getAbsolutePath());
		String webRoot = context.getStringParameter("WebRoot", new File(home, "webroot").getAbsolutePath());
		
		try{
			//log.debug(Constants.Package);
			this.bootstrap(home, webRoot,port, address);
		}catch(LifecycleException e){
			this.setErrorMessage(e.getMessage());
			throw new FailServiceException(e.getMessage());
		}
		
		try{
		
			deployListener = new DemoLifeCycleListener(this){
					protected void handleDeployEvent(String type, ObjectName deployer, DeployEntity entity) {
						if(type.equals(POST_DEMO_STARTED)) {
							deploy("/~" + entity.getService().getDemoName(),
									entity.getServiceFile().getParentFile());
						}else if(type.equals(POST_DEMO_STOPED)) {
							undeploy("/~" + entity.getService().getDemoName());
						}
					}
			};
			
			context.addNotificationListener(context.getServerName(), deployListener,
					new TyppedNotificationFilter("deployer", this), null);

			ObjectName demoList = new ObjectName("*:*,type=demo");
			DeployEntity info = null;
			for(ObjectName demo: (Collection<ObjectName>)context.queryService(demoList)){
				info = (DeployEntity)context.getAttribute(demo, BasicDemoServiceMBean.ATTR_DEPLOY_INFO);
				if(info != null && info.getService() != null){
					deploy("/~" + info.getService().getDemoName(),
							info.getServiceFile().getParentFile());
				}
			}
			
		}catch(Exception e){
			log.error(e,e);
			throw new FailServiceException(e.getMessage());			
		}
	}

	private void bootstrap(String catalinaHome,
						   String webRoot, 
						   int port, 
						   String address
						   ) throws LifecycleException {
		Engine engine = null;

        // Create an embedded server
        this.embedded = new Embedded();
        this.embedded.setUseNaming(false);
        this.embedded.setCatalinaHome(catalinaHome);        
        // set the memory realm
        MemoryRealm memRealm = new MemoryRealm();
        this.embedded.setRealm(memRealm);
        
        engine = this.embedded.createEngine();
        engine.setDefaultHost("localhost");

        // Create a default virtual host
        File tempHost = null;
        
        try {
        	tempHost = File.createTempFile("empty", "");
        	tempHost.mkdirs();
        	log.debug("Embbed Host Root:" + tempHost.getAbsolutePath());
		} catch (IOException e) {
			tempHost = new File(".");
			log.warn(e,e);
		}
        this.host = this.embedded.createHost("localhost", tempHost.getAbsolutePath());
        engine.addChild(this.host);

        // Create the ROOT context
        this.rootcontext = this.embedded.createContext("", webRoot);

        this.rootcontext.addApplicationListener(SiteLoader.class.getName());
        this.rootcontext.setPrivileged(true);
        this.rootcontext.setReloadable(false);
        this.rootcontext.addWelcomeFile("index.jsp");
        this.host.addChild(this.rootcontext);

        // Install the assembled container hierarchy
        this.embedded.addEngine(engine);
        //String addr = null;
		connector = this.embedded.createConnector(address, port, false);
		if(connector == null){
	        /*
	         * embedded.createConnector(...)
	         * seems to be broken.. it always returns a null connector.
	         * see work around below
	         */
	        try {
	            connector = new Connector();
	            //httpConnector.setScheme("http");
	            connector.setSecure(false);
	            //address = InetAddress.getLocalHost();
                IntrospectionUtils.setProperty(connector, "address", "" + address);
	            IntrospectionUtils.setProperty(connector, "port", "" + port);
	            
	        } catch (Exception ex) {
	            ex.printStackTrace();
	        }
		}
        connector.setEnableLookups(false);
        this.embedded.addConnector(connector);
			
        // Start the embedded server
        this.embedded.start();		
        
	}

	public String getListenAddress() {
		return this.address;
	}

	public int getListenPort() {
		return connector.getPort();
	}

	public void deploy(String url, File dir) {
		if(host.findChild(url) != null)
			return;
		if(dir == null || !dir.exists() || !dir.isDirectory())
			return;
		
		log.debug("deploy new context:" + url + ", dir:" + dir.getAbsolutePath());
		
		this.host.addChild(this.embedded.createContext(url, dir.getAbsolutePath()));
	}

	public void undeploy(String url) {
		if(host.findChild(url) != null){
			this.host.removeChild(host.findChild(url));
			log.debug("undeploy web context:" + url);
		}
	}
}
