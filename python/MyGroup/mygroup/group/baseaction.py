
class BaseAction(object):
    
    def __init__(self):
        self.url_mapping = {}
    
    def root(r, url):
        handler = handler_mapping(r, url)
        
        if handler.func_defaults:
            reqiured_args_count = handler.func_code.co_argcount - len(handler.func_defaults)
        else:
            reqiured_args_count = handler.func_code.co_argcount
            
        var_names = handler.func_code.co_varnames
        reqiured_args, options_args = var_names[:reqiured_args_count], var_names[reqiured_args_count:]
            
        params = r.REQUEST
        #params['r'] = r
        
        args, kw_args = _parse_args(r, params, reqiured_args, options_args)
        url_args = self.url_parameter(url)
        if url_args: args[:len(url_args)] = url_args
    
        return result_router(handler(*args, **kw_args))
    
    def handler_mapping(r, url):
        handler = self.url_mapping.get(url, self.no_handler)
        
        return handler
    
    def url_parameter(self, url):
        return ()
    
    def no_handler(self, url):
        return "Not found handler for url!"
        
    def _parse_args(r, args, reqiured_args, options_args):
            
        param = []
        for name in reqiured_args:
            if name in ['r', 'request']:
                param.append(r)
                continue
            
            if not args.has_key(name):
                param.append(None)
            else:
                param.append(args[name])
        
        kw_param = {}
        #if 'param' in options_args: options_args = args.keys()    
        for name in options_args:
            if args.has_key(name):
                kw_param[str(name)] = args[name]
        return (param, kw_param)
    
    def result_router(r):
        
        if isinstance(r, HttpResponse):
            return r
        elif isinstance(r, types.TupleType):
            return render_to_response(*r)
        elif isinstance(r, basestring):
            return HttpResponse(r)
        else:
            return HttpResponse(r)
            
