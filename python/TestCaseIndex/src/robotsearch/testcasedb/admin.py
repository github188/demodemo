from django.contrib import admin
from robotsearch.testcasedb.models import *  

class IndexFileAdmin(admin.ModelAdmin):
    fields = ['uuid', 'path', 'status', 'svn_version', 'svn_author',  ]
    list_display = ('uuid', 'path', 'status', 'svn_version', 'svn_author', )    

class RobotIndexItemAdmin(admin.ModelAdmin):
    fields = ['uuid', 'name', 'item_type', 'status', 'tags', ]
    list_display = ('uuid', 'name', 'file_path', 'item_type', 'status',  )    

admin.site.register(IndexFile, IndexFileAdmin)
admin.site.register(RobotIndexItem, RobotIndexItemAdmin)
