
from RobotIndex import RobotIndex
import os, sys
import logging
from time import time

def _setting_logging():
    
    filename = "%s/%s" % ("logs", "robot_index.log")
    
    FORMAT = "%(asctime)-15s %(name)-5s: %(levelname)-6s %(message)s"
    logging.basicConfig(level=logging.INFO,
                        format=FORMAT,
                        filename=filename)
    
    console = logging.StreamHandler()
    console.setLevel(logging.DEBUG)
    formatter = logging.Formatter('%(name)-5s: %(levelname)-6s %(message)s')
    console.setFormatter(formatter)
    
    logging.getLogger('').addHandler(console)


def main():
    _setting_logging()
    logger = logging.getLogger("index")
    testcase_root = r"C:\USERS\_work\resp\IPA_TestAutomation\trunk\TestCase"
    indexer = RobotIndex(testcase_root, os.path.abspath("index"))
    
    gst = time()
    def walker(arg, dirname, names):
        if dirname.count(".svn"): return
        for f in names:
            path = os.path.join(dirname, f)
            if not f.endswith(".html"): continue
            try:
                tt = time()
                indexer.index_robot_script("A", path)
                logger.info("elaspe time:%s" % (time() - tt))
            except Exception, e:
                logger.error("error path:%s" % path)
                logger.error("%s" % e)
                
    os.path.walk(testcase_root, walker, None)
    gelaspe = time() - gst
    print "index time:%s" % gelaspe
    
    gst = time()
    indexer.optimize()
    print "optimize time:%s" % (time() - gst)
    indexer.close()
            
if __name__ == "__main__":
    import PyLucene
    print 'PyLucene', PyLucene.VERSION, 'Lucene', PyLucene.LUCENE_VERSION
    #cwd = os.path.abspath(os.path.dirname(__file__))
    os.chdir(r"C:\USERS\_work\workspace_pydev\TestCaseIndex\data")    
    main()
    
    