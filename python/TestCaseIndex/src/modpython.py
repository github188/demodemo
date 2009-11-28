import cProfile, time, os
from django.core.handlers.modpython import ModPythonHandler
     
PROFILE_DATA_DIR = "/tmp/tmp_profile"

def log_setting(logging_ini, logging_path):
    import logging.config
    import logging.handlers
    from common.common import join_path
    class __IPATARotatingFileHandler(logging.handlers.RotatingFileHandler):    
        def __init__(self, filename, *args):
            filename = join_path(logging_path, filename)
            logging.handlers.RotatingFileHandler.__init__(self, filename, *args)
    logging.handlers.IPATARotatingFileHandler = __IPATARotatingFileHandler

    import thread
    class __IPATAErrorMailHandler(logging.handlers.SMTPHandler):
        def emit(self, record):
            def send_mail(r):
                try:
                    logging.handlers.SMTPHandler.emit(self, record)
                except BaseException, e:
                    pass
            thread.start_new_thread(send_mail, (record, ))
    logging.handlers.IPATAErrorMailHandler = __IPATAErrorMailHandler
    logging.config.fileConfig(logging_ini)    

import ipata.settings as s
log_setting(s.LOGGING_INI, s.DATA_ROOT)
#setup_sql_logger()

def handler(req):
    '''
    Handler that uses cProfile to store profile data.
    Adapted from django/core/handlers/profiler-hotshot.py
    '''
    if(req.uri.count('testcaselog') > 0):
        profname = "%s.%.3f.prof" % (req.uri.strip("/").replace('/', '.'), time.time())
        profname = os.path.join(PROFILE_DATA_DIR, profname)

        prof = cProfile.Profile()
        results = prof.runcall(ModPythonHandler(), req)
        import sys
        out = open(profname, "w+")
        old_stderr = sys.stderr
        old_stdout = sys.stdout
         
        sys.stdout = sys.stderr = out
        prof.print_stats(1)
        
        sys.stderr = old_stderr
        sys.stdout = old_stdout
        
        out.close()
        return results
    else:
        return ModPythonHandler()(req)

