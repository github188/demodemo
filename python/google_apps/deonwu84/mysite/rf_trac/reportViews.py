
from google.appengine.ext import db
from models import *

def build(r, build=''):
    #project = project_session(r)
    
    build = db.get(db.Key(build))
    
    if build is not None:
        log_list = RobotResult.all().filter("build =", build)
        return ("rf_trac_report_build.html", {"build":build,
                                          "log_list": log_list,
                                         });
    return ("redirect:/rf_trac/index", )