from django.http import HttpResponse
from pyshop.settings import APP_ROOT

def py_manage(r, ):
    template = """
    <?xml version="1.0" encoding="utf-8"?>
    <!-- JNLP File for FiveChess Application -->
    <jnlp
      spec="1.5+"
      codebase="%(app_url)s/static/" href="PyShop.jnlp?name=%(name)s&action=%(action)s">
      <information>
        <title>Testing tools shop</title>
        <vendor>IPATA</vendor>
        <description>A JNLP wrapper for starting easy_install in client.</description>
      </information>
    
    <security>
        <all-permissions/> 
    </security>
      
      <resources>
        <j2se version="1.5+" java-vm-args=""/>
        <jar href="pyshop-0.1.jar"/>
      </resources>
      <application-desc main-class="com.nsn.scripts.jnlp.PyPi">
          <argument>%(name)s</argument>
          <argument>%(action)s</argument>
      </application-desc>
    </jnlp>
    """
    host = (r.META["HTTP_HOST"])
    port = (r.META["SERVER_PORT"])
    if ":" not in host and port not in ['80', 0]:
        port = ":%s" % port
    else:
        port = ""
    
    app_url = "http://%s%s%s" % (host, port, APP_ROOT)
    name = r.REQUEST.get('name', '') 
    action = r.REQUEST.get('action', '')
    
    return HttpResponse(template % locals(), "application/x-java-jnlp-file")

def py_loader(r, name):
    template = """
    <?xml version="1.0" encoding="utf-8"?>
    <!-- JNLP File for FiveChess Application -->
    <jnlp
      spec="1.5+"
      codebase="%(app_url)s/static/" href="%(name)s.jnlp">
      <information>
        <title>Starting %(name)s online...</title>
        <vendor>IPATA</vendor>
        <description>A script loader... </description>
      </information>
      
    <icon href="%(app_url)s/dist/%(name)s/logo.jpg"/>
    <offline-allowed/> 
    <shortcut online="false">
      <desktop/>
    </shortcut>
    
    <security>
        <all-permissions/> 
    </security>
      
      <resources>
        <j2se version="1.5+" java-vm-args=""/>
        <jar href="pyshop-0.1.jar"/>
      </resources>
      <application-desc main-class="com.nsn.scripts.jnlp.PyPi">
          <argument>%(name)s</argument>
          <argument>-start</argument>
          <argument>%(main_entry)s</argument>
      </application-desc>
    </jnlp>
    """
    host = (r.META["HTTP_HOST"])
    port = (r.META["SERVER_PORT"])
    if ":" not in host and port not in ['80', 0]:
        port = ":%s" % port
    else:
        port = ""
    app_url = "http://%s%s%s" % (host, port, APP_ROOT)
    
    import pyshop.coreapp.models as m
    try:
        main_entry = m.Project.objects.get(name=name).main_entry
    except:
        main_entry = ''
        
    return HttpResponse(template % locals(), "application/x-java-jnlp-file")
    