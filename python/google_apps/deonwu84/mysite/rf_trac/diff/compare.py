
from models import RobotTestBuild, RobotResult

class DiffResult():
    """testcase log diff result.
    
    last  cur
    PASS/FAIL->unknown  ---> unknown --> white
    
    FAIL->PASS  ---> info --> blue
    FAIL->FAIL  ---> warn --> yellow
    PASS->FAIL  ---> fatal--> red
    PASS->PASS  ---> ignore --> dark        
    """
    
    def __init__(self, src, des):
        self.src_item = None
        self.des_item = None
        #['ignore', 'fatal', 'warn', 'info', 'unknown']
        self.desc = ""
        

def diff_test_build(build, settings, param):
    compare = settings.single_build and diff_single_build or \
                                        diff_multi_build
    
    log_list = RobotResult.all().filter("build =", build)
    return compare(build, log_list, settings)

def diff_single_build(build, log_list, settings):
    
    build_list = RobotTestBuild.all().ancestor(build.parent).order_by("-create_date")
    for e in ["sut_name", "sut_version", "sut_release", "sut_major", "execute_user"]:
        if getattr(settings, e) == "1":
            cur_value = getattr(build, e)
            build_list = build_list.filter("sut_name =", cur_value)
    
    des_build = build_list.get()
    if des_build is not None:
        des_list = RobotResult.all().filter("build =", des_build)
    #des_log = dict(((e.uu ))
    
    pass

def diff_multi_build(build, log_list, settings):
    pass