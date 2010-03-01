import logging, os, time, sys, datetime
import indexsettings_defaults
from common.ctlDaemon import ControllableDaemon
from common.common import read_from_file, write_to_file, remove_path, run_command
from common.svnpy import CliClient

try:
    import indexsettings
except ImportError:
    indexsettings = None
    
class Settings(object):    
    def __getattr__(self, name):
        try:
            return getattr(indexsettings, name)
        except AttributeError:
            return getattr(indexsettings_defaults, name)

CONFIG = Settings()

class TestCaseIndexer(ControllableDaemon):
    def __init__(self):
        print "work dir:%s" % CONFIG.DATA_ROOT
        ControllableDaemon.__init__(self, 'testcase indexer daemon', 
                                    CONFIG.RUNNING_FLAG, 
                                    CONFIG.CLOSING_FLAG,
                                    CONFIG.RUN_DAEMON)
    
    def prepare(self):
        from utils import log_setting
        log_setting(CONFIG.LOGGING_INI, CONFIG.DATA_ROOT)
        self.logger = logging.getLogger("index_daemon")
        self.logger.path = CONFIG.DATA_ROOT
        import lucene
        os.environ['JAVA_HOME'] = '/usr/local/jdk1.6.0_17'
        lucene.initVM(lucene.CLASSPATH)
        
        self.svnclient = CliClient(work_path=CONFIG.TESTCASE_ROOT)
        self.new_updated_version = None
        self.updated_items = None
        self.last_index_trigger = CONFIG.LAST_UPDATED
        self.last_index_version = None
        
        self.logger.info("test case path:%s" % CONFIG.TESTCASE_ROOT)
        #self.logger.removeHandler(hdlr)
        
        self.run = self.update_subversion
        os.chdir(CONFIG.DATA_ROOT)
            
    def update_subversion(self):        
        timestamp = datetime.datetime.now().strftime('%Y%m%d%H%M%S')
        log_file = 'logs/builds/%s.log' % timestamp
        self.build_logging_handler = logging.handlers.IPATARotatingFileHandler(log_file, 'a', 20000000, 1)
        logging.getLogger("").addHandler(self.build_logging_handler)
        
        if CONFIG.AUTO_UPDATE:
            self.logger.info("auto updating testcase from subversion")
            self.new_updated_version = self.svnclient.update()
        else:
            self.logger.info("ignore update testcase...")
            self.logger.info("checking local svn info...")
            local_info = self.svnclient.info()
            self.new_updated_version = local_info['revision']
        
        self.run = self.diff_last_updated
        
    def diff_last_updated(self):
        self.last_index_version = None
        if not os.path.isdir(CONFIG.INDEX_PATH):
            self.logger.info("Not found index data...")
            self.run = self.init_index_data
            return
        
        if os.path.isfile(self.last_index_trigger):
            self.last_index_version = read_from_file(self.last_index_trigger)
            self.last_index_version = self.last_index_version.strip()
            self.logger.info("last index version:%s" % self.last_index_version)
            
            self.updated_items = None
            if self.last_index_version != self.new_updated_version:
                self.updated_items = self.svnclient.diff_summarize(old_revision=self.last_index_version)
            
            if not self.updated_items:
                self.logger.info("No updating from last index.")
                self.run = self.update_index_trigger
            else:
                self.run = self.update_testcase_index
        else:
            self.logger.info("Not found last index version...")
            self.run = self.init_index_data
            
    def update_index_trigger(self):
        #self.logger.info("update...")
        if self.last_index_version != self.new_updated_version:
            self.logger.info("update index trigger to %s" % 
                             self.new_updated_version)
            write_to_file(self.last_index_trigger, self.new_updated_version)
        
        self.run = self.waiting_polling_interval
    
    def waiting_polling_interval(self):
        self.logger.info('Waiting %d seconds for next round' % CONFIG.POLLING_INTERVAL)
        if(self.build_logging_handler):
            logging.getLogger("").removeHandler(self.build_logging_handler)
            self.build_logging_handler.flush()
            self.build_logging_handler.close()
            
        for i in range(0, CONFIG.POLLING_INTERVAL + 5, 5):
            if self.is_closing(): break
            time.sleep(5)
        
        self.run = self.update_subversion
    
    def update_testcase_index(self):
        self.logger.info("updating testcase index...")
        indexer = self._create_indexer()
    
        for item in self.updated_items:
            if self.is_closing(): break
            if item['kind'] != 'file': continue
            #'modified', 'added', 'deleted', 'conflicted'
            status = {"modified": "U",
                      "added": "A",
                      "deleted": "D",
                      "conflicted":"C",
                      }.get(item['status'], "C")
            if status == "C":
                self.logger.warning("Conflicted file '%s'" % item['path'])
                continue
            if not os.path.isfile(item['path']):
                self.logger.warning("not exists file '%s' in local!" % item['path'])
                continue
            indexer.index_robot_script(status, item['path'])
            self._update_test_case_db(status, os.path.join(CONFIG.TESTCASE_ROOT,
                                                           item['path']))
                        
        indexer.optimize()
        indexer.close()
            
        self.run = self.update_index_trigger
        
    def init_index_data(self):
        self.logger.info("initialization  testcase index...")
        remove_path(CONFIG.INDEX_PATH)
        logger = self.logger
        
        indexer = self._create_indexer()
        gst = time.time()
        def walker(arg, dirname, names):
            if self.is_closing(): raise Exception, "closing"
            if dirname.count(".svn"): return
            for f in names:
                path = os.path.join(dirname, f)
                if not f.endswith(".html"): continue
                try:
                    tt = time.time()
                    indexer.index_robot_script("A", path)
                    logger.info("elaspe time:%s" % (time.time() - tt))
                except Exception, e:
                    logger.error("error path:%s" % path)
                    logger.error("%s" % e)
        
        try:        
            os.path.walk(CONFIG.TESTCASE_ROOT, walker, None)
        except Exception, e:
            if str(e) != "closing":raise
        
        gelaspe = time.time() - gst
        
        logger.info("index all testcase elapsed:%s" % gelaspe)
        
        gst = time.time()
        indexer.optimize()
        logger.info("optimize index :%s" % (time.time() - gst))
        indexer.close()
        
        self.run = self.update_index_trigger
        
    def run_once(self):
        self.prepare()
        self.run()
    
    def _create_indexer(self):
        from index.RobotIndex import RobotIndex
        return RobotIndex(CONFIG.TESTCASE_ROOT, CONFIG.INDEX_PATH)

    def _update_test_case_db(self, status, path):
        scripts = CONFIG.UPDATE_TEST_CASE_DB % (path, status)
        self.logger.info(run_command(scripts, self.logger, level=None))

def main():
    if len(sys.argv) != 2 or sys.argv[1] not in ['start', 'stop', 'forcestart', 'run_once']:
        print 'python index_svn_testcase start|stop|forcestart'
    else:
        import lucene
        os.environ['JAVA_HOME'] = '/usr/local/jdk1.6.0_17'
        #lucene.initVM(lucene.CLASSPATH)
        getattr(TestCaseIndexer(), sys.argv[1])()

if __name__ == '__main__':
    main()

