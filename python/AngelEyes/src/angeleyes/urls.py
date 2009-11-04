from django.conf.urls.defaults import *
from utils import SimpleViews, SimpleUrl

# Uncomment the next two lines to enable the admin:
from django.contrib import admin
#from django.contrib import databrowse
from settings import WWW_ROOT

admin.autodiscover()

urlpatterns = patterns('',
                       
    (r'^admin/(.*)', admin.site.root),
    #(r'^upload_form/$', 'ipata.analyser.views.upload'),
    #(r'^upload_ok/$', 'ipata.analyser.views.upload_ok'),
    (r'^(?P<path>.*\.(?:html|css|js|jpg|gif|png))$', 'django.views.static.serve',
            {'document_root': WWW_ROOT}),
            
    (r'^e/(?P<group>\w+)/(?P<url>\w+)', SimpleViews("angeleyes.eyes.views")),              
)
