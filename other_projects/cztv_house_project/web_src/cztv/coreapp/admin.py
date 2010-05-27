from django.contrib import admin
from cztv.coreapp.models import *

class FieldTemplateAdmin(admin.ModelAdmin):
    fields = ['category', 'name', 'label', 'field_type', 'values', 'description', 'visiable_order']
    list_display = ('category', 'name', 'label', 'field_type', 'values', 'visiable_order')

admin.site.register(FieldTemplate, FieldTemplateAdmin)
