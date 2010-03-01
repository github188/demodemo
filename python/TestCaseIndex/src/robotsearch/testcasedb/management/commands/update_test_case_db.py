from django.core.management.base import NoArgsCommand

from optparse import make_option
from settings import CONFIG
import os
from robotsearch.testcasedb.models import *
from common.svnpy import CliClient
from robot.parsing import TestSuite as RobotTestSuite

try:
    import hashlib
except:
    import md5 as hashlib

def md5(str): return hashlib.md5(str).hexdigest()

class Command(NoArgsCommand):
    help = "import testcase to test case db."
    
    option_list = NoArgsCommand.option_list + (
        make_option('--path', default=None, dest='path',
            help='update path'),
        make_option('--status', default='A', dest='status', 
            help='A=Add (default), D=Deleted '),
        make_option('--branch', default='trunk', dest='branch', 
            help='default is "trunk" '),            
    )

    requires_model_validation = False
    
    def handle_noargs(self, path='', status='A', branch='trunk', **param):
        import logging
        self.logger = logging.getLogger("db.update")
        self.logger.info("status=%s, path=%s" % (status, path))
        self.svnclient = CliClient(work_path=CONFIG.TESTCASE_ROOT)
        
        if (not os.path.exists(path)):
            self.logger.warning("not exists file:%s" % path)
            return
        
        info = self.format_path(branch, path)
        if os.path.isdir(path):
            self.update_directory(status, info)
        elif status == 'D':
            self.remove_test_case_file(info)
        else:
            self.update_test_case_file(info)
            
        self.logger.info("updated path=%s" % path)
    
    def update_test_case_file(self, path_info):
        f, created = IndexFile.objects.get_or_create(uuid=path_info.uuid)
        if created or not f.path:
            f.path = path_info.path
            f.file_type = 'F'
            f.status = 'U'
            f.branch = path_info.branch
            
        info = self.svnclient.info(path_info.abspath)
        f.svn_author = info['author']
        f.svn_lastupdated = info['date']
        f.svn_version = info['revision']
        f.save()
        self.__update_robot_test_case(f, path_info)
        
    def __update_robot_test_case(self, index_file, path_info):
        for e in RobotIndexItem.objects.filter(file=index_file):
            e.delete()
        try:
            main_suite = RobotTestSuite(path_info.abspath)            
            self.__add_suite(index_file, main_suite, "%s,%s" % (index_file.branch, 
                                                                path_info.path))
        except Exception, e:
            if str(e).count("contains no test cases") == 0: raise
            self.logger.info("'%s' is not a robot scripts." % path_info.path)
            
    def __add_suite(self, file, suite, parent=""):
        uuid = md5("%s.%s" % (parent, suite.name))
        force_tags = suite.force_tags or []

        e = RobotIndexItem(uuid=uuid)
        e.branch = file.branch
        e.file = file
        e.item_type = 'S'
        e.status = 'U'
        e.name = suite.name
        e.tags = ";".join(force_tags)
        e.save()
        
        for test in suite.tests:
            self.__add_testcase(file, test, e.uuid, force_tags + ['$$', ])
    
    def __add_testcase(self, file, test, parent, force_tags=[]):
        uuid = md5("%s.%s" % (parent, test.name))
        
        tags = test.tags or []
        
        e = RobotIndexItem(uuid=uuid)
        e.parent = parent
        e.branch = file.branch
        e.file = file
        e.item_type = 'T'
        e.status = 'U'
        e.name = test.name
        e.tags = ";".join(force_tags + tags)
        e.save()
        
        self.logger.info("test case:name=%s, tags=%s, parent=%s" % (e.name, e.tags, parent))        
    
    def remove_test_case_file(self, path_info):
        for f in IndexFile.objects.only('status').filter(uuid=path_info.uuid):
            f.status = 'D'
            f.save()
            for i in RobotIndexItem.object.only('status').filter(file=f):
                i.status = 'D'
                i.save()
    
    def update_directory(self, status, path_info):
        def walker(arg, dirname, names):
            if dirname.count(".svn"): return
            for f in names:
                path = os.path.join(dirname, f)
                if not f.endswith(".html"): continue
                #info = 
                info = self.format_path(path_info.branch, path)
                if status == 'D':
                    self.remove_test_case_file(info)
                else:
                    self.update_test_case_file(info)
                            
        os.path.walk(path_info.abspath, walker, None)
        
    def format_path(self, branch, path):
        path = os.path.normpath(os.path.abspath(path))
        x = lambda:0
        x.abspath = path
        x.path = path.replace(CONFIG.TESTCASE_ROOT, "")
        x.path = x.path.replace("\\", "/")
        x.uuid = md5("%s,%s" %(branch, x.path))
        x.root = CONFIG.TESTCASE_ROOT
        x.branch = branch
        
        return x
