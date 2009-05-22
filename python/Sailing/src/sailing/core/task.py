from __future__ import with_statement
from contextlib import closing
from sailing.common.common import *
from sailing.core import change_status

class FileTask(object):
    
    def __init__(self, path):
        self.path = path
    
    def start(self):
        with closing(open(self.path, 'r')) as links:
            for l in links:
                self.process(l)
    
    def process(self, l):
        pass
     
    def close(self):
        change_status(self.path, 'done')
    