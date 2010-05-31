from django.conf.urls.defaults import *

# Uncomment the next two lines to enable the admin:
from django.contrib import admin
admin.autodiscover()
from ipa4django.views import SimpleViews
from settings import MEDIA_ROOT as WWW_ROOT

urlpatterns = patterns('',
    (r'^static/(.*)', 'django.views.static.serve',
            {'document_root': WWW_ROOT}),            
    
    (r'^admin/(.*)', admin.site.root),
    #(r'^report/',      include('deuap.report.urls')),
    #(r'^$',      include('mysite.content.urls')),
    (r'houseadmin/(.*)', SimpleViews("cztv.houseadmin.views")),
    (r'(.*)', SimpleViews("cztv.coreapp.views")),
)