from django.conf.urls.defaults import *

# Uncomment the next two lines to enable the admin:
from django.contrib import admin
admin.autodiscover()
from ipa4django.views import SimpleViews
from settings import WWW_ROOT

urlpatterns = patterns('',
    (r'^(?P<path>.*\.(?:html|css|js|jpg|gif|png|xls))$', 'django.views.static.serve',
            {'document_root': WWW_ROOT}),
    
    (r'^admin/(.*)', admin.site.root),
    #(r'^report/',      include('deuap.report.urls')),
    #(r'^$',      include('mysite.content.urls')),
    (r'(.*)', SimpleViews("cztv.coreapp.views")),
)