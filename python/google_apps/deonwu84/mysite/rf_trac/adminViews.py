
from views import project_session
from models import *

def trac_list(r, offset=0, limit=10, key=''):
    project = project_session(r)
    
    trac = RobotTrac.all().ancestor(project).order("-create_date")
    
    build_list = RobotTestBuild.all().ancestor(project).order("-create_date")
    
    
    return ("admin/rf_trac_admin_list.html", {"project":project,
                                        "trac_list": trac.fetch(limit, offset),
                                        "build_list": build_list.fetch(limit, offset)
                                        });

def default_view(r):
    return trac_list(r)

    
def pre_handler(r, url):
    project = project_session(r)
    if project is None:
        return ("redirect:/rf_trac/login_project", )
    
    return None
