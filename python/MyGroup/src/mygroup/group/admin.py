from django.contrib import admin
from mygroup.group.models import *

class UserAdmin(admin.ModelAdmin):
    fields = ['name', 'truename', 'password', 'email', 'mobile']
    list_display = ('name', 'truename', 'password', 'email', 'mobile', 'create_date')

class GroupAdmin(admin.ModelAdmin):
    fields = ['name', 'creator', 'description', ]
    list_display = ('name', 'creator', 'description',  'create_date')

class MessageAdmin(admin.ModelAdmin):
    fields = ['author', 'group', 'category', 'text',  ]
    list_display = ('author', 'group', 'category','text','create_date', )

class GroupMemberAdmin(admin.ModelAdmin):
    fields = ['group', 'member', 'truename', 'email', 'mobile',  ]
    list_display = ('group', 'member', 'truename', 'email', 'mobile', )
    search_fields = ['build_name', 'category', 'sw_version']
    
admin.site.register(GroupMessage, MessageAdmin)
admin.site.register(Group, GroupAdmin)
admin.site.register(User, UserAdmin)
admin.site.register(GroupMember, GroupMemberAdmin)

admin.site.register(Greeting)
