import logging
from google.appengine.ext import db
from models import *

def api(key=''):
    project = RobotProject.all().filter('prj_key =', key).get()
    if project:
        return ("rf_trac_prj_api.html", {"prj": project})
    else:
        return """alert("the project key '%s' is not registered!");""" % key

def trac(uuid, action='comment', text='', username='', bugid='', key=''):
    project = __load_project(key)
    if project is None:
        return {"status":"404", "message":"""the project key '%s' is not registered!""" % key}
    
    t = RobotTrac(parent=project, uuid=uuid, action=action, 
                  text=text, username=username, bugid=bugid)
    t.save()
    
    return {"STATUS":"OK" }

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

from google.appengine.api import memcache
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
