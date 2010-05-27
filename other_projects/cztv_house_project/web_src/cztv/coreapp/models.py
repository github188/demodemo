# -*- coding: utf-8 -*-
from django.db import models

# Create your models here.
class FieldTemplate(models.Model):
    category = models.CharField(max_length=32, blank=True)
    name = models.CharField(max_length=32, unique=True)
    
    label = models.CharField(max_length=32, blank=True)
    field_type = models.CharField(max_length=32, blank=True)
    values = models.TextField(blank=True)

    description = models.TextField(blank=True)
    visiable_order = models.IntegerField(default=0)
    
class House(models.Model):
    name = models.CharField(max_length=32)
    product_address = models.CharField(max_length=255)
    
    author = models.CharField(max_length=32)
    create_date = models.DateTimeField('create date', auto_now_add=True)
    update_date = models.DateTimeField('update date', auto_now_add=True)

class HouseAttributes(models.Model):
    house = models.ForeignKey(House)
    name = models.CharField(max_length=32, )
    value = models.CharField(max_length=255)

class HouseComments(models.Model):
    house = models.ForeignKey(House)
    owner_id = models.IntegerField()
    owner_type = models.CharField(max_length=32)
    
    content = models.TextField()
    ip_addr = models.CharField(max_length=32)
    
    author = models.CharField(max_length=32)
    create_date = models.DateTimeField('create date', auto_now_add=True)
    

