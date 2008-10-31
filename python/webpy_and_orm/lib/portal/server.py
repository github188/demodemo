import web
import sys
import os
from types import ModuleType, MethodType #, ClassType, MethodType

try:
    import pythonpathsetter
except ImportError:  
    pass
import config

urls = ('/(.*)', 'dispatcher')

class dispatcher:
	
	def GET(self, url):
		return self.__action__(url)

	def POST(self, url):
		return self.__action__(url)
	
	def parse_web_input(self, params):
		i = web.input()
		p = {}
		for name in params:
			if i.has_key(name):
				p[name] = i[name]
		return p
	
	def __action__(self, url):
		action, auto_fn, post_fn, args = web.ctx.mapping.url_mapping(web.ctx.path)
		if action != None:
			for fn in auto_fn:
				result = fn()
				if result is not None: return result
			
			kwargs = self.parse_web_input(action.kwargs_names)
			result = action(*args, **kwargs)
			
			for fn in post_fn: fn()
			return result
		elif self.static_handler(url):
			pass
		else:
			return self.notfound(web.ctx.path)
	def notfound(self, url):
		print "Not found url mapping for '%s'" % url
        
	def static_handler(self, url):
		static_file = os.path.join(config.web_root[0], * url.split('/'))
		import mimetypes
		if os.path.isfile(static_file):
			base, ext = os.path.splitext(static_file)
			web.header('Content-Type', mimetypes.types_map[ext])
			f = open(static_file, 'rb')
			try:
				web.output(f.read(-1))
				return True
			finally:
				f.close()
		
		return False        
    				
_mappings_pool_ = []

def _context_loader():
    if len(_mappings_pool_) > 0 and config.run_mode is not 'debug':
        web.ctx.mapping = _mappings_pool_.pop()
    else:
        web.ctx.mapping = UrlMapping('/', config.url_config[1])
    from portal.model import cur_pm
    cur_pm()

def _context_unloader():
	if config.run_mode is not 'debug':
		_mappings_pool_.append(web.ctx.mapping)
	web.ctx.mapping = None

class UrlMapping(object):
	def __init__(self, url, fn, parent=None):
		self.nexts = []
		self.auto_fn = None
		self.post_fn = None
		self.action = None
		self.url = url
		import re
		self.pattern = re.compile('^' + (url.endswith('/') and url + '(.*?)' or url) + '$')
		
		if type(fn) is ModuleType:
			self._load_module(fn)
		elif type(fn) is MethodType:
			self.action = MappingHandler(fn)
			
		print "mapping '%s'==>'%s'" % (url, str(fn))
	
	def url_mapping(self, url):
		return self._url_mapping(url, [], [])
		
	def _url_mapping(self, url, pre_fn, post_fn):
		
		matched, args = self._match_url(url)
		if matched:
			if self.auto_fn is not None:
				pre_fn.append(self.auto_fn)
			if self.post_fn is not None:
				post_fn.append(self.post_fn)
			
			for sub in self.nexts:
				mapping = sub._url_mapping(args[0], pre_fn, post_fn)
				if mapping[0] is not None:
					return mapping
			else:
				return (self.action, pre_fn, post_fn, args)
			
		else:
			return (None, None, None, None)
	
	def _match_url(self, url):
		result = self.pattern.match(url)
		return result and (result.group(), result.groups()) or (None, None)
	
	def _load_module(self, mod):
		cls_name = mod.__name__.split('.')[-1]
		cls_name = cls_name[0].upper() + cls_name[1:]
		cls = None
		try:
			cls = getattr(mod, cls_name)
		except AttributeError:
			pass
		if cls is not None: self._load_class(cls)
        
		try:
			sub_mod = os.listdir(mod.__path__[0])
			sub_mod = set( [ os.path.splitext(e)[0] for e in sub_mod if not e.startswith('_') ] )
			
			#filter file like '.svn' 
			sub_mod = [ e for e in sub_mod if e != '' ]
		except AttributeError:
			sub_mod = []
		
		for name in sub_mod:
			#base, ext = os.path.splitext(path)
			try:
				new_mod = __import__(mod.__name__ + '.' + name, globals(), locals(), [ name ])
			except:
				raise
				#print "can't import module:%s" % (mod.__name__ + '.' + name)
				#continue
			else:
				self.nexts.append(UrlMapping(name + '/', new_mod))
					
	def _load_class(self, cls):
		obj = cls()
		self._load_fixtrue(obj)
		
		urls = getattr(obj, "urls")
		import web
		for url, mapping in web.utils.group(urls, 2):
			mapping = isinstance(mapping, basestring) and mapping or mapping.__name__
			try:
				self.nexts.append(UrlMapping(url, getattr(obj, mapping)))
			except AttributeError:
				raise RuntimeError, "not found action for url(%s, %s)!" % (url, str(mapping))			
			
	def _load_fixtrue(self, obj):
		try:
			self.auto_fn = getattr(obj, "__auto__")
		except:
			pass
		try:
			self.post_fn = getattr(obj, "__post__")
		except:
			pass
		
class MappingHandler(object):
	def __init__(self, func):
		self.kwargs_names = []
		self.func = func
		self._parse_kwargs_names_from_doc(func.func_doc or '')
		
	def __call__(self, *args, **kwargs):
		return self.func(*args, **kwargs)
	
	def _parse_kwargs_names_from_doc(self, doc):
		""" 
		
		@param: track_id, ajax
		""" 
		for line in doc.split('\n'):
			if '@param:' in line:
				param = line.replace('@param:', '')
				self.kwargs_names = [ e.strip() for e in param.split(',') if e.strip() != '' ]
				return
	
		
_context_loader()
web.loadhooks['__portal_loader__'] = _context_loader
web.unloadhooks['__portal_unloader__'] = _context_unloader

def start_server():
    web.webapi.internalerror = web.debugerror
    print "start server....."
    if config.listen_port is not 0:
    	if len(sys.argv) > 1:
    		sys.argv[1] = str(config.listen_port)
    	else:
			sys.argv.append(str(config.listen_port))
	
	#config default database conncection that mananged by web
	#web.config.db_parameters = config.db_config['default']
    
    import logging
    logging.basicConfig(level=logging.DEBUG)
    
    web.run(urls, globals(), web.reloader)
      
if __name__ == "__main__":
    start_server()

