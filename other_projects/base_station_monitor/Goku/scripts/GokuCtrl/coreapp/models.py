# -*- coding: utf-8 -*-
from django.db import models

# Create your models here.
class BaseStation(models.Model):
    class Meta:
        db_table = 'base_station'
        #verbose_name = '基站列表'
        verbose_name_plural = '基站列表'
            
    uuid = models.CharField(max_length=10, primary_key=True, verbose_name="基站编号")
    name = models.CharField(max_length=50, verbose_name="基站名称")
    connectionStatus = models.CharField(max_length=10, 
                                        choices=(('connected', "连接成功"),
                                                 ('timeout', "连接超时"),
                                                 ('error', "登录错误"),
                                                 ('new', "新增"),
                                        ),
                                        default='new',
                                        verbose_name="连接状态", 
                                        )
    groupName = models.CharField(max_length=50, default='default', verbose_name="监控分组")
    routeServer = models.CharField(max_length=50, null=True, verbose_name="转发服务器", editable=False)
    locationId = models.CharField(max_length=50, default='', verbose_name="IP地址")
    alarmStatus = models.CharField(max_length=50, null=True, verbose_name="告警状态")
    channels = models.CharField(max_length=150, null=True, verbose_name="通道列表")
    
    devType = models.IntegerField(default=1,
                                  choices=((1, "视频"),
                                         (2, "图片"),
                                         ),
                                   verbose_name="设备类型"
                                  )
    
    lastActive = models.DateTimeField(null=True, verbose_name="最后活动时间", editable=False)
    lastUpdate = models.DateTimeField( auto_now=True, editable=False, verbose_name="最后更新时间")
    createDate = models.DateTimeField(auto_now_add=True, editable=False, verbose_name="创建时间")
    
    locationUUID = models.ForeignKey('Location', db_column='locationUUID', verbose_name="基站位置")
    btsCategory = models.ForeignKey('sysparam.BTSCategory', default='', db_column='btsCategory', verbose_name="端局类型")

    def __unicode__(self):
        return self.uuid        


# Create your models here.
class User(models.Model):
    class Meta:
        db_table = 'user_account'
        verbose_name_plural = '用户列表'
            
    name = models.CharField(max_length=20, primary_key=True, verbose_name="登录名")
    password = models.CharField(max_length=50, 
                                default='new', )
    display = models.CharField(max_length=50, default='default')
    lastActive = models.DateTimeField('last active', null=True)
    
    status = models.CharField(max_length=10, 
                              choices=(('ok', "正常"),
                                       ('removed', "已删除"),
                                       ('locked', "锁定"),
                                       ),
                             default='ok',
                             verbose_name="用户状态")    

    def __unicode__(self):
        return self.name        

# Create your models here.
class UserGroup(models.Model):
    class Meta:
        db_table = 'user_group'
        verbose_name_plural = '用户分组'
            
    name = models.CharField(max_length=20, primary_key=True)
    isAdmin = models.IntegerField(default=0,
                                  choices=((0, "non-Admin"),
                                           (1, "admin"),
                                          ),
                                  )
    def __unicode__(self):
        return self.name        
    
class UserGroupRelation(models.Model):
    class Meta:
        db_table = 'relation_user_group'
        unique_together = (("user", "user_group"),)
        verbose_name_plural = '用户组管理'

    user = models.ForeignKey(User)
    user_group = models.ForeignKey(UserGroup)

class StationGroupRelation(models.Model):
    class Meta:
        db_table = 'relation_station_group'
        unique_together = (("base_station", "user_group"), )
        verbose_name_plural = '基站分组'

    base_station = models.ForeignKey(BaseStation)
    user_group = models.ForeignKey(UserGroup)

class AlarmRecord(models.Model):
    class Meta:
        db_table = 'alarm_record'
        verbose_name_plural = '告警列表'
        
    uuid = models.CharField(max_length=32, primary_key=True, editable=False)

    base_station = models.ForeignKey(BaseStation, db_column='base_station', verbose_name="基站")
    channelId = models.CharField(max_length=10, default='', verbose_name="告警通道")
    alarmCode = models.ForeignKey('sysparam.AlarmDefine', db_column='alarmCode',  verbose_name="告警名称", )
    
    alarmLevel = models.CharField(max_length=10, default='', verbose_name="告警级别")
    
    #1.视频 2.图片, 3.无 
    alarmCategory = models.CharField(max_length=10,
                                     choices=(('1', "视频"),
                                       ('2', "图片"),
                                       ('3', "无视频/图片"),
                                       ),                     
                                     default='',
                                     verbose_name="告警类型"
                                     )
    
    alarmStatus = models.CharField(max_length=10, 
                                   choices=(('1', "未处理"),
                                            ('2', "告警超时自动处理"),
                                            ('3', "手动确认"),
                                            ('4', "无效告警"),
                                       ),                                   
                                   default='1',
                                   verbose_name="告警状态")    
    user = models.CharField(max_length=20, default='', verbose_name="确认人员")
    videoPath = models.CharField(max_length=1024, default='')
    startTime = models.DateTimeField(verbose_name='开始时间', null=True)
    endTime = models.DateTimeField(verbose_name='结束时间', null=True)
    lastUpdateTime = models.DateTimeField(verbose_name='最后更新时间', auto_now=True, editable=False, null=True)
    comfirmTime = models.DateTimeField(verbose_name='手动确认时间', null=True)
    
    def __unicode__(self):
        return "%s-%s" % (self.base_station, self.alarmType)
                
class SystemLog(models.Model):
    class Meta:
        db_table = 'goku_system_log'
        verbose_name_plural = '系统日志'
        
    uuid = models.CharField(max_length=32, primary_key=True)

    actionOwner = models.CharField(max_length=50, default='')
    actionObject = models.CharField(max_length=50, default='')
    actionType = models.CharField(max_length=50, default='')

    description = models.CharField(max_length=1024, null=True)
    createDate = models.DateTimeField('Create Date', null=True)
       
    def __unicode__(self):
        return "%s-%s" % (self.actionOwner, self.actionType)    
    
    
class Location(models.Model):
    class Meta:
        db_table = 'location'
        verbose_name_plural = '区域管理'
        
    uuid = models.CharField(max_length=32, primary_key=True, verbose_name="地点编码")
    name = models.CharField(max_length=50, default='', verbose_name="地点名称")
    parent = models.ForeignKey('Location', db_column='parent', blank=True, null=True, verbose_name="上级地名")

    def __unicode__(self):
        return "%s<%s>" % (self.name, self.uuid)    
