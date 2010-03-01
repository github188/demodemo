
import sys, os, threading, time
from datetime import datetime
from robot.parsing import TestSuite as RobotTestSuite
from robot.parsing.resourcefile import ResourceFile
#from robot.conf.settings import RobotSettings
import logging

import lucene as PyLucene

class RobotIndex(object):
    
    def __init__(self, root, storeDir):        
        self.logger = logging.getLogger("robot_index")
        self.root = root
        
        if not os.path.exists(storeDir):
            os.mkdir(storeDir)
        
        self.analyzer = PyLucene.StandardAnalyzer()
        store = PyLucene.FSDirectory.open(PyLucene.File(storeDir))
        writer = PyLucene.IndexWriter(store, self.analyzer, True)
        writer.setMaxFieldLength(1024 * 4)
        self.writer = writer
            
    def index_robot_script(self, action='A', path=''):
        self.logger.info("%s-->%s" % (action, path))
        data = RobotScriptData(self.root, path)
        if action != 'A':
            data.remove_from_index(self)
        if action != 'D':
            data.add_to_index(self)
            
    def add_document(self, doc):
        self.writer.addDocument(doc)
        
    def remove_document(self, field, key):
        #query = PyLucene.QueryParser(field, self.analyzer)
        #query = query.parse('"%s"' % key)
        self.logger.debug("remove documents:%s->%s" % (field, key))
        self.writer.deleteDocuments(PyLucene.Term(field, key))
    
    def optimize(self):
        self.writer.optimize()
    
    def close(self):
        self.writer.close()

from Schema import TestSuite, TestCase, Keyword, md5
class RobotScriptData(object):
    
    def __init__(self, root, path):
        self.root = root
        self.path = path        
        path, root = self.path.lower(), self.root.lower()
        self.relative_path = path.replace(root, "")
        self.relative_path = self.relative_path.replace("\\", "/")
        self.logger = logging.getLogger("robot_index")
    
    def add_to_index(self, index):
        try:
            self.logger.info("suite:%s" % self.path)
            main_suite = RobotTestSuite(self.path)
            # 'trunk,%s' used to support branch, current only trunk is supported.
            self.__add_suite_to_index(main_suite, index, "trunk,%s" % self.relative_path)
        except Exception, e:
            if str(e).count("contains no test cases") == 0: raise
            
            self.logger.info("resource:%s" % self.path)
            resource = ResourceFile(self.path )
            self.__add_resource_to_index(resource, index, self.relative_path)
    
    def remove_from_index(self, index):
        index.remove_document("path_code", md5("trunk,%s" % self.relative_path))
    
    def __add_resource_to_index(self, res, index, parent=""):
        for kw in res.user_keywords:
            self.__add_keyword_to_index(kw, index, parent)
        
    def __add_suite_to_index(self, suite, index, parent=""):
        self.logger.debug("index suite:%s" % (suite.name, ))
        
        schema = TestSuite(suite, parent, self.relative_path)
        document = self.__build_index_document(schema.index_data())
        index.add_document(document)
        
        for subsuite in suite.suites:
            self.__add_suite_to_index(subsuite, index, schema.uuid)

        for test in suite.tests:
            self.__add_test_to_index(test, index, schema.uuid)
        
        for kw in suite.user_keywords:
            self.__add_keyword_to_index(kw, index, schema.uuid)
    
    def __add_test_to_index(self, test, index, parent=""):
        #self.logger.debug("index test:%s" % (test.name, ))
        
        schema = TestCase(test, parent, self.relative_path)
        document = self.__build_index_document(schema.index_data())
        index.add_document(document)
        
        #for kw in test.keywords:
        #    self.__add_keyword_to_index(kw, index, schema.uuid)

    def __add_keyword_to_index(self, keyword, index, parent=""):
        #self.logger.debug("index keyword:%s" % (keyword.name, ))
        
        schema = Keyword(keyword, parent, self.relative_path)
        document = self.__build_index_document(schema.index_data())
        index.add_document(document)
        #for kw in test.keywords:
        #    self.__add_keyword_to_index(kw, index, schema.uuid)
    
    def __build_index_document(self, data):
        doc = PyLucene.Document()
        for key, value, index in data:
            #store = store and PyLucene.Field.Store.YES or PyLucene.Field.Store.NO
            #self.logger.debug("field:%s-->%s" % (key, value))
            index = {"NO": PyLucene.Field.Index.NO,
                     "NO_NORMS": PyLucene.Field.Index.NO_NORMS,
                     "TOKENIZED": PyLucene.Field.Index.TOKENIZED,
                     "UN_TOKENIZED": PyLucene.Field.Index.UN_TOKENIZED,
                      }.get(index)
            #index = index and PyLucene.Field.Store.YES or PyLucene.Field.Index.UN_TOKENIZED
            doc.add(PyLucene.Field(key, value, PyLucene.Field.Store.YES, index))            
        
        return doc
    