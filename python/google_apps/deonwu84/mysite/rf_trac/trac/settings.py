
#def settings():
from models import ProjectSetting

SETTINGS = {
    "diff":(("sut_name", "0", "sut name"),
            ("sut_version", "0", "sut version"),
            ("sut_release", "0", "sut release"),
            ("sut_major", "0", "sut major"),
            ("execute_user", "0", "execute user"),
            ("last_time", "0", "last time"),
            ("single_build", "0", "single build"),
            ),
}

def diff_settings(r, project):    
    return Setting(r, project, "diff", SETTINGS["diff"])
    

class Setting(object):
    def __init__(self, r, project, name="", items):
        self.project = project
        self.request = r
        self.name = name
        
        self.items = self.__default_value(items)
        self.items_order = []
        self.db = {}
        self.cookies = {}
    
    def __default_value(self, items):
        for e in items:
            name, v = e.pop(0), e.pop(1)
            self.items[name] = v
            self.items_order.append(name)
            
    def __load_db(self, ):
        for e in ProjectSetting.all().ancestor(self.project).\
                filter("name =", self.name).fetch():
            self.db[e.item_name] = e.item_value
    
    def __retirve_cookies(self, ):
        pass
    
    def __getattr__(self, name):
        if name not in self.items_order:
            raise AttributeError, "Setting '%s' have not include item '%s'" %(self.name, name)
        if self.cookies.has_key(name):
            return self.cookies[name]
        elif self.db.has_key(name):
            return self.db[name]
        else:
            return self.items[name]
    
    