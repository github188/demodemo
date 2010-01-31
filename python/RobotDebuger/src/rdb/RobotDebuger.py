from __future__ import with_statement
from contextlib import closing
import os

from debuger.debuger import Debuger
from debuger.breakpoints import KeywordBreakPoint
from threading import Thread
import logging

class DebugSetting(object):
    def __init__(self, path=None):
        import pkg_resources        
        logging_config = pkg_resources.resource_filename('rdb', "settings.rdb")
        self.load_from_file(logging_config)
        
        self.BPS_LIST = []
        self.WATCHING_LIST = []
    
    def load_from_file(self, path):
        with closing(open(path, 'r')) as records:
            for l in records:
                l = l.strip()
                if l.startswith("#") or not l:continue
                if "=" in l:
                    name, value = l.split("=", 1)
                    name, value = name.strip(), value.strip()
                if name.endswith("LIST"):
                    if(hasattr(self, name)):
                        getattr(self, name).append(value)
                    else:
                        setattr(self, name, [value, ])
                else:
                    setattr(self, name, value)

class DebugThread(Thread):
    def __init__(self, interface, args):
        Thread.__init__(self)
        self.setDaemon(True)
        self.interface = interface
        self.args = args
    
    def run(self):
        self.interface.start(self.args)

class RobotDebuger(object):
    def __init__(self, settings):
        self.bp_id = 0
        self.debugCtx = Debuger()
        self.settings = DebugSetting()
        self.logger = None
        self.watched_variable = []
        
        if settings.endswith('.rdb'):
            self.settings.load_from_file(os.path.abspath(settings))
        else:
            self.add_breakpoint(settings)
        
        if self.settings.LOGGING_FILE:
            self.__init_sys_logging()
            self.logger = logging.getLogger("rdb.c")
            self.logger.info("starting robot debuger...")
            
        for e in self.settings.BPS_LIST:
            self.add_breakpoint(e)
        
        for e in self.settings.WATCHING_LIST:
            self.watch_variable(e)
            
    def add_breakpoint(self, bps):
        self.debugCtx.add_breakpoint(KeywordBreakPoint('bp%s' % self.bp_id,
                                                       bps))
        self.bp_id += 1
        #self.debugCtx.add_breakpoint(KeywordBreakPoint('', bps))
        
    def watch_variable(self, name):
        if name not in self.watched_variable:
            self.watched_variable.append(name)
    
    def remove_variable(self, name):
        if name in self.watched_variable:
            self.watched_variable.remove(name)
    
    @property
    def watching_variable(self):return list(self.watched_variable)      
        
    def run(self):
        try:
            for cls_int in self.settings.INTERFACE_LIST:
                handler = cls_int.split(".")
                module_name, handler = ".".join(handler[:-1]), handler[-1]
                module = __import__(module_name, globals(), locals(), [handler, ], -1)
                cls_int = getattr(module, handler)
                DebugThread(cls_int(self), self.settings).start()
        except Exception, e:
            self.logger and self.logger.exception(e)
            raise
            
    def __init_sys_logging(self):
        level = getattr(logging, self.settings.LOGGING_LEVEL)
        logging.basicConfig(level=level,
                            format='%(asctime)s %(name)-8s %(levelname)-6s %(message)s',
                            datefmt='%m-%d %H:%M:%S',
                            filename=self.settings.LOGGING_FILE,
                            filemode='w')
    
    def close(self):
        self.logger.info("robot debuger is closed.")
    