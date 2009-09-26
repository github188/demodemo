# -*- coding: utf-8 -*-
#from django.db import models
from google.appengine.ext import db

# Create your models here.
class User(db.Model):
    
    name = db.StringProperty(required=True)
    password = db.StringProperty(required=True)

    truename = db.StringProperty(required=True)
    
    email = db.StringProperty()
    mobile = db.StringProperty()
    qq = db.StringProperty()
    
    status = db.StringProperty()
    logo = db.BlobProperty()
    #logo = models.CharField(max_length=255, default='/images/header.gif')
    
    create_date = db.DateTimeProperty(auto_now_add=True)
    last_login = db.DateTimeProperty(auto_now_add=True)
    last2_login = db.DateTimeProperty(auto_now_add=True)
    
    def __unicode__(self):
        return "%s<%s>" % (self.name, self.truename)
    
    @property
    def user_id(self): return self.id
    @property
    def logo_url(self): 
        return "/user_img?id=%s" % self.id
    
    @property
    def id(self): return self.is_saved() and self.key().id() or None
    
    @staticmethod
    def load(k):
        return db.get(db.Key.from_path('User', int(k)))
    
    def __str__(self):
        return "%s<%s>" % (self.name, self.truename)
    
class Group(db.Model):
    creator = db.ReferenceProperty(User)
    
    name = db.StringProperty(required=True)
    description = db.StringProperty(multiline=True)
    create_date = db.DateTimeProperty(auto_now_add=True)
    
    #('0', "任何人都可加入"),
    #('1', "邀请的人才能加入"), 
    #('2', "验证后可以加入"),
    #('3', "任何人都不能加入"),    
    status = db.StringProperty(default='0')
    
    def __unicode__(self):
        return "%s<%s>" % (self.id, self.name)    
    
    def status_view(self,):
        return {'0':"任何人都可加入",
                '1':"邀请的人才能加入",
                '2':"验证后可以加入",
                '3':"任何人都不能加入",
                }.get(self.status)
    @property
    def id(self): return self.is_saved() and self.key().id() or None
    
    @staticmethod
    def load(k):
        return db.get(db.Key.from_path('Group', int(k)))    

class GroupMember(db.Model):
    #used parent to relation with Group?
    #group = db.ReferenceProperty(Group)
    member = db.ReferenceProperty(User)
    
    truename = db.StringProperty()
    email = db.StringProperty()
    mobile = db.StringProperty()
    #('0', "正常"),
    #('1', "屏蔽"),
    status = db.StringProperty(default='0')
    
    qq = db.StringProperty()

    create_date = db.DateTimeProperty(auto_now_add=True)

    last_message_id = db.StringProperty()
    last_update = db.DateTimeProperty(auto_now_add=True)
    
    def __unicode__(self):
        return "%s<%s>" % (self.member, self.group)
    
    def __getattr__(self, name):
        if name == 'last_message':
            try:
                if self.last_message_id.isdigit():
                    return GroupMessage.load(self.parent().id, self.last_message_id)
                else:
                    return db.get(db.Key(self.last_message_id))
            except:
                raise
                #return None
            
        raise AttributeError, "GroupMember object has no attribute '%s'" % name
    
    @property
    def truename_v(self): return self.truename or self.member.truename  
    @property
    def email_v(self): return self.email or self.member.email  
    @property
    def mobile_v(self): return self.mobile or self.member.mobile  
    @property
    def qq_v(self): return self.qq or self.member.qq  
    @property
    def user_id(self): return self.member.id
    @property
    def logo_url(self): return self.member.logo_url 
    @property
    def id(self): return self.is_saved() and self.key().id() or None
    
    @staticmethod
    def load(g, k):
        return db.get(db.Key.from_path('Group',int(g), 'GroupMember', int(k)))
    
    @property
    def group(self): return self.parent()
          
    
class GroupInvitedMember(db.Model):
    #used parent to relation with Group?
    #group = db.ReferenceProperty(Group, required=True)
    invitor = db.ReferenceProperty(User, required=True)
    
    truename = db.StringProperty()
    email = db.StringProperty()
    mobile = db.StringProperty()
    qq = db.StringProperty()
    
    #('0', "未注册"),
    #('1', "已加入"),
    status = db.StringProperty(default='0')
        
    create_date = db.DateTimeProperty(auto_now_add=True)
    user_id = db.StringProperty()
    
    def status_view(self):
        return {'0':"未注册",
                '1':"已加入",
                }.get(self.status)
    @property
    def id(self): return self.is_saved() and self.key().id() or None
    
    @staticmethod
    def load(g, k):
        return db.get(db.Key.from_path('Group',int(g), 'GroupInvitedMember', int(k)))
                
    
class GroupMessage(db.Model):
    #used parent to relation with Group?
    #group = db.ReferenceProperty(Group)
    author = db.ReferenceProperty(GroupMember)
    reply = db.SelfReferenceProperty()

    category = db.StringProperty()
    text = db.StringProperty(multiline=True)
    create_date = db.DateTimeProperty(auto_now_add=True)
    weight = db.IntegerProperty(default=0)
    #('0', "正常"),
    #('1', "屏蔽"),
    status = db.StringProperty(default='0')    
    
    def __unicode__(self):
        return "[%s]%s<%s>" % (self.category, self.text[:20], self.author)
    
    @property
    def id(self): return self.is_saved() and self.key().id() or None
    @staticmethod
    def load(g, k):
        return db.get(db.Key.from_path('Group',int(g), 'GroupMessage', int(k)))
    
    @property
    def group(self): return self.parent()

class Greeting(db.Model):
    #used parent to relation with Group?
    #group = db.ReferenceProperty(Group)
    sender = db.ReferenceProperty(User)
    receiver = db.ReferenceProperty(User, collection_name='greetingme_list')
    
    action = db.StringProperty(default='0')
    
    create_date = db.DateTimeProperty(auto_now_add=True)
    
    @property
    def id(self): return self.is_saved() and self.key().id() or None
    
    @staticmethod
    def load(g, k):
        return db.get(db.Key.from_path('Group',int(g),'Greeting', int(k)))

    @property
    def group(self): return self.parent()    
    
class GroupVote(db.Model):
    voter = db.ReferenceProperty(User)
    message = db.ReferenceProperty(GroupMessage)
    
    value = db.IntegerProperty(default=0)
    
    create_date = db.DateTimeProperty(auto_now_add=True)
    
    @property
    def id(self): return self.is_saved() and self.key().id() or None
    @staticmethod
    def load(k):
        return db.get(db.Key.from_path('GroupVote', int(k)))
    
    @property
    def group(self): return self.parent()    
    

    
    