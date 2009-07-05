import types
from django.http import HttpResponse, HttpResponseRedirect
from django.shortcuts import render_to_response
from django.db.models.query import QuerySet

class ViewHandler(object):
    def __init__(self, func):
        self.target = func
        if func.func_defaults:
            reqiured_args_count = func.func_code.co_argcount - len(func.func_defaults)
        else:
            reqiured_args_count = func.func_code.co_argcount
            
        var_names = func.func_code.co_varnames
        self.reqiured_args = var_names[:reqiured_args_count]
        self.options_args = var_names[reqiured_args_count:]
        
    def __parse_args(self, r, args):
            
        param = []
        for name in self.reqiured_args:
            if name in ['r', 'request']:
                param.append(r)
                continue
            if not args.has_key(name):
                raise Exception, "Not found required parameter '%s'" % name
            param.append(args[name])
        
        kw_param = {}
        for name in self.options_args:
            if args.has_key(name):
                kw_param[str(name)] = args[name]
        
        return (param, kw_param)    
    
    def __call__(self, r):
        param, kw_args = self.__parse_args(r, r.REQUEST)
        return self.target(*param, **kw_args)

class SimpleViews(object):
    
    def __init__(self, view):
        self.view = self.__import_views(view)
        self.cached = {}
    
    def __call__(self, request, url):
        h = None
        if self.cached.has_key(url):
            h = self.cached[url]
        else:
            obj = getattr(self.view, url)
            h = ViewHandler(obj)
            self.cached[url] = h
        
        return self.result_router(h(request))
            
    def result_router(self, r):
        if isinstance(r, HttpResponse):
            return r
        elif isinstance(r, types.TupleType):
            if isinstance(r[0], QuerySet):
                return HttpResponse(serializers.serialize("mixed_json", r))
            elif len(r) == 1 and str(r[0]).startswith("http:"):
                return HttpResponseRedirect(r[0])
                
            return render_to_response(*r)
        elif isinstance(r, basestring):
            return HttpResponse(r)
        else:
            return HttpResponse(serializers.serialize("mixed_json", r))
        
    def __import_views(self, v):
        
        if isinstance(v, types.ModuleType):
            return v
        elif isinstance(v, basestring):
            return __import__(v, globals(), locals(), ["__name__"], -1)
        elif isinstance(v, types.ClassType):
            return v()
        
        raise Exception, "Not supported views '%s'." % v

class SimpleUrl(SimpleViews):
    
    def __init__(self, handler):
        self.target = ViewHandler(self.__import_url(handler))
    
    def __call__(self, request,):
        return self.result_router(self.target(request))
    
    
    def __import_url(self, v):
        if isinstance(v, basestring):
            handler = v.split(".")
            module_name, handler = ".".join(handler[:-1]), handler[-1]
            module = __import__(module_name, globals(), locals(), [handler, ], -1)
            return getattr(module, handler)
        
        raise Exception, "Not supported views '%s'." % v    
    
