# -*- coding: utf-8 -*-
from django.db import models
import logging

# Create your models here.
class User(models.Model):
    """平台帐号信息"""
    class Meta:
        db_table = 'weibo_user'

    email = models.CharField(max_length=64, unique=True)
    screen_name = models.CharField(max_length=64, )
    password = models.CharField(max_length=64, )
    
    level = models.CharField(max_length=16, 
                             default="level1",
                             choices=(('level1', "普通会员"),
                                      ('level2', "高级会员"),
                                     )
                            )
    
    balance = models.IntegerField(help_text='帐户余额')
        
    update_time = models.DateTimeField('update time', auto_now=True)
    create_time = models.DateTimeField('create time', auto_now_add=True)
    
    def __unicode__(self):
        #logging.info("xxxx:%s" % type(self.screen_name))
        #return type(self.screen_name #self.screen_name #.encode("utf8")
        return self.email
    
    def __str__(self):
        return unicode(self.screen_name)

class WeiboProfile(models.Model):
    """围脖帐号信息"""
    class Meta:
        db_table = 'weibo_profile'
    #user_id = 
    user = models.ForeignKey('User')
    weibo_id = models.CharField(max_length=64)
    app_key = models.CharField(max_length=64)
    app_token = models.CharField(max_length=64)
    authon_token = models.CharField(max_length=64)
    authon_key = models.CharField(max_length=64)

    weibo_source = models.CharField(max_length=16,
                                      default="sina",
                                      choices=(('sina', "sina"),
                                               ('qq', "qq"),
                                               )
                                    )

    is_vip = models.IntegerField()
    fans_count = models.IntegerField()
    details = models.TextField()
    
    update_time = models.DateTimeField('update time', auto_now=True)
    create_time = models.DateTimeField('create time', auto_now_add=True)
    
    def __unicode__(self):
        return self.weibo_id    
    
class WeiboTask(models.Model):
    class Meta:
        db_table = 'weibo_task'
                    
    user = models.ForeignKey('User')
    content = models.TextField(max_length=255, help_text='任务内容。')
    image_url = models.CharField(max_length=512, help_text='任务图片。')

    desc = models.TextField(help_text='任务说明。')
            
    tags = models.CharField(max_length=255)

    task_status = models.CharField(max_length=5,
                                   default="S1",
                                   choices=(('S1', "竞标中"),
                                            ('S2', "开始进行"),
                                            ('S3', "时间结束"),
                                            ('S4', "完成"), 
                                         )                                
                                   )
    
    task_type = models.CharField(max_length=5,
                                   default="S1",
                                   choices=(('S1', "指定中标"),
                                            ('S2', "任意提交"),
                                         )                         
                                 )
    
    pay_type = models.CharField(max_length=5,
                                   default="S1",)

    pay_status = models.CharField(max_length=5,
                                   default="S1",
                                   choices=(('S1', "未交保证金"),
                                            ('S2', "已交保证金"),
                                         )                                   
                                   )
    #price = models.IntegerField()    
    #10:10000~100000
    #10:10000~100000
    #100:10000~100000
    #price = models.CharField(max_length=255)
    price_1 = models.IntegerField(help_text='0~10000。')
    price_2 = models.IntegerField(help_text='10,000~100,000。')
    price_3 = models.IntegerField(help_text='100,000~1000,000。')
    
    #竞标结束时间
    finish_time = models.DateTimeField('finish time', null=True)
        
    update_time = models.DateTimeField('update time', auto_now=True)
    create_time = models.DateTimeField('create time', auto_now_add=True)
    
    def __unicode__(self):
        return u"task_%s" % self.id    
    
class TaskComment(models.Model):
    class Meta:
        db_table = 'task_comment'
                    
    user = models.ForeignKey('User')
    task = models.ForeignKey('WeiboTask')

    comment_type = models.CharField(max_length=5,
                                   default="S1",
                                   choices=(('S1', "普通"),
                                            ('S2', "申请竞标"), 
                                            )
                                           )
        
    desc = models.TextField(help_text='内容。')
    
    create_time = models.DateTimeField('create time', auto_now_add=True)
    
    def __unicode__(self):
        return u"task_%s_%s" % (self.id, self.user.screen_name)
    
class TaskContract(models.Model):
    """任务合同"""
    class Meta:
        db_table = 'task_contract'
        
    user = models.ForeignKey('User')
    task = models.ForeignKey('WeiboTask')
    
    desc = models.TextField()
    
    status = models.CharField(max_length=5,
                              default="S1",
                               choices=(('S1', "申请"),
                                        ('S2', "中标"),
                                        ('S3', "提交"),
                                        ('S4', "付费"), 
                                        )
                               )
    
    task_deliver = models.TextField(help_text='任务交互地址。')
        
    update_time = models.DateTimeField('update time', auto_now=True)
    create_time = models.DateTimeField('create time', auto_now_add=True)
    
    def __unicode__(self):
        return u"task_%s_%s" % (self.id, self.user.screen_name)      
    
class UploadFile(models.Model):
    """上传附件"""
    class Meta:
        db_table = 'task_uploaded'
    user = models.ForeignKey('User')
    path = models.CharField(max_length=512, )
    external_link = models.CharField(max_length=512, )        
    desc = models.TextField()
    
    ref_id = models.CharField(max_length=64, )
    
    update_time = models.DateTimeField('update time', auto_now=True)
    create_time = models.DateTimeField('create time', auto_now_add=True)

    def __unicode__(self):
        return self.path
    

class UserTransaction(models.Model):
    """帐号交易金额清单"""
    class Meta:
        db_table = 'task_transaction'
        
    user = models.ForeignKey('User')
    action = models.CharField(max_length=5,
                              default="S1",
                              choices=(('S1', "充值"),
                                        ('S2', "支付任务"),
                                        ('S3', "任务收入"),
                                        ('S4', "提款"), 
                                        )
                              )
    count = models.IntegerField(help_text='当前发生金额')
    balance = models.IntegerField(help_text='交易后的余额')
    
    transaction_id = models.CharField(max_length=64, help_text='交易凭证ID')
        
    create_time = models.DateTimeField('create time', auto_now_add=True)
    
    def __unicode__(self):
        return u"t_%s_%s" % (self.action, self.user.screen_name)
    
        