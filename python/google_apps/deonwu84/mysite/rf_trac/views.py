import logging
from google.appengine.ext import db
from models import *
from datetime import datetime
from robot_utils import RobotUtils
from trac.utils import urlread
from google.appengine.api import memcache
from trac import project_settings, CATEGORY_SETTING

def api(r, key=''):
    project = RobotProject.all().filter('prj_key =', key).get()
    if project:
        host = (r.META["SERVER_NAME"])
        port = (r.META["SERVER_PORT"])
        port = port != '80' and ":%s" % port or ""
        endpoint = "http://%s%s/rf_trac/" % (host, port)
        
        if host.count("localhost") > 0:
            return ("rf_trac_prj_api.html", {"project": project,
                                             "endpoint": endpoint,
                                             "settings": __build_setting_js(r, project)})
        
        plugin_js = memcache.get("trac_plugin_js", namespace='rftrac')
        if not plugin_js or host.count("localhost"):
            jscdn = host.count("deonwu84") and "jsicdn01.appspot.com" or "localhost:8888"
            plugin_js = urlread("http://%s/cdn/jQuery/jquery-1.2.6.min.js" % jscdn)
            plugin_js += urlread("http://%s/cdn/jQuery/jquery-ui.js" % jscdn)
            plugin_js += urlread("http://%s/cdn/=rftrac:setup_robot_trac" % jscdn)
            memcache.add(key="trac_plugin_js", 
                         value=plugin_js, 
                         time=3600, namespace='rftrac')
                
        plugin_js += """;setup_robot_trac("%s", "%s", %s);""" %(endpoint, 
                                                                key,
                                                                __build_setting_js(r, project))
        
        return ("text/javascript", plugin_js)
    else:
        return """alert("the project key '%s' is not registered!");""" % key

def __build_setting_js(r, projects):
    s = project_settings(r, projects, CATEGORY_SETTING)
    
    # [['', ''], ['', '']]
    items = []
    for e in s.category.splitlines():
        if not e or e.startswith("#"): continue
        k, v = e.count(":") > 0 and e.split(":", 1) or (e, e)
        items.append("['%s', '%s']" % (k, v))
    
    settings_js = "{category:%s}" % ("[%s]" % ",".join(items)) 
    
    return settings_js

def trac(uuid, action='comment', text='', username='', bugid='', key=''):
    project = __load_project(key)
    if project is None:
        return {"status":"404", "message":"""the project key '%s' is not registered!""" % key}
    
    t = RobotTrac(parent=project, uuid=uuid, action=action, 
                  text=text, username=username, bugid=bugid)
    t.save()
    
    return {"status":"OK" }

def trac_list(r, uuid, offset=0, limit=10, key=''):
    project = __load_project(key)
    if project is None:
        return {"status":"404", "message":"""the project key '%s' is not registered!""" % key}

    trac = RobotTrac.all().ancestor(project).order("-create_date").filter('uuid =', uuid)
    if trac.count() == 0:
        return {"status":"404", "message":"Not found trac for '%s'" % uuid}
    else:
        l = ({'action': e.action, 
              'bugid': e.bugid, 
              'text': e.text,
              'user': e.username,
              'create_date':e.create_date} for e in trac.fetch(limit, offset))
        
        return {"status":"OK", "id":uuid, "trac":l}

def update(uuid, longname='', start_time='', end_time='', url='', error='', status=''):
    pass

def upload_build(r, key="", build_name=""):
    
    #from uploaded import UploadUtil
    project = __load_project(key)
    error = ""
    if project is not None:
        if r.method == 'POST':
            build = RobotUtils.import_test_build(r.FILES['file'], 
                                                 build_name, project)
            
            return ("redirect:/rf_trac/r/build?build=%s" % build.id, )
    else:
        error = """the project key '%s' is not registered!""" % key
    
    build_name = datetime.now().strftime('%Y%m%d%H%M%S')
    return ("rf_trac_prj_upload.html", {"prj_key": key, 
                                        "error": error,
                                        "build_name": build_name})


def default_view(r):
    project_list = RobotProject.all()
    return ("rf_trac_prj_list.html", {"prj_list": project_list})

def create_project(r, ):
    import uuid
    import hashlib
    if r.method == 'POST':
        f = r.REQUEST
        prj = RobotProject(name=f['name'])
        prj.admin_user = f['admin_user']
        prj.admin_password = f['admin_password']
        prj.prj_key = hashlib.md5(str(uuid.uuid4())).hexdigest()
        prj.put()
        
    return default_view(r)

def login_project(r, prj_name='', admin_user='', admin_password=''):
    
    project = RobotProject.all().filter('name =', prj_name).get()
    error = ""
    if r.method == 'POST':
        if project is not None:
            if project.admin_user != admin_user:
                error = "invalid user name"
            elif project.admin_password != admin_password:
                error = "invalid password"
            else:
                project_session(r, project.prj_key)
                return ("redirect:/rf_trac/v/index", )
            
            #project_session(r, project.prj_key)
            #return ("redirect:/rf_trac/v/index", )
        else:
            error = "invalid project name"
            
    return ("rf_trac_prj_login.html", {"error": error, "prj_name":prj_name})

def __load_project(key):
    if not key: return None
    
    project = memcache.get(key)
    if project is None: 
        project = RobotProject.all().filter('prj_key =', key).get()
        memcache.add(key, project, 60 * 60 * 2)
        
    return project

def main(r):
    return ("rf_trac_prj_main.html", )

def project_session(r, project=None, remove=None):
    from google.appengine.api import memcache
    
    sessionId = r.COOKIES.get('sessionid', None)
    if not sessionId: return None
    logging.debug("cur project session id:%s" % sessionId)
    
    if project is not None:
        logging.debug("save current project:%s" % project)
        memcache.add(key=sessionId, value=project, time=3600, namespace='rftrac')
    
    if remove is True:
        memcache.delete(key=sessionId, namespace='rftrac')
        r.COOKIES['sessionid'] = ''
        return None
    
    project = memcache.get(sessionId, namespace='rftrac')
    logging.debug("retrieve current project:%s" % project)
    return __load_project(project)
