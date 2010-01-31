from rdb.interface import BaseDebugInterface

import BaseHTTPServer
import SimpleHTTPServer
from time import time
from datetime import datetime
from urlparse import urlparse
from robot.serializing import Template, Namespace
from rdb.debuger.breakpoints import KeywordBreakPoint
import logging, re, random, urllib2
from views import BreakPointView, CallStackView
from robot.running import NAMESPACES

class WebHandler(BaseHTTPServer.BaseHTTPRequestHandler):
    
    def do_GET(self, ):
        output = self.process()
        
        self.send_response(200)
        self.send_header("Content-type", "text/html")
        self.send_header("Cache-Control", "no-cache")
        self.send_header("Expires", self.date_time_string(time()))
        self.send_header("Last-Modified", self.date_time_string(time()))
        self.send_header("Content-Length", len(output))
        self.end_headers()
        
        self.wfile.write(output)
    
    def process(self):
        self.logger = logger = logging.getLogger("rdb.web")
        try:
            self.rbt_debuger = self.server.robot_debuger
            
            #output = "xx:%s,\n%s" % (self.server.debug_interface, self.path)
            url = urlparse(self.path)
            command_name = command = url.path.split("/")[1]
            params = self.__parse_param(url.query)
            if self.check_session(params):
                logger.debug("command:%s,%s" % (command, str(params)))
                if command and hasattr(self.rbt_debuger, command):
                    command = getattr(self.rbt_debuger, command)
                elif command and hasattr(self, command):
                    command = getattr(self, command)
                if callable(command):
                    status, msg = self.execute(command, params)
                else:
                    status, msg = ('ERR', "Not found command '%s'" % command)
            else:
                status = 'ERR'
                msg = 'debug session error'
            #logger.debug("command:%s,%s, %s" % (command, status, msg))
            
            output = self.response_views(command_name, status, msg)
            return output
        except BaseException, e:
            logger.exception(e)
            return "Error:%s" % e
    
    def refresh(self):return "status at %s" % datetime.now()
    
    def check_session(self, params):
        session = params.get("sid", "").strip()
        result = session and self.server.sid and session == self.server.sid
        self.server.sid = str(random.random())
        return result
    
    def log_message(self, format, *args):
        self.logger.debug(format % args)
    
    def response_views(self, command, status, msg):
        import templates as t
        
        reload(t)
        DEBUGER_TEMPLATE = t.DEBUGER_TEMPLATE
        
        debugCtx = self.rbt_debuger.debugCtx
        
        #break porints
        break_points = [ BreakPointView(e) for e in debugCtx.break_points 
                            if isinstance(e, KeywordBreakPoint) ]
        for e in break_points:
            if e.obj == debugCtx.active_break_point:
                e.css_class = 'active'
                
        #call stack
        call_stack = [ CallStackView(e) for e in debugCtx.call_stack ]
        if debugCtx.active_break_point:
            call_stack[-1].css_class = 'active'
        call_stack.reverse()
        
        #listener attrs
        if len(call_stack) > 0:
            listener_attrs = call_stack[0].attrs
        else:
            listener_attrs = {}
        keys = listener_attrs.keys()
        attr_order = ['doc', 'longname', 'starttime', 'endtime', 'elapsetime',
                      'tags', 'status', 'message', 'statistics']
        index_attr = lambda x: x not in attr_order and 999 or attr_order.index(x)
        keys.sort(lambda x, y:cmp(index_attr(x), index_attr(y)))
        cur_attrs = []
        for e in keys:
            o = lambda:0
            o.name = e
            o.value = listener_attrs[e]
            cur_attrs.append(o)
            
        #varibles
        varibles = listener_attrs.get('args', [])
        cur_varibles = []
        robot_vars = NAMESPACES.current.variables        
        args_varibles = [e for e in varibles if e[0] in ['$', '@']]
        for e in args_varibles + self.rbt_debuger.watching_variable:
            o = lambda:0
            o.name = e
            try:
                o.value = robot_vars.replace_string(e)
                o.value = re.sub(r"([^ <>]{50})",
                                 lambda r: "<span>%s</span> " % r.group(0),
                                 o.value)
            except Exception, e:
                if "Non-existing" in str(e):
                    o.value = "<STRIKE>Non-existing variable</STRIKE>"
                else: raise
            cur_varibles.append(o)
            
        #robot status
        robot_status = lambda:0
        if debugCtx.active_break_point is not None:
            if hasattr(debugCtx.active_break_point, 'kw_name'):
                robot_status.name = "Paused at keyword breakpoint '%s'" % \
                    debugCtx.active_break_point.kw_name
            else:
                robot_status.name = "Paused by step debug."
            robot_status.css_class = "paused"
        else:
            robot_status.name = "Running....."
            robot_status.css_class = "running"
            
        #refresh time
        refresh_interval = debugCtx.active_break_point and "120" or "1"
        
        namespace = Namespace(call_stack=call_stack,
                              break_points=break_points,
                              command=command,
                              command_result=status,
                              msg=msg,
                              title="Robot framework web debuger",
                              robot_status=robot_status,
                              cur_attrs=cur_attrs,
                              cur_varibles=cur_varibles,
                              refresh_interval=refresh_interval, 
                              session=self.server.sid
                              )
        
        return Template(template=DEBUGER_TEMPLATE).generate(namespace)
    
    def execute(self, command, params):        
        if command.func_defaults:
            reqiured_args_count = command.func_code.co_argcount - len(command.func_defaults)
        else:
            reqiured_args_count = command.func_code.co_argcount
            
        var_names = command.func_code.co_varnames
        reqiured_args, options_args = var_names[:reqiured_args_count], var_names[reqiured_args_count:]
            
        try:
            args, kw_args = self.__parse_args(params, reqiured_args, options_args)
            result = command(*args, **kw_args)
        except Exception, e:
            return ('ERR', str(e))
        
        return ('OK', result and str(result) or '')
    
    def __parse_param(self, param):
        p = {}
        for e in param.split("&"):
            if "=" not in e: continue
            k, v = e.split("=", 1)
            p[k] = urllib2.unquote(v)
        return p
    
    def __parse_args(self, args, reqiured_args, options_args):
        param = []
        for name in reqiured_args:
            if 'self' == name:continue         
            if not args.has_key(name):
                raise RuntimeError, "Not found required parameter '%s'" % name
            param.append(args[name])
        kw_param = {}    
        for name in options_args:
            if args.has_key(name):
                kw_param[str(name)] = args[name]
        return (param, kw_param)

class WebDebuger(BaseDebugInterface):
    def start(self, cfg):
        server_address = (cfg.WEB_BIND, int(cfg.WEB_PORT))
        httpd = BaseHTTPServer.HTTPServer(server_address, WebHandler)
        httpd.robot_debuger = self
        httpd.sid = ""
        
        self.logger.info("starting web interface, binding address=%s:%s..." %(cfg.WEB_BIND,
                                                                              cfg.WEB_PORT))
        httpd.serve_forever()
        
    
if "__main__" == __name__:
    x = lambda:0
    x.WEB_PORT = 8000
    print "start server..."
    WebDebuger().start(x)
    