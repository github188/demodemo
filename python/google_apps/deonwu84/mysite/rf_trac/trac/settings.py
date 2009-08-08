
#def settings():
import logging
from mysite.rf_trac.models import ProjectSetting
class SettingItem():
    def __init__(self, name, value, desc, type):
        self.name = name
        self.value = value
        self.desc = desc
        self.type = type
_ = SettingItem

SETTINGS = {
    "diff":(_("sut_name", "0", "sut name", 'boolean'),
            _("sut_version", "0", "sut version", 'boolean'),
            _("sut_release", "0", "sut release", 'boolean'),
            _("sut_major", "0", "sut major", 'boolean'),
            _("execute_user", "0", "execute user", 'boolean'),
            #_("last_time", "0", "last time", 'boolean'),
            _("single_build", "0", "single build", 'boolean'),
            ),
    "category":(
            _("category", "xx\n01:xx01\n02:xx02\n", "category", 'text'),
            )
}

DIFF_SETTING = "diff"
CATEGORY_SETTING = "category"

def client_settings(r, project, name):
    sessionId = r and r.COOKIES.get('sessionid', "") or ""
    return __load_settings(sessionId, project, name)

def project_settings(r, project, name):
    return __load_settings("", project, name) #Setting(r, project, name, SETTINGS[name])

def __load_settings(key, project, name):
    from google.appengine.api import memcache
    setting_key = "%s_%s_%s" % (key, project.prj_key, name)
    
    settings = memcache.get(setting_key, namespace='rftrac')
    if settings is None:
        settings = Setting(None, project, name, SETTINGS[name])
        memcache.add(key=setting_key, value=settings, time=3600, namespace='rftrac')
        logging.debug("create setting:%s" % setting_key)
    else:
        logging.debug("hit settings in cache:%s" % setting_key)
    
    return settings

class Setting(object):
    def __init__(self, r, project, name, items):
        self.project = project
        self.request = r
        self.name = name
        
        self.items = {}
        self.items_order = []
        #a alias
        self.settings = self.items_order 
        
        self.db = {}
        self.cookies = {}
        self.__default_value(items)
        self.__load_db()
        #self.__retirve_cookies()
    
    def __default_value(self, items):
        for e in items:
            self.items[e.name] = e
            self.items_order.append(e.name)
            
    def __load_db(self, ):
        for e in ProjectSetting.all().ancestor(self.project).\
                filter("name =", self.name):
            self.db[e.item_name] = e.item_value
    
    def __retirve_cookies(self, ):
        if self.request is None: return
        for n in self.items_order:
            if self.request.REQUEST.has_key(n):
                self.cookies[n] = self.request.REQUEST[n]
            elif self.cookies.has_key(n):
                del self.cookies[n]
    
    def __getattr__(self, name):
        if name.startswith("__") or name not in self.items_order:
            raise AttributeError, "Setting have not include item '%s'" %(name)
            #raise AttributeError, "Setting '%s' have not include item '%s'" %(self.name, name)
        if self.cookies.has_key(name):
            return self.cookies[name]
        elif self.db.has_key(name):
            return self.db[name]
        else:
            return self.items[name].value
    
    def set_value(self, param, all=""):
        for n in self.items_order:
            if param.has_key(n):
                self.cookies[n] = param[n]
            elif all:
                self.cookies[n] = ""
            elif self.cookies.has_key(n):
                del self.cookies[n]
    
    def setting_type(self, name):
        if self.items.has_key(name):
            return self.items[name].type
        
    def value(self, name):
        return getattr(self, name)
    
    def help(self, name):
        if self.items.has_key(name):
            return self.items[name].desc
    
    def save(self, values):
        for e in self.items.values():
            if e.type == 'boolean':
                e.value = values.get(e.name, "0")
            elif e.type == 'text':
                e.value = values.get(e.name, "")
            
            item = ProjectSetting.all().ancestor(self.project).\
                    filter("name =", self.name).\
                    filter("item_name =", e.name).get()
            if item is None:
                item = ProjectSetting(parent=self.project,
                                      name=self.name,
                                      item_name=e.name,
                                      )
            item.item_value = e.value
            item.put()
            self.db[item.item_name] = item.item_value  
    