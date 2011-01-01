
import os
from configobj import ConfigObj, Section, UnreprError

if os.name == 'nt':
    SETTINGS_DIRECTORY = os.path.join(os.environ['APPDATA'], 'Goku')
else:
    SETTINGS_DIRECTORY = os.path.expanduser('~/.Goku')
    
if not os.path.exists(SETTINGS_DIRECTORY):
    os.mkdir(SETTINGS_DIRECTORY)
    
class Settings(object):
        
    def __init__(self):
        self.path = os.path.join(SETTINGS_DIRECTORY, "goku.cfg")
        self._config_obj = ConfigObj(self.path, unrepr=True)
        
        self.set("APP_ROOT", SETTINGS_DIRECTORY, autosave=False)
        
    def __setitem__(self, name, value):
        self.set(name, value)
    
    def __getitem__(self, name):
        value = self._config_obj[name]
        return value
    
    def set(self, name, value, autosave=True, override=True):
        """Sets setting 'name' value to 'value'.
        
        'autosave' can be used to define whether to save or not after values are
        changed. 'override' can be used to specify whether to override existing 
        value or not. Setting which does not exist is anyway always created.
        """
        if name not in self._config_obj or override:
            self._config_obj[name] = value
            if autosave:
                self.save()
                
    def save(self):
        #print "filename:%s" % self._config_obj.filename
        self._config_obj.write()                
                
    def _is_section(self, name):
        return self._config_obj.has_key(name) and \
               isinstance(self._config_obj[name], Section)                

    