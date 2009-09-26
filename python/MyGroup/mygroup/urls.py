from django.conf.urls.defaults import *

# Uncomment the next two lines to enable the admin:
# from django.contrib import admin
# admin.autodiscover()
#from django.contrib import admin
from mygroup.group.views import group, user
#admin.autodiscover()

urlpatterns = patterns('',
    # Example:
    # (r'^mygroup/', include('mygroup.foo.urls')),

    # Uncomment the admin/doc line below and add 'django.contrib.admindocs' 
    # to INSTALLED_APPS to enable admin documentation:
    # (r'^admin/doc/', include('django.contrib.admindocs.urls')),

    # Uncomment the next line to enable the admin:
    #(r'^(?P<path>.*\.(?:html|css|js|jpg|gif|png))$', 'django.views.static.serve',
    #        {'document_root': WWW_ROOT}),
    #(r'^admin/(.*)', admin.site.root),
    
    (r'^group/(.*)', group.root),
    (r'^my/(.*)', user.root),   
        
    (r'^(?:index|)$', 'mygroup.group.views.index'),
    (r'^my$', 'mygroup.group.views.my'),        
    (r'^reg$', 'mygroup.group.views.reg'), 
    (r'^login$', 'mygroup.group.views.login'),   
    (r'^logout$', 'mygroup.group.views.logout'),
    (r'^upload_show$', 'mygroup.group.views.upload_show'),
    (r'^user_img$', 'mygroup.group.views.user_images'),
    (r'^test$', 'mygroup.group.views.test'),
    
)
