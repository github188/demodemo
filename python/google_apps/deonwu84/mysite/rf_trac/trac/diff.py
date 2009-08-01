
import logging
from mysite.rf_trac.models import RobotTestBuild, RobotResult

class DiffResult():
    """testcase log diff result.
    
    last  cur
    PASS/FAIL->unknown  ---> unknown --> white
    
    FAIL->PASS  ---> info --> blue
    FAIL->FAIL  ---> warn --> yellow
    PASS->FAIL  ---> fatal--> red
    PASS->PASS  ---> ignore --> dark        
    """
    
    RESULT_RULES = {"FAIL->PASS":'info',
                    "FAIL->FAIL":'warn',
                    "PASS->FAIL":'fatal',
                    "PASS->PASS":'ignore'
                    }
    
    def __init__(self, original, diff):
        self.original = original
        self.diff = diff
        #['ignore', 'fatal', 'warn', 'info', 'unknown']
        self.info = "unknown"
        if original and diff:
            st = "%s->%s" % (diff.status, original.status)
            self.info = self.RESULT_RULES.get(st, "unknown")
            
    def __getattr__(self, name):
        
        if name.startswith("diff_") and self.diff:
            try:
                return getattr(self.diff, name[5:])
            except AttributeError, e:
                return "N/A"
        else:
            raise AttributeError, "DiffResult has no attribute '%s'" % name
        
    def __str__(self):
        return "%s->%s:%s" %(self.original, self.diff, self.info)
        

def diff_test_build(build, settings, param=""):
    max_search_build = settings.single_build == "1" and 1 or 10    
    log_list = RobotResult.all().filter("build =", build)
    
    logging.info("start diff build:%s, max_search_build=%s" %(build.build_name, max_search_build))
    
    return diff_multi_build(build, log_list, settings, max_search_build)

def diff_multi_build(build, log_list, settings, max_build=10):
    
    logging.info("diff build :%s, parent=%s" %(build.build_name, build.parent()))
    
    build_list = RobotTestBuild.all().ancestor(build.parent()).order("-create_date").\
            filter("create_date <", build.create_date) #exclude build self also.
    
    for e in ["sut_name", "sut_version", "sut_release", "sut_major", "execute_user"]:
        if getattr(settings, e) == "1":
            cur_value = getattr(build, e)
            build_list = build_list.filter("sut_name =", cur_value)
        
    #convert query interface.
    log_list = [ e for e in log_list ]
    result = {}
    
    for e in build_list:
        if max_build > 0 and len(log_list) > 0:
            max_build -= 1
            log_list = fetch_diff_from_build(e, log_list, result)
        else:
            break
        
    for e in log_list:
        result[e.uuid] = DiffResult(e, None)
                
    return result
    
def fetch_diff_from_build(build, original_list, result):
    logging.info("fetch diff log:%s, %s" %(build.build_name, build.create_date))
    des_list = RobotResult.all().filter("build =", build)
    testcase = dict(((e.caseid, e) for e in des_list))
    
    not_found = []
    for log in original_list:
        matched = testcase.get(log.caseid, None)
        if matched is None: 
            not_found.append(log)
        else:
            result[log.uuid] = DiffResult(log, matched)
    
    return not_found
