import logging
from google.appengine.ext import db
from models import *

def api(key=''):
    project = RobotProject.all().filter('prj_key =', key).get()
    if project:
        return ("rf_trac_prj_api.html", {"prj": project})
    else:
        return """alert("the project key '%s' is not registered!");""" % key

def trac(uuid, text='', username='', key=''):
    if request.method == 'GET':
        trac = RobotTrac.all().filter('prj_key =', key).get()
    else:
        pass

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
        
    return prj.prj_key
    
def login_project(r, prj_id=''):
    pass
