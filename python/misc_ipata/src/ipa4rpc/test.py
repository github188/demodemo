"""
this module provide some help class for unittest.
"""

from time import time
import os,urllib2, BaseHTTPServer, SimpleHTTPServer
from rpc_over_http import RPCStub

class SimpleRPCHandler(BaseHTTPServer.BaseHTTPRequestHandler):
    def do_POST(self, ):
        #data = self.rfile.read()
        
        length = self.headers['Content-Length'] #int(os.environ['CONTENT_LENGTH'])
        
        data = self.rfile.read(int(length))
        param = self.__parse_param(data)        
        output = self.server.rpc_service(param['method'], param['params'])
        
        self.send_response(200)
        self.send_header("Content-type", "application/octet-stream")
        self.send_header("Cache-Control", "no-cache")
        self.send_header("Expires", self.date_time_string(time()))
        self.send_header("Last-Modified", self.date_time_string(time()))
        self.send_header("Content-Length", len(output))
        self.end_headers()
        
        self.wfile.write(output)
    def do_GET(self):
        return self.do_POST()
    
    def __parse_param(self, param):
        p = {}
        for e in param.split("&"):
            if "=" not in e: continue
            k, v = e.split("=", 1)
            p[k] = urllib2.unquote(v)
        return p    
    
class SimpleRPCServer(object):
    def __init__(self, rpc_services):
        self._rpc = RPCStub('', rpc_services(), )
    
    def start(self):
        self.httpd = BaseHTTPServer.HTTPServer(("127.0.0.1", 0), SimpleRPCHandler)
        self.httpd.rpc_service = self.rpc_service
        self.server_port = self.httpd.server_port
        print "start test rpc server at:%s\n" % self.server_port
        import thread, time
        thread.start_new_thread(self.httpd.serve_forever, ())
        #self.httpd.serve_forever()
        time.sleep(1)
        return self.server_port
    
    def rpc_service(self, *arg, **kw):
        return self._rpc(*arg, **kw)
        
    def close(self):
        print "close rpc server..."
        self.httpd.server_close()
