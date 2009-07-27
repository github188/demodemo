
from google.appengine.ext import db
from models import *
from trac.settings import diff_settings
from diff.compare import diff_test_build

def build(r, build=''):
    #project = project_session(r)
    
    build = db.get(db.Key(build))
    
    if build is not None:
        log_list = RobotResult.all().filter("build =", build)
        return ("report/rf_trac_report_build.html", {"build":build,
                                          "log_list": log_list,
                                         });
    return ("redirect:/rf_trac/index", )

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
    
    settings = diff_settings(r, build.parent)
    diff_result = diff_test_build(build, settings)
    
    log_list = RobotResult.all().filter("build =", build)
    return ("report/rf_trac_report_build.html", {"build":build,
                                      "log_list": log_list,
                                      "diff_result":diff_result
                                     });
