# -*- coding: utf-8 -*-
from django.db import models
import logging

try:
    import json
except:
    import simplejson as json

class ProfileCredit(models.Model):
    """用户的价值评分记录"""
    class Meta:
        db_table = 'weibo_credit'
        verbose_name_plural = '围脖评级'
        verbose_name = '围脖评级'
    
    uuid = models.CharField(max_length=32, primary_key=True)
    credit_level = models.FloatField(default=0, verbose_name="最终评分")
    details = models.TextField()
    
    status = models.CharField(max_length=10,
                                 default="updated",
                                 choices=(('updated', "更新成功"),
                                          ('expired', "需要重新计算"),
                                         ),
                                 verbose_name="评级状态",
                                 help_text='价值状态，由于参数/样本的变化需要定时重新计算。'
                                 )
    
    update_time = models.DateTimeField('update time', auto_now=True)
    create_time = models.DateTimeField('create time', auto_now_add=True)
    
    def update_credit_data(self, data={}):        
        attr = self._get_details()
        attr.update(data)
        self.details = json.dumps(attr)

    def update_credit_details(self, data={}):
        attr = self._get_details()
        attr.update(data)
        self.details = json.dumps(attr)
        
    def get_credit_data(self, f):
        return self._get_details().get(f, 0)
    
    def _get_details(self):
        if not hasattr(self, "_cache_details"):
            try:
                self._cache_details = json.loads(self.details)
            except:
                self._cache_details = {}
        return self._cache_details
    
class CreditParameter(models.Model):
    """用户的价值评分参数。"""
    class Meta:
        db_table = 'weibo_credit_param'
        verbose_name_plural = '评级参数'
        verbose_name = '评级参数'

    data_source = models.CharField(max_length=10, verbose_name="数据源",
                                   default='weibo',
                                   help_text='支持对不同的围脖，采用不同的权值，和采用空间. 默认是weibo.')
    name = models.CharField(max_length=32, verbose_name="参数名",)
    weight = models.FloatField(verbose_name="评分权值")
    default_value = models.IntegerField(verbose_name="默认评分",
                                        help_text='如果用户没有此项数据时，默认取值. 用于新增参数.',
                                        default=0)
    
    total = models.IntegerField(verbose_name="样本数量", default=0)
    
    level1 = models.CharField(max_length=32, verbose_name="区间1")
    level2 = models.CharField(max_length=32, verbose_name="区间2")
    level3 = models.CharField(max_length=32, verbose_name="区间3")
    level4 = models.CharField(max_length=32, verbose_name="区间4")
    level5 = models.CharField(max_length=32, verbose_name="区间5")
    
    level6 = models.CharField(max_length=32, verbose_name="区间6")
    level7 = models.CharField(max_length=32, verbose_name="区间7")
    level8 = models.CharField(max_length=32, verbose_name="区间8")
    level9 = models.CharField(max_length=32, verbose_name="区间9")
    level10 = models.CharField(max_length=32, verbose_name="区间10")
    
    #credit_level = models.IntegerField(default=0, verbose_name="最终评分")    
    #样本取值分布, <取值>:<概率>.
    #1000:0.15096
    #2000:0.15096
    #5000:0.15096
    #details = models.TextField()
    #base_line = models.TextField()

    update_time = models.DateTimeField('update time', auto_now=True)
    create_time = models.DateTimeField('create time', auto_now_add=True)
    
    def get_data_level(self, data):
        """计算某一个参数在这一纬度的最后得分。"""
        levels = self._get_levels()
        for i in range(10):
            if data < levels[i]: break
        if i == 9 and data > levels[9]: return 100
        end = levels[i]
        start = 0 
        if i > 0: start = levels[i-1]
        return i * 10 + (data - start) * 10.0 / (end - start)
    
    def _get_levels(self):
        if not hasattr(self, '_level_cache'):
            self._level_cache = []
            for i in range(10):
                d = getattr(self, 'level%s' % (i + 1))
                self._level_cache.append(int(d.split(":")[0].strip()))        
        return self._level_cache

class CreditQueue(models.Model):
    """等待评分的围脖用户列表"""
    
    class Meta:
        db_table = 'credit_queue'
        verbose_name_plural = '围脖评级队列'
        verbose_name = '围脖评级队列'
    
    uuid = models.CharField(max_length=32, primary_key=True)
    priorty = models.IntegerField(default=0)
    create_time = models.DateTimeField('create time', auto_now_add=True)
    
class APPAccessKey(models.Model):
    """应用授权Key列表, 用来访问Weibo数据进行数据采样。"""
    
    class Meta:
        db_table = 'weibo_access_key'
        verbose_name_plural = '围脖用户访问授权'
        verbose_name = '围脖用户访问授权'
    
    app_key = models.CharField(max_length=32, )
    user_id = models.CharField(max_length=32, )
    app_secret = models.CharField(max_length=32, )
    access_key = models.CharField(max_length=32, )
    access_secret = models.CharField(max_length=32, )

    last_use_time = models.DateTimeField('update time', )
    update_time = models.DateTimeField('update time', auto_now=True)
    create_time = models.DateTimeField('create time', auto_now_add=True)

from django.db.models.signals import pre_save, post_save
from Weibo.coreapp.models import WeiboProfile
from credit_service import update_access_key, update_credit_report
post_save.connect(update_access_key, sender=WeiboProfile)
post_save.connect(update_credit_report, sender=ProfileCredit)


#参考指标？
#
#class
#100, 10%
#1000, 50%
#10000, 10%

#---2010-08-09.txt
#fans, 10000, 100
#fans, 10002, 100
#fans, 10000, 100

#reply, user_0002, 100
#reply, user_xxxx, 2000

#
#1002, fans:99, reply:88, xx, 
#活跃度， ---最近登录状况， 发围脖数量？
#活粉， ---是否有登录，转贴，
#
