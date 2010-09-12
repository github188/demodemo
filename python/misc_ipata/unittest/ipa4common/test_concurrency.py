import unittest

from ipa4common.concurrency import TaskScheduler

class TestTaskScheduler(unittest.TestCase):
    
    def test_pending_count_updated(self):
        task = TaskScheduler(0)
        
        self.assertEqual(task.pending_count(), 0)
        
        task.add_task(lambda x:1, (), 'test')
        self.assertEqual(task.pending_count(), 1)

        task.add_task(lambda x:1, (), 'test2')
        self.assertEqual(task.pending_count(), 2)
        
        self.assertEqual(task.pending_count('test2'), 1)
        
