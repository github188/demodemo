
from google.appengine.ext import db
from models import *
from trac import diff_test_build, client_settings, DIFF_SETTING
from trac import project_settings, CATEGORY_SETTING
from google.appengine.api import memcache

import logging

def build(r, build=''):
    #project = project_session(r)
    
    return diff(r, build)

def diff(r, build="", ajax='no'):
    """
    diff with history build:
    2. robot build
    2. base line
        
    1. sut name
       sut version
    3. sut release
    4. execute by
    
    last  cur
    PASS/FAIL->unknown  ---> unknown --> white
    
    FAIL->PASS  ---> info --> blue
    FAIL->FAIL  ---> warning --> yellow
    PASS->FAIL  ---> fatal--> red
    PASS->PASS  ---> ignore --> dark
        
    """

    build = db.get(db.Key(build))
    if build is None: return ("redirect:/rf_trac/index", )
    
    settings = client_settings(r, build.parent(), DIFF_SETTING)
    if r.REQUEST.has_key('submit'): 
        settings.set_value(r.REQUEST, all='Y')
    else:
        settings.set_value({})
        
    diff_result = diff_test_build(build, settings)
    
    s = project_settings(r, build.parent(), CATEGORY_SETTING)
    
    category_items = []
    for e in s.category.splitlines():
        if not e or e.startswith("#"): continue
        k, v = e.count(":") > 0 and e.split(":", 1) or (e, e)
        category_items.append("<option value='%s'>%s</option>" % (k, v))
    
    count = 0
    diff_form = ""   
    diff_setting = settings
    for e in diff_setting.items_order:
        check = diff_setting.value(e) == '1' and "checked" or ""
        help = diff_setting.help(e)
        diff_form += """<input id='%s' type='checkbox' name='%s' %s value='1'>
        <label for="%s">%s</label>&nbsp;&nbsp;""" % (e, e, check, e, help)
        count += 1
        #if count % 4 == 0: diff_form += "<br/>"        
    
    log_list = RobotResult.all().filter("build =", build)
    
    log_list = [e for e in log_list]
    for e in log_list:
        e.diff = diff_result[e.uuid]
    
    return ("report/rf_trac_report_build.html", {"build":build,
                                      "log_list": log_list,
                                      "category_list":"".join(category_items),
                                      "diff_form": diff_form,
                                     });

def log_st(r, uuid=''):
    log = RobotResult.all().filter("uuid =", uuid).get()
    
    st = {}
    if log is not None:
        b = log.build
        for e in ['build_name', 'sut_name', 'sut_version', 'execute_user']:
            st[e] = getattr(b, e)
        st['starttime'] = log.starttime
        st['trac_status'] = "unknown"
        st['elapsed'] = "elapsed"
            
    return st
    
def case(r, id="", key=""):
    project = __load_project(key)
    if project is None:
        return ("redirect:/rf_trac/", )
    
    case_list = RobotTest.all().ancestor(project).filter("reportid =", id)
    log_list = RobotResult.all().ancestor(project).filter("reportid =", id)
    log_list = [e for e in log_list]
    for l in log_list: l.trac_list = RobotTrac.get_trac_list(l, 10)
    
    return ("report/rf_trac_report_case.html", {"project":project,
                                      "log_list": log_list,
                                      "case_list": case_list
                                     });
    
def __load_project(key):
    if not key: return None
    
    project = memcache.get(key)
    if project is None: 
        project = RobotProject.all().filter('prj_key =', key).get()
        memcache.add(key, project, 60 * 60 * 2)
        
    return project
    
def rebuild_index(r):
    logging.info("start rebuild all test build index....")
    for b in RobotTestBuild.all():
        b.build_index()
    logging.info("done to index all test build.")
    return "done to index all test build."

from trac.utils import urlread
def plug_js(r, key=''):
    project = RobotProject.all().filter('prj_key =', key).get()
    if project:
        host = (r.META["SERVER_NAME"])
        port = (r.META["SERVER_PORT"])
        port = port != '80' and ":%s" % port or ""
        endpoint = "http://%s%s/rf_trac/" % (host, port)
        
        if host.count("localhost") > 0 or host.count("127.0.0.1") > 0:
            return ("report/rf_trac_js_loader.html", {"project": project,
                                             "endpoint": endpoint,
                                             })
        
        cached_key = "trac_report_js"
        
        plugin_js = memcache.get(cached_key, namespace='rftrac')
        if not plugin_js or host.count("localhost"):
            jscdn = host.count("deonwu84") and "jsicdn01.appspot.com" or "localhost:8888"
            plugin_js = urlread("http://%s/cdn/jQuery/jquery-1.2.6.min.js" % jscdn)
            plugin_js += urlread("http://%s/cdn/jQuery/jquery-ui.js" % jscdn)
            plugin_js += urlread("http://%s/cdn/=rftrac:setup_diff_reporting" % jscdn)
            memcache.add(key=cached_key, 
                         value=plugin_js, 
                         time=3600, namespace='rftrac')
                
        plugin_js += """;setup_diff_reporting("%s", "%s");""" %(endpoint, 
                                                                key,
                                                                )
        
        return ("text/javascript", plugin_js)
    else:
        return """alert("the project key '%s' is not registered!");""" % key
    