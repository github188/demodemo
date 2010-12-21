# -*- coding: utf-8 -*-
from django.db import models

# Create your models here.
class BaseStation(models.Model):
    class Meta:
        db_table = 'base_station'
        #verbose_name = '基站列表'
        verbose_name_plural = '基站列表'
            
    uuid = models.CharField(max_length=10, primary_key=True, verbose_name="基站编号")
    connectionStatus = models.CharField(max_length=10, 
                                        choices=(('connected', "Connected"),
                                                 ('timeout', "TimeOut"),
                                                 ('error', "Login Error"),
                                                 ('new', "New added"),
                                        ),
                                        default='new',
                                        verbose_name="连接状态", 
                                        )
    groupName = models.CharField(max_length=50, default='default', verbose_name="监控分组")
    routeServer = models.CharField(max_length=50, null=True, verbose_name="转发服务器", editable=False)
    locationId = models.CharField(max_length=50, default='', verbose_name="基站地址")
    alarmStatus = models.CharField(max_length=50, null=True, verbose_name="告警状态")
    devType = models.IntegerField(default=1,
                                  choices=((1, "视频"),
                                         (2, "图片"),
                                         ),
                                   verbose_name="设备类型"
                                  )
    
    lastActive = models.DateTimeField(null=True, verbose_name="最后活动时间", editable=False)
    lastUpdate = models.DateTimeField( auto_now=True, editable=False, verbose_name="最后更新时间")
    createDate = models.DateTimeField(auto_now_add=True, editable=False, verbose_name="创建时间")

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
                              choices=(('ok', "Ok"),
                                       ('removed', "Removed"),
                                       ('locked', "locked"),
                                       ),
                             default='ok')    

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
        verbose_name_plural = '告警管理'
        
    uuid = models.CharField(max_length=32, primary_key=True)

    base_station = models.ForeignKey(BaseStation, db_column='base_station')
    startTime = models.CharField(max_length=50, default='')
    alarmType = models.CharField(max_length=10, default='')
    alarmStatus = models.CharField(max_length=10, default='')    
    user = models.CharField(max_length=20, default='')
    videoPath = models.CharField(max_length=1024, default='')
    startTime = models.DateTimeField('Start time', null=True)
    endTime = models.DateTimeField('end time', null=True)
       
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
                
