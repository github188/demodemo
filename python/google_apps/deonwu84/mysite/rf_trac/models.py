# -*- coding: utf-8 -*-
#from django.db import models
from google.appengine.ext import db

class RobotProject(db.Model):
    name = db.StringProperty(required=True)
    admin_user = db.StringProperty()
    admin_password = db.StringProperty()

    #a project key for distinguish project.
    prj_key = db.StringProperty()
    status_list = db.StringProperty(multiline=True)
    create_date = db.DateTimeProperty(auto_now_add=True)
    active = db.IntegerProperty(default=0)

# parent is Project.
class RobotResult(db.Model):
    uuid = db.StringProperty(required=True)
    longname = db.StringProperty()
    start_time = db.StringProperty()
    end_time = db.StringProperty()
    
    url = db.StringProperty()
    error = db.Text()
    status = db.StringProperty()
    create_date = db.DateTimeProperty(auto_now_add=True)
    
# parent is Project.
class RobotTrac(db.Model):
    uuid = db.StringProperty(required=True)
    
    action = db.StringProperty()
    
    bugid = db.StringProperty() 
    text = db.Text()
    username = db.StringProperty()
    create_date = db.DateTimeProperty(auto_now_add=True)
    

    

    
    