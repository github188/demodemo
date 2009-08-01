
from views import project_session
from models import *
from trac import project_settings, DIFF_SETTING, CATEGORY_SETTING

def trac_list(r, offset=0, limit=10, name=''):
    project = project_session(r)
    if name and name != project.name:
        return ("redirect:/rf_trac/login_project?prj_name=%s" % name, )
    
    trac = RobotTrac.all().ancestor(project).order("-create_date")
    
    build_list = RobotTestBuild.all().ancestor(project).order("-create_date")
    
    
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


def default_view(r):
    return trac_list(r)

    
def pre_handler(r, url):
    project = project_session(r)
    if project is None:
        return ("redirect:/rf_trac/login_project", )
    
    return None
