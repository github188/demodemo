# -*- coding: utf-8 -*-
#from django.db import models
from google.appengine.ext import db
import logging

class RobotProject(db.Model):
    name = db.StringProperty(required=True)
    admin_user = db.StringProperty()
    admin_password = db.StringProperty()

    #a project key for distinguish project.
    prj_key = db.StringProperty()
    status_list = db.StringProperty(multiline=True)
    create_date = db.DateTimeProperty(auto_now_add=True)
    active = db.IntegerProperty(default=0)
    
    def __str__(self):
        return self.name

# parent is Project.
class ProjectSetting(db.Model):
    name = db.StringProperty(required=True) #setting name
    item_name = db.StringProperty()
    item_value = db.StringProperty(multiline=True)
    #description = db.StringProperty(multiline=True)
        

# parent is Project.
class RobotTestBuild(db.Model):
    #uuid = db.StringProperty(required=True)
    build_name = db.StringProperty(required=True)
    sut_name =  db.StringProperty()

    sut_version =  db.StringProperty()
    sut_release =  db.StringProperty()
    sut_major =  db.StringProperty()
    
    status = db.StringProperty() #removed/ reporting/
    #execute_user = db.StringProperty()
    execute_user = db.StringProperty()
    create_date = db.DateTimeProperty(auto_now_add=True)
    
    summary_count = db.IntegerProperty(default=0)
    summary_pass = db.IntegerProperty(default=0)
    summary_fail = db.IntegerProperty(default=0)
    
    @property
    def id(self):
        return str(self.key())
    
    def build_index(self):
        from trac import client_settings, DIFF_SETTING
        project = self.parent()
        oldIndex = TestBuildIndex.all().ancestor(project).filter("build =", self)
        for e in oldIndex: e.delete()
        
        settings = client_settings(None, project, DIFF_SETTING)
        
        items = []
        for f in settings.settings:
            if not hasattr(self, f):continue
            items.append("%s:%s" % (f, getattr(self, f)))
        
        from utils import md5
        logging.debug("start build index for build '%s' in '%s'" % (self.build_name, project.name))
        #indexes = self.__full_combine(items)
        for i in self.__full_combine(items):
            logging.debug("index:%s" % i)
            TestBuildIndex(parent=project, build=self, 
                           index=md5(i),
                           create_date=self.create_date).put()
            
        logging.debug("done to build index.")
        
    def __full_combine(self, cells):
        #r = list(cells)
        cells = [ (e, cells[e]) for e in range(len(cells)) ]
        results = list(cells)
        for i, e in cells:
            for j, r in list(results):
                if i <= j: continue
                results.append((i, "%s;%s" % (r, e)))
        
        return [ v for i, v in results ] + ['']
        

class TestBuildIndex(db.Model):
    """a index for search test build.
    """
    build = db.ReferenceProperty(RobotTestBuild)
    index = db.StringProperty()
    create_date = db.DateTimeProperty()

# parent is Project.
class RobotTest(db.Model):
    caseid = db.StringProperty(required=True)
    suite = db.SelfReferenceProperty()
    
    reportid = db.StringProperty()
    longname = db.StringProperty()
    testname = db.StringProperty()
    suitename = db.StringProperty()

    author = db.StringProperty()
    documents = db.Text()
    tags = db.StringProperty()
    #last_build, 

# parent is Project.
class RobotResult(db.Model):
    uuid = db.StringProperty(required=True)
    build = db.ReferenceProperty(RobotTestBuild)
    case = db.ReferenceProperty(RobotTest)
    reportid = db.StringProperty()
    
    starttime = db.StringProperty()
    endtime = db.StringProperty()
    elapsed = db.IntegerProperty(default=0)
    url = db.StringProperty()
    status = db.StringProperty()
    message = db.Text()
    tags = db.StringProperty()

    create_date = db.DateTimeProperty(auto_now_add=True)
    
    def __getattr__(self, name):
        #'caseid', 
        if name in ['longname', 'suitename', 'testname']:
            return self.case and getattr(self.case, name) or ""
        else:
            db.Model.__getattr__(self, name)
    
# parent is Project.
class RobotTrac(db.Model):
    uuid = db.StringProperty(required=True)
    
    action = db.StringProperty()
    
    bugid = db.StringProperty()
    text = db.Text()
    username = db.StringProperty()
    create_date = db.DateTimeProperty(auto_now_add=True)

# parent is Project.
class TracLogging(db.Model):
    #uuid = db.StringProperty(required=True)    
    action = db.StringProperty()
    #text = db.Text()
    text = db.StringProperty()
    username = db.StringProperty()
    create_date = db.DateTimeProperty(auto_now_add=True)
    
    @staticmethod
    def log(project, text2, action="INFO", username="SYSTEM"):
        logging.info("text:%s" % text2)
        TracLogging(parent=project, action=action,
                    text=text2,
                    username=username).put()
    
    
    