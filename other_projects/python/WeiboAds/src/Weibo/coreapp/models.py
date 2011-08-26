# -*- coding: utf-8 -*-
from django.db import models
import logging

# Create your models here.
class User(models.Model):
    """平台帐号信息"""
    class Meta:
        db_table = 'weibo_user'
        verbose_name_plural = '用户信息'
        verbose_name = '用户信息'        

    email = models.CharField(max_length=64, unique=True)
    screen_name = models.CharField(max_length=64, )
    password = models.CharField(max_length=64, )
    
    level = models.CharField(max_length=16, 
                             default="level1",
                             choices=(('level1', "普通会员"),
                                      ('level2', "高级会员"),
                                     )
                            )
    
    balance = models.IntegerField(help_text='帐户余额', default=0)
        
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
        verbose_name_plural = '围脖资料'
        verbose_name = '围脖资料'        
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

    is_vip = models.IntegerField(default = 0)
    fans_count = models.IntegerField()
    details = models.TextField(default = "")
    
    update_time = models.DateTimeField('update time', auto_now=True)
    create_time = models.DateTimeField('create time', auto_now_add=True)
    
    @staticmethod
    def import_from_sina(user, auth):
        p = WeiboProfile.objects.filter(weibo_source='sina', weibo_id=user.id)[:1]
        if p:
            p = p[0]
            p.app_key = auth._consumer.key
            p.app_token = auth._consumer.secret
            p.authon_token = auth.access_token.secret
            p.authon_key = auth.access_token.key
            p.fans_count = int(user.followers_count)
            p.save()
        else:
            p = WeiboProfile(weibo_source='sina', weibo_id=user.id)
            p.app_key = auth._consumer.key
            p.app_token = auth._consumer.secret
            p.authon_token = auth.access_token.secret
            p.authon_key = auth.access_token.key
            p.fans_count = int(user.followers_count)
            u, c = User.objects.get_or_create(email = user.id)
            u.screen_name = user.screen_name
            u.save()
            p.user = u
            p.save()
        return p
    
    def __unicode__(self):
        return self.weibo_id
    
class WeiboTask(models.Model):
    class Meta:
        db_table = 'weibo_task'
        verbose_name_plural = '任务列表'
        verbose_name = '任务列表'        
                    
    user = models.ForeignKey('User')
    
    task_type = models.CharField(max_length=5,
                                 default="post",
                                 choices=(('post', "直发"),
                                          ('reply', "转发"),
                                         ),
                                 verbose_name="任务类型",
                                 help_text='直发-由接受任务的手动发送到围脖。转发-广告主提供已存在的围脖类容。'
                                 )
    
    content = models.TextField(max_length=255, verbose_name="任务内容", help_text='任务内容。')
    image_url = models.CharField(max_length=512, verbose_name="图片连接", help_text='图片连接。')

    desc = models.TextField(verbose_name="任务说明", help_text='可以增加些特殊需求.')
            
    tags = models.CharField(max_length=255, help_text='用来分类过滤任务列表.')

    task_status = models.CharField(max_length=5,
                                   default="S1",
                                   choices=(('S1', "竞标中"),
                                            ('S2', "开始进行"),
                                            ('S3', "时间结束"),
                                            ('S4', "完成"), 
                                         )                                
                                   )
    
    contract_type = models.CharField(max_length=5,
                                   default="S1",
                                   choices=(('S1', "指定中标"),
                                            ('S2', "任意提交"),
                                         ),
                                    help_text='合同交易方式。'
                                 )
    
    pay_type = models.CharField(max_length=5, default="S1",)

    pay_status = models.CharField(max_length=5,
                                   default="S1",
                                   choices=(('S1', "未交保证金"),
                                            ('S2', "已交保证金"),
                                         )                                   
                                   )
    
    price_1 = models.IntegerField(verbose_name="价格1", help_text='0~1万粉丝的价格')
    price_2 = models.IntegerField(verbose_name="价格2", help_text='1万~10万粉丝的价格')
    price_3 = models.IntegerField(verbose_name="价格3", help_text='10万~以上粉丝的价格')
    
    #竞标结束时间
    finish_time = models.DateTimeField('finish time', null=True)
        
    update_time = models.DateTimeField('update time', auto_now=True)
    create_time = models.DateTimeField('create time', auto_now_add=True)
    
    def __unicode__(self):
        return u"task_%s" % self.id    
    
class TaskComment(models.Model):
    class Meta:
        db_table = 'task_comment'
        verbose_name_plural = '任务回复'
        verbose_name = '任务回复'
                    
    user = models.ForeignKey('User')
    task = models.ForeignKey('WeiboTask')
    contract = models.OneToOneField('TaskContract', null=True)

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
        verbose_name_plural = '交易合同'
        verbose_name = '交易合同'      
        
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
        verbose_name_plural = '任务附件'
        verbose_name = '任务附件'
                
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
        verbose_name_plural = '资金流转记录'
        verbose_name = '资金流转记录'        
        
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
    
        