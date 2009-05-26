
from sailing.common.ctlDaemon import ControllableDaemon
from sailing.common.logger import Logger
from sailing.common.common import *
from sailing.common.utils import import_class
from sailing.conf import settings as CONFIG
import glob
import os
import logging

class Application(ControllableDaemon):
    
    def __init__(self):
        
        ControllableDaemon.__init__(self, '%s daemon' % CONFIG.APP_NAME, CONFIG.RUNNING_FLAG, CONFIG.CLOSING_FLAG)
            
    def prepare(self):
        self._setting_logging()
        self.logger = logging.getLogger("root")
        
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
            
    def _setting_logging(self):
        
        filename = "%s/%s" % (CONFIG.LOG_PATH, CONFIG.LOG_NAME)
        
        FORMAT = "%(asctime)-15s %(name)-5s: %(levelname)-6s %(message)s"
        logging.basicConfig(level=logging.DEBUG,
                            format=FORMAT,
                            filename=filename)
        
        console = logging.StreamHandler()
        console.setLevel(logging.INFO)
        formatter = logging.Formatter('%(name)-5s: %(levelname)-6s %(message)s')
        console.setFormatter(formatter)
        
        logging.getLogger('').addHandler(console)
