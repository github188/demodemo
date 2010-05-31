# -*- coding: utf-8 -*-
from django.db import models

# Create your models here.
class FieldTemplate(models.Model):
    class Meta:
        verbose_name = "房产字段"
        verbose_name_plural = "房产字段" 
            
    category = models.CharField(max_length=32, blank=True)
    name = models.CharField(max_length=32, unique=True)
    
    label = models.CharField(max_length=32, blank=True)
    field_type = models.CharField(max_length=32, blank=True, choices=(("choice", "单选"),
                                                                      ("multi_choice", "多选"),
                                                                      ("text", "文本")
                                                                      ))
    values = models.TextField(blank=True)

    description = models.TextField(blank=True)
    visiable_order = models.IntegerField(default=0)
    status = models.CharField(default='Y', max_length=2, choices=(("Y", "有效",),
                                                                  ("N", "无效",)
                                                                  ))
    
class FieldCategory(models.Model):
    class Meta:
        verbose_name = "房产字段分组"
        verbose_name_plural = "房产字段分组" 
    
    name = models.CharField(max_length=32, unique=True)
    label = models.CharField(max_length=32, blank=True)
    visiable_order = models.IntegerField(default=0)
    status = models.CharField(default='Y', max_length=2, choices=(("Y", "有效",),
                                                                  ("N", "无效",)
                                                                  ))
class House(models.Model):
    class Meta:
        verbose_name = "房产信息"
        verbose_name_plural = "房产信息"
    
    name = models.CharField(max_length=32, verbose_name="物业名称")
    product_address = models.CharField(max_length=255)
    
    product_address2 = models.CharField(max_length=255)
    product_address3 = models.CharField(max_length=255)
    product_address4 = models.CharField(max_length=255)
    
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
    

