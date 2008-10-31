
class Model(object):
    def __init__(self, **kwargs):
        for f in self._pm_fields_:
            if kwargs.has_key(f[0]):
                setattr(self, f[0], kwargs[f[0]])
            else:
                setattr(self, f[0], None)
                
    
    def __repr__(self):
        _pm_var_ = {}
        for f in self._pm_fields_:
            _pm_var_[f[0]] = getattr(self, f[0])
        return repr(_pm_var_)