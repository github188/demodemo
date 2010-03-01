# Copyright 2009 Nokia Siemens Networks Oyj
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import os
import thread
import os.path

def log_setting(logging_ini, logging_path):
    import logging.config
    import logging.handlers
    class __IPATARotatingFileHandler(logging.handlers.RotatingFileHandler):
        def __init__(self, filename, *args):
            filename = os.path.join(logging_path, filename)
            dir = os.path.dirname(filename)
            if not os.path.exists(dir):
                os.makedirs(dir)
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
                    #import sys
                    #sys.stderr.write(str(e))
            thread.start_new_thread(send_mail, (record, ))
    logging.handlers.IPATAErrorMailHandler = __IPATAErrorMailHandler
        
    logging.config.fileConfig(logging_ini)
    
