# -*- coding: utf-8 -*-
from django.db import models
import logging

try:
    import json
except:
    import simplejson as json

PARAM_GROUP = (('weibo', "WeiBo"),
                ('site', "网站"),
               )

class ParameterDefine(models.Model):
    """需要配置的参数定义。"""
    class Meta:
        db_table = 'param_define'
        verbose_name_plural = '参数定义'
        verbose_name = '参数定义'

    group = models.CharField(max_length=10,
                             default="weibo",
                             choices= PARAM_GROUP,
                             verbose_name="参数分组",
                             help_text='可以定义不同类型的参数分组，例如，围脖的参数，网站的参数，广告的参数等。'
                           )
    uuid = models.CharField(max_length=32, primary_key=True, 
                            verbose_name="参数名称", 
                            help_text='在网站代码里面，需要使用到此名称。 例如：site_title, site_keywords。')
    
    details = models.TextField(verbose_name="参数说明",
                               help_text='在配置页面会显示，参数的描述。',
                                blank=True)

    input_type = models.CharField(max_length=12,
                                  verbose_name="参数的类型",
                                  choices=(('text', "text"),
                                           ('textarea', "textarea"),
                                           ('select', "select"),
                                           ('checkbox', "checkbox"),
                                      ),                                  
                                  help_text='参数的输入类型',
                                  default='text',
                                 )    

    default_val = models.TextField(max_length=512,
                                   verbose_name="默认值", 
                                   help_text='参数的默认值',
                                   blank=True)
    
    update_time = models.DateTimeField('update time', auto_now=True)
    create_time = models.DateTimeField('create time', auto_now_add=True)
    
    
class ParameterValue(models.Model):
    """参数的具体配置至。"""
    
    class Meta:
        db_table = 'param_value'
        verbose_name_plural = '网站参数'
        verbose_name = '网站参数'

    site_url = models.CharField(max_length=256, unique=True, 
                                verbose_name="网站地址", 
                                help_text='配置的唯一地址，需要使用这个地址做Cache清理操作，一定要配置正确。例如：http://www.littlefun.net/apps/yuanfen/sina')
    
    group = models.CharField(max_length=10,
                             default="weibo",
                             choices= PARAM_GROUP,
                             verbose_name="参数分组",
                             help_text='可以定义不同类型的参数分组，例如，围脖的参数，网站的参数，广告的参数等。'
                           )
    values = models.TextField(verbose_name="参数值",
                              help_text='参数的具体值')
    
    update_time = models.DateTimeField('update time', auto_now=True)
    create_time = models.DateTimeField('create time', auto_now_add=True)
    
    