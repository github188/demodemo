# -*- coding: utf-8 -*-
from google.appengine.ext import db
import logging    

# parent is Project.
class BookUser(db.Model):
    name = db.StringProperty(required=True) #setting name
    balance = db.FloatProperty(default=0.0)
    
    discount = db.FloatProperty(default=1.0)
    create_date = db.DateTimeProperty(auto_now_add=True)
    lastupdate = db.DateTimeProperty()
    list_order = db.IntegerProperty(default=0)
    
    @property
    def id(self): return self.is_saved() and self.key().id() or None
    
    @property
    def path_id(self):
        return self.is_saved() and "%s,%s" % (self.parent().id, self.key().id()) or None
        
    @staticmethod
    def load(k):
        book, user = k.split(",")
        return db.get(db.Key.from_path('BookAccount', int(book), 'BookUser', int(user)))
        

class BookAccount(db.Model):
    name = db.StringProperty(required=True)
    admin_user = db.StringProperty()
    admin_password = db.StringProperty()

    account = db.ReferenceProperty(BookUser)
    
    book_key = db.StringProperty()
    create_date = db.DateTimeProperty(auto_now_add=True)
    active = db.IntegerProperty(default=0)
    
    @property
    def id(self): return self.is_saved() and self.key().id() or None
    
    @staticmethod
    def load(k):
        return db.get(db.Key.from_path('BookAccount', int(k)))
    
    def __str__(self):
        return self.name

# parent is Project.
class BookAccountRecord(db.Model):    
    book_date = db.DateTimeProperty()
    action = db.StringProperty(required=True) #充值 套现 转账 消费 校对
    host = db.StringProperty(required=True)
    people = db.StringProperty(required=True)
    expense = db.FloatProperty(default=0.0)
    
    comment = db.TextProperty(default="")
        
    create_date = db.DateTimeProperty(auto_now_add=True)
    #lastupdate = db.DateTimeProperty()
    
    @property
    def id(self): return self.is_saved() and self.key().id() or None
    

# parent is Project.
class BookUserRecord(db.Model):
    bookrecord = db.ReferenceProperty(BookAccountRecord)
    user = db.ReferenceProperty(BookUser)
    action = db.StringProperty() #充值 套现 转账 消费
    expense = db.FloatProperty(default=0.0)
    balance = db.FloatProperty(default=0.0)
    
    comment = db.TextProperty(default="")
    create_date = db.DateTimeProperty(auto_now_add=True)
    #lastupdate = db.DateTimeProperty()
    
    @property
    def id(self): return self.is_saved() and self.key().id() or None
