from django.db import models
from datetime import datetime
import hashlib

class Activity(models.Model):
    group = models.CharField(max_length=32, )
    
    believer = models.CharField(max_length=32, )
    ipaddress = models.CharField(max_length=20, )
    timestamp = models.CharField(max_length=20, )
    
    #actionfrom = models.CharField(max_length=10)
    source = models.CharField(max_length=50)
    #track, error, info, debug
    category = models.CharField(max_length=10)
    action = models.CharField(max_length=1000)
    
    def __unicode__(self):
        return "%s<%s>" % (self.group, self.action)

#class 
