
from runtime import BaseRuntime as RT
from runtime import KeywordRuntime
import re

class BreakPoint(object):
    def __init__(self, name):
        self.name = name
        self.active = True
        self.expired = False
    
    def matched_context(self, stack):
        return False
    
    def __str__(self):
        return "break:%s" % (self.name)
    
class KeywordBreakPoint(BreakPoint):
    def __init__(self, name, kw_name, state=RT.START):
        BreakPoint.__init__(self, name)
        self.kw_name = kw_name
        self.state = state
        self.pattern = '^' + kw_name.replace('*', '.*') + '$'
        
    def matched_context(self, stack):
        
        if not self.active or len(stack) <= 0:
            return False
        
        rt = stack[-1]
        if isinstance(rt, KeywordRuntime):
            if re.match(self.pattern, rt.name, re.I) and rt.state == self.state:
                return True
            
        return False
    
    def __str__(self):
        return "break:%s, pattern=%s" % (self.name, self.kw_name)
    
class RuntimeBreakPoint(BreakPoint):
    def __init__(self, name, rt, state=RT.START):
        BreakPoint.__init__(self, name)
        self.rt = rt
        self.state = state
        self.rt_done = False

    def matched_context(self, stack):
        
        if not self.active or len(stack) <= 0:
            return False 
        
        if self.rt_done and self.state == RT.DONE:
            self.expired = True
            return True
        
        if stack[-1] == self.rt:
            if stack[-1].state == self.state:
                return True
            elif self.state == RT.DONE and stack[-1].state == RT.END:
                """it's active at next step
                """
                self.rt_done = True
                return False
        else:
            return False
        
    def __str__(self):
        xx = str(self.rt)
        return "Runtime break:%s, rt=%s" % (self.name, xx)
        

class SemaphoreBreakPoint(BreakPoint):
    def __init__(self, name, init_count=1):
        BreakPoint.__init__(self, name)
        self.semaphore = init_count
        
    def matched_context(self, stack):
        
        if not self.active or len(stack) <= 0:
            return False
        
        if stack[-1].state == RT.START:
            self.semaphore -= 1
            
        self.expired = self.semaphore == 0
        
        return self.expired

    def __str__(self):
        return "Semaphore break:%s, semaphore=%s" % (self.name, self.semaphore)

    