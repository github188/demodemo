from django.contrib import admin
from models import *

class BaseStationAdmin(admin.ModelAdmin):
    fields = ['uuid', 'connectionStatus', 'groupName', 'routeServer', 'locationId', 
              'alarmStatus', 'devType' ]
    list_display = ('uuid', 'connectionStatus', 'groupName', 'routeServer', 'locationId', 
              'alarmStatus', 'devType', 'lastActive')
    list_filter = ['devType', 'connectionStatus', 'alarmStatus', ]
    search_fields = ['uuid', 'locationId', ]
    
class AlarmRecordAdmin(admin.ModelAdmin):
    fields = ['base_station', 'alarmType', 'startTime', 'videoPath']
    list_display = ('base_station', 'alarmType', 'startTime', 'videoPath')
    list_filter = ['alarmType', ]

class UserAdmin(admin.ModelAdmin):
    fields = ['name', 'display', 'password', ]
    list_display = ('name', 'display', 'password', )
    search_fields = ['name', 'display', ]

class UserGroupAdmin(admin.ModelAdmin):
    fields = ['name', 'isAdmin', ]
    list_display = ('name', 'isAdmin', )
    list_filter = ['name', ]              
    search_fields = ['name', ]


class UserGroupRelationAdmin(admin.ModelAdmin):
    fields = ['user_group', 'user',  ]
    list_display = ('user_group', 'user', )
    list_filter = ['user_group', ]
    #search_fields = ['dev_area', 'team_name', ]    

class StationGroupRelationAdmin(admin.ModelAdmin):
    fields = ['user_group', 'base_station', ]
    list_display = ('user_group', 'base_station', )
    list_filter = ['user_group', ]


admin.site.register(BaseStation, BaseStationAdmin)
admin.site.register(AlarmRecord, AlarmRecordAdmin)
admin.site.register(User, UserAdmin)
admin.site.register(UserGroup, UserGroupAdmin)
admin.site.register(UserGroupRelation, UserGroupRelationAdmin)
admin.site.register(StationGroupRelation, StationGroupRelationAdmin)





