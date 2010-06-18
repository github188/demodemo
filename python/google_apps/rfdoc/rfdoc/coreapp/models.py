# -*- coding: utf-8 -*-
#from django.db import models
from google.appengine.ext import db
import logging
import search

class LibraryGroup(db.Model):
    name = db.StringProperty(required=True)
    create_user = db.UserProperty(auto_current_user_add=True)
    #a project key for distinguish project.
    admin_list = db.StringProperty(multiline=True)
    #prj_key = db.StringProperty()
    create_date = db.DateTimeProperty(auto_now_add=True)
    active = db.IntegerProperty(default=0)
    
    #private, public
    view_level = db.StringProperty()
    
    def __str__(self):
        return self.name
    
    @staticmethod
    def get_or_create(name):
        group = LibraryGroup.all().filter("name =", name).get()
        if group is None:
            group = LibraryGroup(name=name)
            group.put()
        return group

# parent is LibraryGroup.   
class Library(db.Model):
    name = db.StringProperty(required=True)
    doc = db.TextProperty()
    version = db.StringProperty()
    author = db.StringProperty()
    type = db.StringProperty()
    
    def __unicode__(self):
        return self.name
    
    @staticmethod
    def get_by_name(parent, name):
        return Library.all().ancestor(parent).filter("name =", name).get()
        

# parent is LibraryGroup.    
class Keyword(search.SearchableModel):
    library = db.ReferenceProperty(Library)
    name = db.StringProperty(required=True)
    doc = db.TextProperty()
    version = db.StringProperty()
    author = db.StringProperty()
    
    #mml = TruncateCharField(max_length=100, )
    source = db.StringProperty()
    md5 = db.StringProperty()
    status = db.StringProperty()
    args = db.StringProperty()

    def __unicode__(self):
        return self.name
    
    @classmethod
    def SearchableProperties(cls):
      return [['name', 'doc'], ]

# parent is LibraryGroup.   
class LibraryInit(db.Model):
    library = db.ReferenceProperty(Library, collection_name="init_set")
    name = db.StringProperty()
    doc = db.TextProperty()
    args = db.StringProperty()
 
    def __unicode__(self):
        return self.name
