from django.contrib import admin
from cztv.coreapp.models import *
from django import forms
import re

class FieldTemplateAdmin(admin.ModelAdmin):
    fields = ['category', 'name', 'label', 'field_type', 'values', 'description', 'visiable_order', 'status']
    list_display = ('category', 'name', 'label', 'field_type', 'values', 'visiable_order', 'status')

class FieldCategoryAdmin(admin.ModelAdmin):
    fields = ['name', 'label', 'visiable_order', 'status']
    list_display = ('name', 'label', 'visiable_order', 'status')

class HouseFields(object):
    def __init__(self, ):
        self._fieldsets = None
        self._fields = {}
    
    def bound_field(self, form, name):
        if name in self._fields:
            return forms.forms.BoundField(form, self._fields[name],
                                          name)
        else:
            raise RuntimeError("Not found form field %s" % name)
    
    def declared_fieldsets(self):
        if self._fieldsets is None:
            self._fieldsets = self.__init_dynamical()
        return self._fieldsets
    
    def __init_dynamical(self,):
        #from cztv.coreapp.models import FieldCategory
        category = FieldCategory.objects.filter(status='Y')
        #category
        _fieldsets = [(None, {"fields": ('name',) })]
        for e in category:
            _fieldsets.append((e.label,
                               {
                               "fields": self.__init_category_field(e.name)
                               }))
                              
        return _fieldsets
        
    def __init_category_field(self, cate):
        fields = []
        field_list = FieldTemplate.objects.filter(status='Y', category=cate)
        for e in field_list:
            fields.append(e.name)
            self.__init_form_field(e)
            
        return tuple(fields)
            
    def __init_form_field(self, f):
        field = None
        if f.field_type in ['choice', 'multi_choice']:
            choices = []
            for item in f.values.split(";"):
                if "," not in item:continue
                val, label = item.split(",", 1)
                choices.append((val.strip(), label.strip()))
                
            widget = forms.widgets.CheckboxSelectMultiple
            field = forms.ChoiceField(label=f.label, choices=choices,
                                      widget=widget
                                      )
        else:
            field = forms.CharField(label=f.label)
        
        self._fields[f.name] = field #forms.CharField(label=f.label)
        

HouseMETA = HouseFields()

class HouseAdminForm(forms.ModelForm):
    #product_address5 = forms.CharField()
    #def __getattr__(self, name):
    #    return forms.CharField()
    class Media:
        css = {
            "all": ("house_form.css",)
        }    

    def __getitem__(self, name):
        if name in HouseMETA._fields:
            return HouseMETA.bound_field(self, name)
        else:
            return forms.ModelForm.__getitem__(self, name)
    

class HouseAdmin(admin.ModelAdmin):

    #js = ("my_code.js",)    
    #fields = ['name', 'product_address', 'product_address2', 'product_address3', 'product_address4', 'create_date']
    list_display = ('name', 'product_address', 'product_address2', 'product_address3', 
                    'product_address4', 'product_address5')
    
    form = HouseAdminForm
    #change_form_template = 'admin/myapp/extras/openstreetmap_change_form.html'
    
    def _declared_fieldsets(self):
        return HouseMETA.declared_fieldsets()
        #return None
    declared_fieldsets = property(_declared_fieldsets)    
    
    def product_address5(self, o):
        return o.name

from django.conf import settings
#avoid to validate the fieldsets.
old_debug = settings.DEBUG
settings.DEBUG = False
admin.site.register(FieldTemplate, FieldTemplateAdmin)
admin.site.register(FieldCategory, FieldCategoryAdmin)
admin.site.register(House, HouseAdmin)
settings.DEBUG = old_debug
