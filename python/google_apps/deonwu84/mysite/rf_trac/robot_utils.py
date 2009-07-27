
import os
import logging
from robot.output.readers import TestSuite
from robot import utils
from models import RobotResult, RobotTestBuild
from datetime import datetime
#from models import RobotResult

class RobotUtils(object):
    
    @staticmethod
    def import_test_build(f, build_name, project):
        logging.debug("uploaded image:%s, file size:%s" % (build_name, f.size))
        
        suite = RobotUtils.retrieve_robot_suite(f.read())
        build = RobotTestBuild(parent=project, build_name=build_name)
        for k, v in suite.metadata.iteritems():
            k = k.replace(" ", "_").lower()
            if k in ["build_name", "sut_name", "sut_version", 
                     "sut_release", "sut_major"]:
                setattr(build, k, v) 
        
        build.put()
        suite.parent = None
        RobotUtils.import_test_suite(build, suite, project)
        build.summary_count = build.summary_fail + build.summary_pass
        build.put()
        
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
        t.testname = test.name
        t.suitename = test.parent.name
        t.longname = test.longname

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
        import hashlib
        name = "%s.%s" %(test.parent.name, test.name)
        
        return hashlib.md5(name.lower()).hexdigest()
        
        