from django.contrib import admin
from models import *

class BaseStationAdmin(admin.ModelAdmin):
    fields = ['uuid', 'name', 'connectionStatus', 'groupName', 'locationId', 
              'devType', 'btsCategory', 'locationUUID' ]
    list_display = ('uuid', 'name', 'connectionStatus', 'locationUUID', 'routeServer', 'locationId', 
              'alarmStatus', 'devType', 'btsCategory', 'lastActive')
    list_filter = ['devType', 'connectionStatus', 'alarmStatus', ]
    search_fields = ['uuid', 'locationId', ]
    
class AlarmRecordAdmin(admin.ModelAdmin):
    fields = ['base_station', 'channelId', 'alarmCode', 'alarmStatus', 'startTime', 'videoPath']
    list_display = ('base_station', 'channelId', 'alarmCode', 'startTime', 'alarmCategory', 'alarmStatus', 'user', 'videoPath')
    list_filter = ['alarmCode', ]

class UserAdmin(admin.ModelAdmin):
    fields = ['name', 'display', 'password', ]
    list_display = ('name', 'display', 'password', )
    search_fields = ['name', 'display', ]

class UserGroupAdmin(admin.ModelAdmin):
    fields = ['name', 'isAdmin', ]
    list_display = ('name', 'isAdmin', )
    list_filter = ['name', ]              
    search_fields = ['name', ]
    
class SystemLogAdmin(admin.ModelAdmin):
    fields = ['actionType', 'actionOwner', 'actionObject', 'description', 'createDate' ]
    list_display = ('actionType', 'actionOwner', 'actionObject', 'description', 'createDate' )
    list_filter = ['actionType', ]              
    search_fields = ['actionOwner', 'actionObject']

class UserGroupRelationAdmin(admin.ModelAdmin):
    fields = ['user_group', 'user',  ]
    list_display = ('user_group', 'user', )
    list_filter = ['user_group', ]
    #search_fields = ['dev_area', 'team_name', ]    

class StationGroupRelationAdmin(admin.ModelAdmin):
    fields = ['user_group', 'base_station', ]
    list_display = ('user_group', 'base_station', )
    list_filter = ['user_group', ]

class LocationAdmin(admin.ModelAdmin):
    fields = ['uuid', 'name', 'parent']
    list_display = ('uuid', 'name', 'parent')
    
#class AlarmDefineAdmin(admin.ModelAdmin):
#    fields = ['alarmName', 'alarmLevel', 'alarmCategory', 'reActiveTime']
#    list_display = ('alarmCode', 'alarmName', 'alarmLevel', 'alarmCategory', 'reActiveTime')    

admin.site.register(Location, LocationAdmin)
admin.site.register(BaseStation, BaseStationAdmin)
admin.site.register(AlarmRecord, AlarmRecordAdmin)
admin.site.register(User, UserAdmin)
admin.site.register(UserGroup, UserGroupAdmin)
admin.site.register(UserGroupRelation, UserGroupRelationAdmin)
admin.site.register(StationGroupRelation, StationGroupRelationAdmin)
admin.site.register(SystemLog, SystemLogAdmin)
#admin.site.register(AlarmDefine, AlarmDefineAdmin)





