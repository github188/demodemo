# -*- coding: utf-8 -*-
from django.contrib import admin
from Weibo.params.models import *

    
class ParameterDefineAdmin(admin.ModelAdmin):
        
    fields = ['group', 'uuid', 'input_type', 'default_val', 'details', ]
    list_display = ('group', 'uuid', 'input_type', 'default_val', 'details',  'create_time', 'update_time')
    search_fields = ('uuid', )
        
class ParameterValueAdmin(admin.ModelAdmin):
    
    fields = ['group', 'site_url', 'values', ]
    list_display = ('id', 'group', 'site_url',  'create_time', 'update_time')
    search_fields = ('site_url', )

admin.site.register(ParameterDefine, ParameterDefineAdmin)
admin.site.register(ParameterValue, ParameterValueAdmin)

