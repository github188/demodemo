# -*- coding: utf-8 -*-
from django.contrib import admin
from models import *

from GokuCtrl.sysparam.models import AlarmDefine 

from django import forms
class BaseStationForm(forms.ModelForm):
    class Meta:
        model = BaseStation
    supportAlarms = forms.ModelMultipleChoiceField(label='告警列表', required=False,
                                                  queryset= AlarmDefine.objects.all(), 
                                                  widget=forms.CheckboxSelectMultiple)
    #forms.ModelForm.
    

class BaseStationAdmin(admin.ModelAdmin):
    fields = ['uuid', 'name', 'groupName', 'locationId', 'channels', 
              'devType', 'btsCategory', 'locationUUID', 'supportAlarms' ]
    list_display = ('uuid', 'name', 'connectionStatus', 'locationUUID', 'routeServer', 'locationId', 
              'alarmStatus', 'devType', 'btsCategory', 'lastActive')
    list_filter = ['devType', 'connectionStatus', 'alarmStatus', ]
    search_fields = ['uuid', 'locationId', ]
    form = BaseStationForm
    
    def save_model(self, request, obj, form, change):
        obj.supportAlarm = ",".join(form.data.getlist('supportAlarms'));
        super(BaseStationAdmin, self).save_model(request, obj, form, change)  
        
    
class AlarmRecordAdmin(admin.ModelAdmin):
    fields = ['uuid', 'base_station', 'channelId', 'alarmCode', 'alarmStatus', 'startTime', 'alarmCategory', 'alarmLevel', 'videoPath']
    list_display = ('uuid', 'base_station', 'channelId', 'alarmCode', 'startTime', 'alarmCategory', 'alarmLevel', 'alarmStatus', 'user', 'videoPath')
    list_filter = ['alarmCode', ]

class UserAdmin(admin.ModelAdmin):
    fields = ['name', 'display', 'password', 'status']
    list_display = ('name', 'display', 'status', )
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
    
class VideoTaskAdmin(admin.ModelAdmin):
    fields = ["taskID", "userName", "windowID", "name", "uuid", "channel", 
              "weekDays", "startTime", "endTime",
              "minShowTime", "status", "showOrder", ]
    list_display = ("taskID", "userName", "windowID", "name", "uuid", "channel", 
                    "weekDays", "startTime", "endTime",
                    "minShowTime", "status", "showOrder",)
    order_by = ("userName", "windowID", "showOrder")
    list_filter = ['userName', ]
    
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
admin.site.register(VideoTask, VideoTaskAdmin)
#admin.site.register(AlarmDefine, AlarmDefineAdmin)





