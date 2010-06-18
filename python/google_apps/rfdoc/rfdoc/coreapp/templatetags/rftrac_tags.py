from django import template
register = template.Library()

from mysite.rf_trac.trac import project_settings, CATEGORY_SETTING
from mysite.rf_trac.models import *

from google.appengine.api import memcache

@register.filter(name='category_status')
def category_status(t, *arg, **kwargs):
    category_list = __load_category_settings(t.parent())
        
    trac = RobotTrac.get_trac(t)
    category = trac is not None and trac.action or "" 
    option = ""
    for v, label in category_list:
        selected = v == category and "selected" or ""
        option += "<option %s value='%s'>%s</option>" % (selected, v, label)
    
    return option

@register.filter(name='category_label')
def category_label(value, p,*arg, **kwargs):
    category_list = __load_category_settings(p)
    for v, label in category_list:
        if value == v: return label
    return "unknown"

def __load_category_settings(project):    
    cached_key = "category_list_%s" % project.prj_key
    category_list = memcache.get(cached_key, namespace='rftrac')
    if category_list is None:
        s = project_settings(None, project, CATEGORY_SETTING)
        category_list = []
        for e in s.category.splitlines():
            if not e or e.startswith("#"): continue
            k, v = e.count(":") > 0 and e.split(":", 1) or (e, e)
            category_list.append((k, v))
        memcache.add(key=cached_key, value=category_list, time=3600, namespace='rftrac')
    return category_list
    