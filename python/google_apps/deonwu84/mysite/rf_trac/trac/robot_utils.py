
import os
import logging
from robot.output.readers import TestSuite
from robot import utils
from mysite.rf_trac.models import RobotResult, RobotTestBuild, RobotTest
from datetime import datetime
import hashlib
#from models import RobotResult

class RobotUtils(object):
    
    @staticmethod
    def import_test_build(f, build_name, project, param):
        logging.debug("uploaded image:%s, file size:%s" % (build_name, f.size))
        
        suite = RobotUtils.retrieve_robot_suite(f.read())
        build = RobotTestBuild(parent=project, build_name=build_name)

        for n in ["build_name", "sut_name", "sut_version", 
                   "sut_release", "sut_major", "execute_user"]:
            if param.has_key(n):
                setattr(build, n, param[n])
        
        for k, v in suite.metadata.iteritems():
            k = k.replace(" ", "_").lower()
            if k in ["build_name", "sut_name", "sut_version", 
                     "sut_release", "sut_major", "execute_user"]:
                setattr(build, k, v)
        
        build.put()
        RobotUtils.import_test_suite(build, suite, project)
        build.summary_count = build.summary_fail + build.summary_pass
        build.put()
        
        build.build_index()
        logging.debug("successful to import %s test result" % (build.summary_count, ))
        
        return build
        
    @staticmethod
    def retrieve_robot_suite(data):
        root = utils.DomWrapper(None, data)
        
        def _get_suite_node(root):
            if root.name != 'robot': 
                raise AttributeError
            node = root.get_node('suite')
            node.generator = root.get_attr('generator', 'notset').split(' ')[0].lower()
            return node
        
        suite = TestSuite(_get_suite_node(root), -1) 
        suite.set_names()
        suite.parent = None
        
        return suite
    
    @staticmethod
    def import_test_suite(build, suite, project):
        for test in suite.tests:
            test.parent = suite
            RobotUtils.import_test_case(build, test, project)
    
        for subsuite in suite.suites:
            subsuite.parent = suite
            RobotUtils.import_test_suite(build, subsuite, project)
            
    
    @staticmethod
    def import_test_case(build, test, project):        
        uuid = RobotUtils.build_case_log_uuid(build, test)
        t = RobotResult(parent=project, uuid=uuid)
        t.build = build
        
        t.case = RobotUtils.import_robot_case(build, test, project)
        t.reportid = t.case.reportid
        #t.testname = test.name
        #t.suitename = test.parent.name
        #t.longname = test.longname

        t.starttime = test.starttime
        t.endtime = test.endtime
        t.message = test.message
        t.status = test.status
        t.tags = ";".join(test.tags)
        
        t.caseid = RobotUtils.build_case_id(build, test, project)
        
        def convert_to_time(s_time):
            import re
            t = re.match(r'(\d{4})(\d{2})(\d{2}) (\d{2}):(\d{2}):(\d{2}).(\d{3})',
                        str(s_time))
            if t: return datetime(*map(int, t.groups()))
            return None
        _t = convert_to_time
        elapse_time = _t(test.endtime) - _t(test.starttime)
        t.elapsed = elapse_time.days * 86400 + elapse_time.seconds
        t.put()
        if t.status == "PASS":
            build.summary_pass += 1
        else:
            build.summary_fail += 1
    
    @staticmethod
    def build_case_log_uuid(build, test):
        import hashlib
        name = "%s.%s" %(test.parent.name, test.name)        
        times = "%s_%s" % (test.starttime, test.endtime)
        logid = "%s_%s" % (name.strip(), times)
        
        return hashlib.md5(logid.lower()).hexdigest()
        
    @staticmethod
    def build_case_id(build, test, project):
        if test.parent:
            name = "%s.%s" %(test.parent.name, test.name)
            case_id = md5(name.lower())
            if RobotTest.all().ancestor(project).filter("caseid =", case_id).get() is None:
                RobotTest(parent=project,
                          caseid=case_id,
                          testname=test.name,
                          suitename=test.parent.name).put()
            return case_id
        else:
            return ""
    
    @staticmethod
    def import_robot_case(build, test, project):
        uuid = md5(test.longname.lower())
        t = RobotTest.all().ancestor(project).filter("caseid =", uuid).get()
        if t is None:
            t = RobotTest(parent=project, 
                          caseid=uuid,
                          longname=test.longname, 
                          testname=test.name)
            if test.parent:
                t.suitename=test.parent.name
                t.suite = RobotUtils.import_robot_case(build, test.parent, project)
            t.reportid = RobotUtils.build_case_id(build, test, project)
            t.put()
        return t
    
def md5(str): return hashlib.md5(str).hexdigest()
    
    
        