# -*- coding: utf-8 -*-
from django.contrib import admin
from Weibo.coreapp.models import *

class UserAdmin(admin.ModelAdmin):
    fields = ['email', 'screen_name', 'password', 'level', 'balance', ]
    list_display = ('email', 'screen_name', 'password', 'level', 'balance', 'create_time', 'update_time')
    search_fields = ('email', )
    order_by = ("-create_time")
    
class WeiboProfileAdmin(admin.ModelAdmin):
    fields = ['user', 'weibo_id', 'weibo_source', 'is_vip', 'fans_count',]
    list_display = ('user', 'weibo_id', 'weibo_source', 'is_vip', 'fans_count', 'create_time', 'update_time')
    search_fields = ('weibo_id', )
        
class WeiboTaskAdmin(admin.ModelAdmin):
    fields = ['user', 'content', 'image_url', 'desc', 'tags', 'task_status', 'task_type', 'pay_status', 'price_1', 'price_2', 'price_3']
    list_display = ('user', 'tags', 'task_status', 'task_type', 'pay_status', 'price_1', 'price_2', 'price_3', 'create_time', 'update_time')

class TaskCommentAdmin(admin.ModelAdmin):    
    fields = ['user', 'task', 'comment_type', 'desc', ]
    list_display = ('user', 'task', 'comment_type', 'create_time')

class TaskContractAdmin(admin.ModelAdmin):    
    fields = ['user', 'task', 'desc', 'status', 'task_deliver',]
    list_display = ('user', 'task', 'status', 'task_deliver', 'create_time', 'update_time')    

class UploadFileAdmin(admin.ModelAdmin):    
    fields = ['user', 'path', 'ref_id']
    list_display = ('user', 'path', 'external_link', 'create_time', 'update_time')    
    
class UserTransactionAdmin(admin.ModelAdmin):    
    fields = ['user', 'action', 'count', 'balance', 'transaction_id', ]
    list_display = ('user', 'action', 'count', 'balance', 'transaction_id', 'create_time')        

admin.site.register(User, UserAdmin)
admin.site.register(WeiboProfile, WeiboProfileAdmin)
admin.site.register(WeiboTask, WeiboTaskAdmin)
admin.site.register(TaskComment, TaskCommentAdmin)

admin.site.register(TaskContract, TaskContractAdmin)
admin.site.register(UploadFile, UploadFileAdmin)
admin.site.register(UserTransaction, UserTransactionAdmin)

