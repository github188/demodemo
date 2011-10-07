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
                                          # ('checkbox', "checkbox"),
                                      ),                                  
                                  help_text='参数的输入类型',
                                  default='text',
                                 )    

    default_val = models.TextField(max_length=512,
                                   verbose_name="默认值", 
                                   help_text='参数的默认值',
                                   blank=True)

    view_order = models.IntegerField(verbose_name="显示排序", 
                                     help_text='在参数配置页面的位置（升序排列）. 如果多个参数位置相同，按照创建时间排序。',
                                     default=0)
    
    update_time = models.DateTimeField('update time', auto_now=True)
    create_time = models.DateTimeField('create time', auto_now_add=True)
    
    @staticmethod
    def load_param_group(group):
        ps = ParameterDefine.objects.filter(group=group).order_by("view_order", "-create_time") #.order_by("-create_time")
        return ps
    
    @property
    def default_value(self):
        if self.input_type == 'select':
            return self.default_val.split("\n")[0]
        else:
            return self.default_val
        
    @property
    def value_choices(self):
        return ((e, e) for e in self.default_val.split("\n"))
        
    def get_field(self):
        from django.forms.fields import Field, ChoiceField
        from django.forms.widgets import Textarea
        
        cls = Field
        attrs = {'required':False,
                 'initial':self.default_value,
                 'help_text': self.details,
                 'label': self.uuid,
                 }
        
        if self.input_type == 'select':
            attrs['choices'] = self.value_choices
            cls = ChoiceField
        if self.input_type == 'textarea':
            #attrs['rows'] = '6'
            #attrs['cols'] = '80'
            attrs['widget'] = Textarea({'rows':6, 'cols':80})
        
        return cls(**attrs)
    
    def __unicode__(self):
        return self.uuid
    
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
    
    
    def edit_param(self):
        from Weibo.settings import APP_ROOT
        return """<a href='%s/config/param_edit?pid=%s'>修改参数</a>""" \
        % (APP_ROOT, self.id)
        
    edit_param.allow_tags = True
    #edit_param.verbose_name = "文件下载"
    #edit_param.verbose_name = "告警类型"
    
    @property
    def dict_values(self):
        v = {}
        try:
            v = json.loads(self.values)
        except:
            pass
        return v
    
    def clean_site_cache(self):
        pass
    
    def update_values(self, vals):
        new_values = self.dict_values
        new_values.update(vals)
        self.values = json.dumps(new_values)        
        
    def __unicode__(self):
        return self.site_url
        
    def __str__(self):
        return self.site_url
    