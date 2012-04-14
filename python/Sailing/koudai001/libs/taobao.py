import hashlib
from time import localtime, strftime
import os
import json
import urllib
#os.environ['TZ'] = 'Asia/Shanghai'
#tzset()
#'2012-04-14 10:03:00'


class Taobao(object):
    def __init__(self, app_id, app_secret, http=None, host='http://gw.api.taobao.com/router/rest?'):
        #http://gw.api.taobao.com/router/rest
        #'http://gw.api.tbsandbox.com/router/rest?'
        self.app_id = app_id
        self.app_secret = app_secret
        self.http = http or self._default_http()
        self.param = {'app_key': app_id,  
                      'app_secret': app_secret,
                      }
        self.host = host
    
    def __getattr__(self, name):
        if not name.startswith("__"):
            name = name.replace("_", ".")
            return CallProxy(name, self._api_param_order(name), self.param, self.http, self.host)
        else:
            raise AttributeError("Not found attr '%s'" % name)
        
    def _api_param_order(self, name):
        return {
            'taobao.itemcats.get': ['fields', 'parent_cid', 'cids'],
            'taobao.item.get': ['fields', 'num_iid'],
        }.get(name, [])
        
    def _default_http(self, ):
        class httpClient(object):
            def get(self, url):
                data = None
                req = urllib.urlopen(url)
                data = req.read()
                return data
        return httpClient()
        
class CallProxy(object):
    
    def __init__(self, name, app_order, sys_param, http, host):
        self.sys_order = ['method', 'session', 'timestamp', 'format', 'app_key', 'v', 'sign', 'sign_method']
        self.app_order = app_order
        self.sys_param = sys_param
        self.http = http
        self.host = host
        self.sys_param['method'] = name
        
    def __call__(self, **kw):
        self.sys_param['timestamp'] = strftime("%Y-%m-%d %H:%M:%S", localtime())
        self.sys_param['v'] = '2.0'
        self.sys_param['sign_method'] = 'md5'
        self.sys_param['format'] = 'json'
        
        sign = self._get_sign(self.sys_param, kw)        
        str_param = self._create_str_param(self.sys_param, kw)
        str_param = '%s&sign=%s' % (str_param, sign)     
        api_url = '%s%s' % (self.host, str_param)
                
        #timestamp = '';
        #yyyy-MM-dd HH:mm:ss
        #print "------------------"
        #print api_url
        #print "-------------------"
        resp = self.http.get(api_url)
        #print resp
        #print "-------------------"
        
        data = json.loads(resp)
        if data.get('error_response'):
            raise Exception(str(data.get('error_response')))
        return data
        
    def _get_sign(self, sys_param, app_param):
        b = dict(sys_param)
        b.update(app_param)
        
        param_order = self.sys_order + self.app_order
        param_order.sort()
        params = [ "%s%s" % (e, b.get(e)) for e in param_order if str(b.get(e, '')).strip() ]
        
        secret = sys_param['app_secret']
        str_param = "".join(params)
        str_param = "%s%s%s" % (secret, str_param, secret,)
        #print "xx:%s" % str_param
        
        return hashlib.md5(str_param).hexdigest().upper()
        
        
    def _create_str_param(self, sys_param, app_param):
        b = dict(sys_param)
        b.update(app_param)
        
        param_order = self.sys_order + self.app_order
        
        
        params = [ "%s=%s" % (e, urllib.quote(str(b.get(e)))) for e in param_order if str(b.get(e, '')).strip() ]
        
        str_param = "&".join(params)
        return str_param
        
        
        
        