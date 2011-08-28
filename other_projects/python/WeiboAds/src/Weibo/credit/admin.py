# -*- coding: utf-8 -*-
from django.contrib import admin
from Weibo.credit.models import *

class CreditParameterAdmin(admin.ModelAdmin):
    fields = ['name', 'weight', 'default_value', 'level1', 'level2', 'level3', 'level4', 'level5', 'level6', 'level7', 'level8', 'level9',
                    'level10']
    list_display = ('name', 'weight', 'default_value', 'total', 'level1', 'level2', 'level3', 'level4', 'level5', 'level6', 'level7', 'level8', 'level9',
                    'level10')
    search_fields = ('name', )
    order_by = ("-weight")
    
class ProfileCreditAdmin(admin.ModelAdmin):
    fields = ['uuid', 'credit_level', 'status', ]
    list_display = ('uuid', 'credit_level', 'status', 'update_time', 'create_time')
    search_fields = ('uuid', )
        
class CreditQueueAdmin(admin.ModelAdmin):
    fields = ['uuid', 'priorty', ]
    list_display = ('uuid', 'priorty', 'create_time')

class APPAccessKeyAdmin(admin.ModelAdmin):    
    fields = ['app_key', 'app_secret', 'access_key', 'access_secret']
    list_display = ('app_key', 'app_secret', 'access_key', 'access_secret', 'last_use_time', 'update_time', 'create_time')

 

admin.site.register(CreditParameter, CreditParameterAdmin)
admin.site.register(ProfileCredit, ProfileCreditAdmin)
admin.site.register(CreditQueue, CreditQueueAdmin)
admin.site.register(APPAccessKey, APPAccessKeyAdmin)
