
from views import project_session
from models import *
from trac import project_settings, DIFF_SETTING, CATEGORY_SETTING
from google.appengine.ext import db
from datetime import datetime

def build_list(r, offset=0, limit=10, name=''):
    project = project_session(r)
    if name and name != project.name:
        return ("redirect:/rf_trac/login_project?prj_name=%s" % name, )
    
    trac = RobotTrac.all().ancestor(project).order("-create_date")
    
    build_list = RobotTestBuild.all().ancestor(project)\
            .filter("status IN", ["", None,]).order("-create_date")
    
    return ("admin/rf_trac_admin_list.html", {"project":project,
                                        "trac_list": trac.fetch(limit, offset),
                                        "build_list": build_list.fetch(limit, offset)
                                        });

def settings(r, ):
    project = project_session(r)
    
    category_setting = project_settings(r, project, CATEGORY_SETTING)
    diff_setting = project_settings(r, project, DIFF_SETTING)
    if r.method == "POST":
        category_setting.save(r.POST)
        diff_setting.save(r.POST)
        
    diff_form = ""
    count = 0
    for e in diff_setting.items_order:
        check = diff_setting.value(e) == '1' and "checked" or ""
        help = diff_setting.help(e)
        diff_form += """<input id='%s' type='checkbox' name='%s' %s value='1'>
        <label for="%s">%s</label>&nbsp;&nbsp;""" % (e, e, check, e, help)
        count += 1
        if count % 4 == 0: diff_form += "<br/>"
            
    
    return ("admin/rf_trac_admin_settings.html", {"project":project,
                                        "category_setting": category_setting,
                                        "diff_form": diff_form
                                        });


def remove_build(r, build=""):
    #project = project_session(r)
    build = db.get(db.Key(build))
    
    build.status = 'removed'
    build.put()
    
    return ("redirect:/rf_trac/v/build_list", )

class F():
    def __init__(self, label, name, type, value='', text='', readonly=''):
        self.name = name
        self.type = type
        self.label = label
        self.text = text
        self.value = value
        self.readonly = readonly

def edit_build(r, build=""):
    #project = project_session(r)
    b = db.get(db.Key(build))
    
    form  = (F('Build name', 'build_name', 'text', b.build_name, readonly='readonly'),
             F('SUT name', 'sut_name', 'text', b.sut_name, 'test bed name or IP address, Meta:Sut Name'),
             F('SUT version', 'sut_version', 'text', b.sut_version),
             F('Relase version', 'sut_release', 'text', b.sut_release),
             F('Major version', 'sut_major', 'text', b.sut_major),
             F('Execute user', 'execute_user', 'text', b.execute_user),
             )
    
    if r.method == "POST":
        for n in ["build_name", "sut_name", "sut_version", 
                  "sut_release", "sut_major", "execute_user"]:
            if r.REQUEST.has_key(n):
                setattr(b, n, r.REQUEST[n])
        b.put()
        b.build_index()
        return ("redirect:/rf_trac/v/build_list", )
    
    return ("admin/rf_trac_build_edit.html", {"build": b, 
                                              "project": b.parent(),
                                              "form": form})    
    #return ("redirect:/v/build_list", )
def project_logging(r, ):
    project = project_session(r)
    logging = TracLogging.all().ancestor(project).order("-create_date")
    
    return ("admin/rf_trac_build_logging.html", {"logging": logging,
                                                 "project": project
                                                 })

def testcase_list(r, ):
    project = project_session(r)
    caselist = RobotTest.all().ancestor(project).order("testname")
    
    return ("admin/rf_trac_case_list.html", {"caselist": caselist,
                                             "project": project
                                             })

def default_view(r):
    return build_list(r)

    
def pre_handler(r, url):
    project = project_session(r)
    if project is None:
        return ("redirect:/rf_trac/login_project", )
    
    return None
