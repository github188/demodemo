from django.contrib import admin
from angeleyes.eyes.models import *  

class ActivityAdmin(admin.ModelAdmin):
    fields = ['group', 'believer', 'action', 'ipaddress', 'timestamp', 'source', 'category']
    list_display = ('group', 'believer', 'action', 'ipaddress', 'timestamp', 'source', 'category')    

admin.site.register(Activity, ActivityAdmin)
