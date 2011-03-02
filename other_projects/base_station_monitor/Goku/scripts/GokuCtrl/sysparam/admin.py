
from django.contrib import admin
from models import *

class AlarmDefineAdmin(admin.ModelAdmin):
    fields = ['alarmName', 'alarmLevel', 'alarmCategory', 'reActiveTime']
    list_display = ('alarmCode', 'alarmName', 'alarmLevel', 'alarmCategory', 'reActiveTime')
    list_filter = ['alarmCategory', 'alarmLevel']  
    #list_filter = ['alarmLevel', ]   
    
class BTSCategoryAdmin(admin.ModelAdmin):
    fields = ['uuid', 'name']
    list_display = ('uuid', 'name')       
        
admin.site.register(AlarmDefine, AlarmDefineAdmin)
admin.site.register(BTSCategory, BTSCategoryAdmin)
 