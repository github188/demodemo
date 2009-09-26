# -*- coding: utf-8 -*-
from google.appengine.ext import db
import logging
import re

class ContentUser(db.Model):
    track = db.StringProperty()
    name = db.StringProperty(required=True)
    truename = db.StringProperty()
    email = db.StringProperty()
    
    create_date = db.DateTimeProperty(auto_now_add=True)
    
    @property
    def id(self): return self.is_saved() and self.key().id() or None
    

class ContentCategory(db.Model):
    lang = db.StringProperty() #zh, en    
    code = db.StringProperty() #x
    name = db.StringProperty() #x
    desc = db.StringProperty() #哲理， QQ签名， 雷人
    
    @property
    def id(self): return self.is_saved() and self.key().id() or None
    

# parent is ContentCategory.
class ContentMessage(db.Model):
    track = db.StringProperty()
    ipaddr = db.StringProperty() #
    
    user = db.ReferenceProperty(ContentUser)
    vote_weight = db.IntegerProperty(default=0)
    vote_up = db.IntegerProperty(default=0)
    vote_down = db.IntegerProperty(default=0)
    tags = db.StringProperty()
    summary = db.StringProperty(multiline=True)
    text = db.TextProperty()
    status = db.IntegerProperty(default=0)
    
    tags_keys = db.ListProperty(db.Key)
    
    create_date = db.DateTimeProperty(auto_now_add=True)
    update_date = db.DateTimeProperty()
    
    def add_tags(self, tags):
        tags = list(set(re.split(u"(\s+|　)", tags)))
        self.tags_keys = [ ContentTag.load_by_name(self.parent(), t).key() for t in tags if t.strip() ]
        self.tags = " ".join(tags)
        
    @property
    def id(self): return self.is_saved() and self.key().id() or None
    
    @staticmethod
    def load(cate, id):
        if not str(id).isdigit(): return None
        return db.get(db.Key.from_path('ContentCategory', cate.id, 
                                       'ContentMessage', int(id)))
        
    def __getattr__(self, name):
        if name == 'htmlText':
            self.htmlText = self.text.replace("\n", "<br/>")
            return self.htmlText
        
        raise AttributeError, "no attribute %s in ContentMessage" % name

# parent is ContentCategory.
class ContentTag(db.Model):
    name = db.StringProperty()
    clicked = db.IntegerProperty(default=0)
    count = db.IntegerProperty(default=0)
    order = db.IntegerProperty(default=0)
    shape = db.StringProperty() #color
    
    status = db.IntegerProperty(default=0) #not list if < 0
    update_date = db.DateTimeProperty()
    create_date = db.DateTimeProperty(auto_now_add=True)
    
    def message_query(self, where="", order = ""):
        GQL = "WHERE tags_keys = :1 "
        if where: GQL += "AND %s" % where
        if order: GQL += "ORDER BY %s" % order
        
        return ContentMessage.gql(GQL, self.key()) #.ancestor(self.parent())
        #return ContentMessage.all().ancestor(self.parent()).filter("tags_keys =", self.key())

    @staticmethod
    def load_by_name(cate, name):
        tag = ContentTag.all().ancestor(cate).filter("name =", name).get()
        if tag is None:
            tag = ContentTag(parent=cate, name=name)
            tag.put()
        return tag
    
    @property
    def id(self): return self.is_saved() and self.key().id() or None
    

# parent is ContentMessage.
class ContentVote(db.Model):
    track = db.StringProperty()
    ipaddr = db.StringProperty()
    
    voter = db.ReferenceProperty(ContentUser)
    #message = db.ReferenceProperty(ContentMessage)
    value = db.IntegerProperty(default=0)
    create_date = db.DateTimeProperty(auto_now_add=True)
    
    summary = db.StringProperty(multiline=True)
    
    @property
    def id(self): return self.is_saved() and self.key().id() or None
    
