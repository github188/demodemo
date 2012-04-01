# -*- coding: utf8 -*-
import unittest

import os, sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

from actions import TopicParser

base_root = os.path.dirname(os.path.abspath(__file__))

class FetchGuessTopicTestCase(unittest.TestCase):
    
    def test_fetch_topic_data(self):
        topic = TopicParser()
        
        data = self._load_data('gest_topic_1.html')
                
        t = topic.process(data, {})
        self.assertEqual(t['title'], "许仙不在我们这里，真的不在，白素贞你就收手吧！", "not found topic title")
        self.assertEqual(t['images'], "http://static.uqude.com/uploads/v2/0D/20120309/1331277758386_357.300s600s0.jpg", "not found topic images")
        self.assertEqual(t['desc'], "这是什么体？<br>", "not found topic desc, '%s'" % t['desc'])
        
        options = t['options']
        
        self.assertEqual(len(options), 4, "the topic len shoud 4, fined:%s" % len(options))
        
        
        self.assertEqual(options['174161']['id'], '174161')
        self.assertEqual(options['174161']['desc'], '淘宝体')
        
        self.assertEqual(options['174163']['desc'], '咆哮体')
        
        self.assertEqual(t['coverId'], '37920')
        self.assertEqual(t['coverSubjectId'], '234644')
        
    def test_fetch_answer(self):
        topic = TopicParser()
        
        data = self._load_data('gest_topic_1_answer.html')

        an = topic.process_answer(data, {})
        
        self.assertEqual(an['options'], "白素贞体", )
        self.assertEqual(an['desc'], "近日白素贞体在网络流行<br><br>", )

    def test_fetch_answer2(self):
        topic = TopicParser()
        
        data = self._load_data('guest_topic_2_answer.html')

        an = topic.process_answer(data, {})
        
        self.assertEqual(an['options'], "“中国第一制服美女”", )
        self.assertEqual(an['desc'], "", )


    def _load_data(self, f):
        fd = open(os.path.join(base_root, f), 'r')
        data = fd.read()
        fd.close()
        
        return data

if __name__ == "__main__":
    unittest.main()