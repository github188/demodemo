import unittest
#import ipata.analyser.models as m 
from pyshop.coreapp.project_importor import ProjectImportor
import os
        
class TestPorjectImportor(unittest.TestCase):
    
    def setUp(self):
        self.dist_root = os.path.abspath(os.path.join(os.path.dirname(__file__),
                                                      "dist"))
        
    def test_import_new_project(self):
        pi = ProjectImportor()
        
        prj = pi.update_project(os.path.join(self.dist_root, "easy_install"))
        
        self.assertEqual(prj.name, 'easy_install')
        self.assertEqual(prj.dev_type, 'dev')
        self.assertEqual(prj.dev_status, '3')
        self.assertEqual(prj.language, 'javascript')
        self.assertEqual(prj.status, 'updated')
        self.assertEqual(prj.classifiers.name, 'web tools')
        self.assertEqual(prj.summary, 'test summary')
        
        
    def test_import_empty_project(self):
        pi = ProjectImportor()
        
        prj = pi.update_project(os.path.join(self.dist_root, "easy_empty"))
        self.assertEqual(prj.name, 'easy_empty')
        self.assertEqual(prj.status, 'expired')

    def test_last_releast_version(self):
        pi = ProjectImportor()
        
        prj = pi.update_project(os.path.join(self.dist_root, "easy_release"))
        self.assertEqual(prj.name, 'easy_release')
        self.assertEqual(prj.status, 'updated')
        self.assertEqual(prj.last_version, '1.1')

