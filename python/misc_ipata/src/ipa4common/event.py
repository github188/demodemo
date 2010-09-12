"""
simple event system.
"""

import fnmatch
from concurrency import TaskScheduler 

class Event(object):
    def __init__(self, name, manager, bubble, sync=True):
        self.name = name
        self.status = "new" #"new, dispatching, handled, canceled"
        self.manager = manager
        self.bubble = bubble
        self.sync = sync
        
    def connect(self, reciver):
        self.manager.connect(reciver, self.name)
        
    def cancel(self):self.status = 'canceled'
    
    def __call__(self, *args, **kw):
        return self.manager.fire(self, *args, **kw)

EVENT_SEQUENCE = ( e for e in range(10000) )
class EventManager(object):
    def __init__(self, name=None, parent=None, task_manager=None):
        self.parent = parent
        self.name = name or 'EM_%s' % EVENT_SEQUENCE.next()
        self.events = {}
        self.reciver_list = []
        self.task_manager = task_manager or TaskScheduler(2)
    
    def connect(self, reciver, event_name, check_exists=True):
        """
        @param reciver: -- 
        @param event_name: --
        @param check_exists: -- 
        """
        if check_exists and not self.check_exists(event_name):
            raise RuntimeError, "not exists event '%s'." % event_name
        
        self.reciver_list.append((reciver, event_name))
    
    def disconnect(self, reciver, event_name=None):
        for e in list(self.reciver_list):
            rec, r = e
            if rec != reciver: continue
            if event_name and event_name != r: continue
            self.reciver_list.remove(e)
    
    def fire(self, event, *args, **kw):
        if not isinstance(event, Event): event = self.__find_event_by_name(event)
        def process_event():
            event.status = 'dispatching'
            for (rec, rule) in self.chain_of_reciver(event.name, event.bubble):
                rec(event, *args, **kw)
                if event.status == 'canceled':
                    break
                else:
                    event.status = 'handled'
        
        if event.sync:
            process_event()
        else:
            self.task_manager.add_task(process_event, )
        
        return event
    
    def chain_of_reciver(self, name, bubble):
        for r, pattern in self.reciver_list:
            if fnmatch.fnmatch(name, pattern):
                yield (r, pattern)
        if bubble and self.parent is not None:
            for e in self.parent.chain_of_reciver(name, bubble):
                yield e
      
    def create_events(self, name_list=None, sync=True):
        """
        @param name_list: -- 
        @param sync: --
            create new events object.
            1. xxx, -- command event only broadcast in one level.
            2. [xxx] -- signal event, 
        """
        
        return_list = False
        if name_list is None:
            name_list = ["evt_%s" % EVENT_SEQUENCE.next(), ]
        elif isinstance(name_list, basestring):
            name_list = [name_list, ]
        else:
            return_list = True
            
        created_list = []
        for e in name_list:
            bubble, name = (False, str(e))
            if name.startswith('['):
                bubble, name = (True, name[1:-1])
            if self.check_exists(name):
                raise RuntimeError, "exists event '%s'" % name
            event = Event(name, self, bubble, sync)
            self.events[name] = event
            created_list.append(event)
        
        return return_list and created_list or created_list[0]
    
    def new_manager(self, name=''):
        name = name or "EM_%s" % EVENT_SEQUENCE.next()
        return EventManager(name, self, self.task_manager)
    
    def __find_event_by_name(self, name):
        evt = self.events.get(name, None)
        if evt is None and self.parent is not None:
            return self.parent.__find_event_by_name(name)
        
        return evt
    
    def check_exists(self, event_name):
        """locate event in manager."""
        for e in self.events.keys():
            if fnmatch.fnmatch(e, event_name):return True
        if self.parent is not None:
            return self.parent.check_exists(event_name)
        return False

#=================UNIT TEST=================
import unittest
class TesEventManager(unittest.TestCase):
    def test_create_event(self):
        #create anonymity event
        em = EventManager()
        e = em.create_events()
        
        self.assertEqual(e.bubble, False)
        self.assertEqual(e.sync, True)

        e = em.create_events('test')
        self.assertEqual(e.name, 'test')
        self.assertEqual(e.bubble, False)
        self.assertEqual(e.sync, True)
        
        e = em.create_events('[test1]', False)
        self.assertEqual(e.name, 'test1')
        self.assertEqual(e.bubble, True)
        self.assertEqual(e.sync, False)
        
    def test_connect_and_file(self):
        #create anonymity event
        em = EventManager()
        e = em.create_events()
        
        obj = lambda x:1
        def handler(e, name):
            obj.event = e
            obj.name = name
        
        e.connect(handler)
        
        e("test fire")
        
        self.assertEqual(obj.event, e)
        self.assertEqual(obj.name, "test fire")
    
    
#=================Usage demo=================

if __name__ == '__main__':
    #create anonymity event
    em = EventManager()
    e = em.create_events()
    def handler(e, name):
        print "handling event:%s, status=%s" % (e.name, e.status)
        print "event args:%s" % name
    
    e.connect(handler)
    e("test fire")
    
    print "create named and matching with pattern"
    e = em.create_events("test_event")
    em.connect(handler, "test_*")
    em.fire("test_event", "test file from name")

    e = em.create_events("test_event2")
    e("test file from test_event2")

    print "create async handler"
    import time
    def rsync_handler(e, name):
        
        print "handling event:%s, status=%s" % (e.name, e.status)
        print "starting....."
        time.sleep(2)   
        print "done %s....." % e.name
        
    e = em.create_events(None, False)
    e.connect(rsync_handler)
    
    print "fire event...."
    e("rsync")
    print "done fire event!"
    time.sleep(4)
    
    print "parent manager should handle sub-manager event !"
    sub_em = em.new_manager()
    e = sub_em.create_events("[test_subevent]")
    e("sub event")
    
    print "parent manager should not handle sub-manager event !"
    sub_em = em.new_manager()
    e = sub_em.create_events("test_subevent")
    e("sub event")
    print "done......."
    
    print "cancel event process!"
    def handler2(e, name):
        print "canceling event:%s, status=%s" % (e.name, e.status)
        e.cancel()
            
    sub_em = em.new_manager()
    e = sub_em.create_events("[test_subevent3]")
    e.connect(handler2)
    e("cancel test")
    print "done......."    
    
    
    

    
    