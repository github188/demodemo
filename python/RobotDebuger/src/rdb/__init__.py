from RobotDebuger import RobotDebuger
from debuger.runtime import KeywordRuntime, TestCaseRuntime, TestSuiteRuntime
import logging, sys

class Listener(object):
    ROBOT_LISTENER_API_VERSION = 2
    
    def __init__(self, cfg='debug.rdb', *bps):
        from robot import version
        ROBOT_VERSION = version.get_version()
        if ROBOT_VERSION < '2.1':
            sys.__stderr__.write("Robot 2.1 is required for RDB(robot debug).\n")
            sys.exit(0)
        
        self.debuger = RobotDebuger(cfg)
        self.debuger.run()
        self.call_start = []
        self.debugCtx = self.debuger.debugCtx
        self.logger = logging.getLogger("rbt.lis")
        for e in bps:
            self.debuger.add_breakpoint(e)
        
    def start_suite(self, name, attrs):
        self.logger.debug("start_suite:%s, attr:%s" % (name, attrs))
        self.call_start.append(TestSuiteRuntime(name, attrs))
        self.debugCtx.start_function(self.call_start[-1])

    def end_suite(self, name, attrs):
        self.logger.debug("end_suite:%s, attr:%s" % (name, attrs))
        self.call_start[-1].attrs = attrs
        self.debugCtx.end_function(self.call_start[-1])
        self.call_start.pop()

    def start_test(self, name, attrs):
        self.logger.debug("start_test:%s, attr:%s" % (name, attrs))
        self.call_start.append(TestCaseRuntime(name, attrs))
        self.debugCtx.start_function(self.call_start[-1])
    
    def end_test(self, name, attrs):
        self.logger.debug("end_test:%s, attr:%s" % (name, attrs))
        self.call_start[-1].attrs = attrs
        self.debugCtx.end_function(self.call_start[-1])
        self.call_start.pop()
    
    def start_keyword(self, name, attrs):
        self.logger.debug("start_keyword:%s, attr:%s" % (name, attrs))
        self.call_start.append(KeywordRuntime(name, attrs))
        self.debugCtx.start_function(self.call_start[-1])
        
    def end_keyword(self, name, attrs):
        self.logger.debug("end_keyword:%s, attr:%s" % (name, attrs))
        self.call_start[-1].attrs = attrs
        self.debugCtx.end_function(self.call_start[-1])
        self.call_start.pop()
    
    def close(self):
        self.logger.debug("close..................")
        self.debuger.close()
