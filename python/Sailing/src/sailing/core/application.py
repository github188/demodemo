
from sailing.common.ctlDaemon import ControllableDaemon
from sailing.common.logger import Logger
from sailing.common.common import *
from sailing.common.utils import import_class
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
        
        sailor_name = "sailing.%s.%s" % (CONFIG.APP_NAME, CONFIG.APP_NAME.capitalize())
        sailor_cls = import_class(sailor_name)
        
        if sailor_cls is None: raise Exception("Not found application '%s'" % sailor_name)
        self.sailor = sailor_cls()
        self.sailor.ready()
        
    def run(self):
        from sailing.core import FileTask
        task_list = FileTask.search(CONFIG.APP_NAME, "waiting", len=5)
        
        if len(task_list) == 0:
            self.sailor.idle()
            self.logger.info('Waiting %d seconds for idle action' % CONFIG.IDLE_INTERVAL)
            sleep(CONFIG.IDLE_INTERVAL)
        else:
            for t in task_list:
                t.status = 'running'
                self.sailor.start(t)
                t.status = 'done'
            self.sailor.waiting()
            
            self.logger.info('Waiting %d seconds for next round' % CONFIG.POLLING_INTERVAL)
            sleep(CONFIG.POLLING_INTERVAL)
