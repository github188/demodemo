import logging

class BaseDebugInterface(object):
    
    def __init__(self, debuger):
        self.robotDebuger = debuger
        self.debugCtx = debuger.debugCtx
        self.logger = logging.getLogger("rbt.int")
        self.bp_id = 0
    
    def start(self, settings):
        """start debug interface."""
        pass
    
    def close(self):
        pass
    
    def go_steps(self, count): self.debugCtx.go_steps(int(count))
    def go_into(self): self.debugCtx.go_into()
    def go_over(self): self.debugCtx.go_over()
    def go_on(self): self.debugCtx.go_on()
    def go_return(self): self.debugCtx.go_return()
    def go_pause(self): return self.debugCtx.go_pause()
    def add_breakpoint(self, bp): self.robotDebuger.add_breakpoint(bp)
    def watch_variable(self, name): return self.robotDebuger.watch_variable(name)
    def remove_variable(self, name): return self.robotDebuger.remove_variable(name)
    
    
    def update_variable(self, name, value):
        from robot.running import NAMESPACES
        NAMESPACES.current.variables[name] = value
    
    @property
    def watching_variable(self):return self.robotDebuger.watching_variable
    
    def disable_breakpoint(self, name):
        for e in self.debugCtx.break_points:
            if e.name == name: e.active = False
        #self.debugCtx.add_breakpoint(KeywordBreakPoint('', bps))
        
    def enable_breakpoint(self, name):
        for e in self.debugCtx.break_points:
            if e.name == name: e.active = True

    def update_breakpoint(self, name):
        for e in self.debugCtx.break_points:
            if e.name == name: e.active = not e.active
        #self.debugCtx.add_breakpoint(KeywordBreakPoint('', bps))

    
    