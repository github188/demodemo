
from sailing.common.ctlDaemon import ControllableDaemon
from sailing.common.logger import Logger
from sailing.common.common import *
from sailing.conf import settings as CONFIG
import glob
import os

class Application(ControllableDaemon):
    
    def __init__(self):
        
        ControllableDaemon.__init__(self, '%s daemon' % CONFIG.APP_NAME, CONFIG.RUNNING_FLAG, CONFIG.CLOSING_FLAG)
            
    def prepare(self):
        logger = Logger(name=CONFIG.LOG_NAME, path=CONFIG.LOG_PATH, 
                        level=CONFIG.LOG_LEVEL, size=CONFIG.LOG_SIZE, rotation=CONFIG.LOG_ROTATION, 
                        notifier=CONFIG.LOG_NOTIFIER, notification_level=CONFIG.LOG_NOTIFICATION_LEVEL)
        self.logger = logger
        
        self.logger.info("Starting '%s' on directory '%s'." % (CONFIG.APP_NAME, CONFIG.DATA_ROOT))
        if not exists_path(CONFIG.APP_NAME):
            self.logger.info("Not found application directory, '%s'" % (CONFIG.APP_NAME))
            make_path(CONFIG.APP_NAME)
            self.logger.info("Create directory, '%s'" % (CONFIG.APP_NAME))
        
        try:
            module = __import__('sailing.%s' % CONFIG.APP_NAME, globals(), locals(), ['main', ], -1)
            self.command = getattr(module, 'main')
        except:
            self.logger.info("Could not find function 'sailing.%s.main'." % CONFIG.APP_NAME)
            self.command = None
        
        if not callable(self.command): raise Exception("Not found application '%s'" % CONFIG.APP_NAME)
        
        
    def run(self):
        from sailing.core import change_status   
        for fn in self.list_file("waiting"):
            self.command(change_status(fn, 'running'))
            
        self.logger.info('Waiting %d seconds for next round' % CONFIG.POLLING_INTERVAL)
        sleep(CONFIG.POLLING_INTERVAL)

    def list_file(self, type, len=100):
        return glob.glob(join_path(CONFIG.APP_NAME, '*.%s' % type))[:len]
    
    
    
    
    
    
    
    
    
    
    
    
     