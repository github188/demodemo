
from google.appengine.ext import db
from models import *
from trac import diff_test_build, client_settings, DIFF_SETTING
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
    
    settings = client_settings(r, build.parent, DIFF_SETTING)
    diff_result = diff_test_build(build, settings)
    
    log_list = RobotResult.all().filter("build =", build)
    
    log_list = [e for e in log_list]
    for e in log_list:
        e.diff = diff_result[e.uuid]
    
    return ("report/rf_trac_report_build.html", {"build":build,
                                      "log_list": log_list,
                                     });
