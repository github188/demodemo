from django.conf.urls.defaults import *

# Uncomment the next two lines to enable the admin:
from django.contrib import admin
admin.autodiscover()
from settings import WWW_ROOT

urlpatterns = patterns('',
    (r'^(?P<path>.*\.(?:html|css|js|jpg|gif|png|jar))$', 'django.views.static.serve',
            {'document_root': WWW_ROOT}),                        
    # Example:

    # Uncomment the admin/doc line below and add 'django.contrib.admindocs' 
    # to INSTALLED_APPS to enable admin documentation:
    # (r'^admin/doc/', include('django.contrib.admindocs.urls')),

    # Uncomment the next line to enable the admin:
    #(r'^admin/(.*)', admin.site.root),
    (r'^admin/', include(admin.site.urls)),
    
    (r'^config/load_params', 'Weibo.params.views.load_params'),
    (r'^config/param_edit', 'Weibo.params.views.config'),
    (r'', include('Weibo.coreapp.urls')),
    (r'.*', 'Weibo.coreapp.views.guest_index'),
)
