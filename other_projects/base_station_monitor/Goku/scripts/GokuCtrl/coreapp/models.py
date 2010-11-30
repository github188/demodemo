from django.db import models

# Create your models here.
class BaseStation(models.Model):
    class Meta:
        db_table = 'base_station'
            
    uuid = models.CharField(max_length=10, primary_key=True)
    connectionStatus = models.CharField(max_length=10, 
                                        choices=(('connected', "Connected"),
                                                 ('timeout', "TimeOut"),
                                                 ('error', "Login Error"),
                                                 ('new', "New added"),
                                        ),
                                        default='new')
    groupName = models.CharField(max_length=50, default='default')
    routeServer = models.CharField(max_length=50, default='')
    locationId = models.CharField(max_length=50, default='')
    alarmStatus = models.CharField(max_length=50, default='')
    devType = models.IntegerField(default=0,
                                  choices=((1, "Video"),
                                         (2, "Image"),
                                         ),
                                  )
    
    lastActive = models.DateTimeField('last active', null=True)
    lastUpdate = models.DateTimeField('last update', auto_now=True)
    createDate = models.DateTimeField('create date', auto_now_add=True)

    def __unicode__(self):
        return self.uuid        


# Create your models here.
class User(models.Model):
    class Meta:
        db_table = 'user_account'
            
    name = models.CharField(max_length=20, primary_key=True)
    password = models.CharField(max_length=50, 
                                default='new', )
    display = models.CharField(max_length=50, default='default')

    def __unicode__(self):
        return self.name        

# Create your models here.
class UserGroup(models.Model):
    class Meta:
        db_table = 'user_group'
            
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

    user = models.ForeignKey(User)
    user_group = models.ForeignKey(UserGroup)

class StationGroupRelation(models.Model):
    class Meta:
        db_table = 'relation_station_group'
        unique_together = (("base_station", "user_group"), )

    base_station = models.ForeignKey(BaseStation)
    user_group = models.ForeignKey(UserGroup)

class AlarmRecord(models.Model):
    class Meta:
        db_table = 'alarm_record'

    base_station = models.ForeignKey(BaseStation)
    startTime = models.CharField(max_length=50, default='')
    alarmType = models.CharField(max_length=10, default='')
    videoPath = models.CharField(max_length=1024, default='')
    
    def __unicode__(self):
        return "%s-%s" % (self.base_station, self.alarmType)
                
