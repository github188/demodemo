# -*- coding: utf-8 -*-
from django.db import models

# Create your models here.
class User(models.Model):
    
    name = models.CharField(max_length=32)
    password = models.CharField(max_length=16)

    truename = models.CharField(max_length=32)
    
    email = models.CharField(max_length=64)
    mobile = models.CharField(max_length=16)
    qq = models.CharField(max_length=16)
    
    status = models.CharField(max_length=16)
    logo = models.CharField(max_length=255, default='/images/header.gif')
    
    create_date = models.DateTimeField('create date', auto_now_add=True)
    last_login = models.DateTimeField('last login', auto_now_add=True)
    last2_login = models.DateTimeField('last2 login', auto_now_add=True)
    
    def __unicode__(self):
        return "%s<%s>" % (self.id, self.name)
    
    @property
    def user_id(self): return self.id
    @property
    def logo_url(self): return self.logo
    
class Group(models.Model):
    creator = models.ForeignKey(User)
    
    name = models.CharField(max_length=64)
    description = models.TextField()
    create_date = models.DateTimeField('create date', auto_now_add=True)
    
    status = models.CharField(max_length=1, 
                          choices=(('0', "任何人都可加入"),
                                   ('1', "邀请的人才能加入"), 
                                   ('2', "验证后可以加入"),
                                   ('3', "任何人都不能加入"),
                                   ))
    
    def __unicode__(self):
        return "%s<%s>" % (self.id, self.name)    
    
    def status_view(self,):
        return {'0':"任何人都可加入",
                '1':"邀请的人才能加入",
                '2':"验证后可以加入",
                '3':"任何人都不能加入",
                }.get(self.status)
    

class GroupMember(models.Model):
    group = models.ForeignKey(Group)
    member = models.ForeignKey(User)
    
    truename = models.CharField(max_length=32)
    email = models.CharField(max_length=64)
    mobile = models.CharField(max_length=16)
    status = models.CharField(max_length=1, choices=(('0', "正常"),
                                   ('1', "屏蔽"),
                                   ), default='0')
    qq = models.CharField(max_length=16)

    create_date = models.DateTimeField('create date', auto_now_add=True)

    last_message_id = models.IntegerField(default=0)
    last_update = models.DateTimeField('last_update', auto_now_add=True)
    
    def __unicode__(self):
        return "%s<%s>" % (self.member, self.group)
    
    def __getattr__(self, name):
        if name == 'last_message':
            try:
                return GroupMessage.objects.get(id=self.last_message_id)
            except:
                return None
            
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
    def logo_url(self): return self.member.logo
    
class GroupInvitedMember(models.Model):
    group = models.ForeignKey(Group, null=True)
    invitor = models.ForeignKey(User, null=True)
    
    truename = models.CharField(max_length=32, null=True)
    email = models.CharField(max_length=64, null=True)
    mobile = models.CharField(max_length=16, null=True)
    qq = models.CharField(max_length=16, null=True)
    status = models.CharField(max_length=1, choices=(('0', "未注册"),
                                   ('1', "已加入"), 
                                   ), default='0')
    create_date = models.DateTimeField('create date', auto_now_add=True)
    user_id = models.IntegerField(default=0)
    
    def status_view(self):
        return {'0':"未注册",
                '1':"已加入",
                }.get(self.status)
    
class GroupMessage(models.Model):
    
    author = models.ForeignKey(GroupMember)
    group = models.ForeignKey(Group)
    reply_id = models.IntegerField(default=0)

    category = models.CharField(max_length=8)
    text = models.TextField()
    create_date = models.DateTimeField('create date', auto_now_add=True)
    weight = models.IntegerField(default=0)
    status = models.CharField(max_length=1, choices=(('0', "正常"),
                                   ('1', "屏蔽"),
                                   ), default='0')
    
    def __unicode__(self):
        return "[%s]%s<%s>" % (self.category, self.text[:20], self.author)        

from django.db.models.fields.related import ManyToManyRel
class MyMessage(models.Model):
    author = models.ForeignKey(User)
    followed_id = models.IntegerField(default=0)
    reply_id = models.IntegerField(default=0)

    category = models.CharField(max_length=8)
    text = models.TextField()
    create_date = models.DateTimeField('create date', auto_now_add=True)
    weight = models.IntegerField(default=0)
    status = models.CharField(max_length=1, choices=(('0', "正常"),
                                   ('1', "屏蔽"),
                                   ), default='0')
    
    def __unicode__(self):
        return "[%s]%s<%s>" % (self.category, self.text[:20], self.author)

class FollowShip(models.Model):
    owner = models.ForeignKey(User)
    followed_id = models.IntegerField(default=0)

    truename = models.CharField(max_length=32, null=True)
    status = models.CharField(max_length=1, choices=(('0', "正常"),
                                   ('1', "屏蔽"), 
                                   ), default='0')
    
    def status_view(self):
        return {'0':"正常",
                '1':"屏蔽",
                }.get(self.status)

class Greeting(models.Model):
    sender = models.ForeignKey(User)
    receiver_id = models.IntegerField(default=0)
    group = models.ForeignKey(Group, null=True)
    
    action = models.CharField(max_length=1, choices=(('0', "问候"), 
                                   ), default='0')
    
    create_date = models.DateTimeField('create date', auto_now_add=True)
    
class GroupVote(models.Model):
    voter = models.ForeignKey(User)
    message = models.ForeignKey(GroupMessage)
    
    action = models.CharField(max_length=1, choices=(('0', "问候"), 
                                   ), default='0')
    value = models.IntegerField(default=0)
    
    create_date = models.DateTimeField('create date', auto_now_add=True)
    
class UploadedFile(models.Model):
    owner = models.ForeignKey(User)
    path = models.CharField(max_length=255, )
    create_date = models.DateTimeField('create date', auto_now_add=True)
    
    