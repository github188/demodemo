from django.conf.urls.defaults import *

# Uncomment the next two lines to enable the admin:
# from django.contrib import admin
# admin.autodiscover()

from settings import MEDIA_ROOT as WWW_ROOT

from settings import DIST_ROOT

urlpatterns = patterns('',
    # Example:
    # (r'^pypi/', include('pypi.foo.urls')),

    # Uncomment the admin/doc line below and add 'django.contrib.admindocs' 
    # to INSTALLED_APPS to enable admin documentation:
    # (r'^admin/doc/', include('django.contrib.admindocs.urls')),
    
    (r'PyShop.jnlp$', 'pyshop.coreapp.jnlp.py_manage'),
    (r'online/(?P<name>\w+).jnlp$', 'pyshop.coreapp.jnlp.py_loader'),  
    
    (r'^static/(.*)', 'django.views.static.serve',
            {'document_root': WWW_ROOT}),
    (r'^dist/(.*)', 'django.views.static.serve',
            {'document_root': DIST_ROOT}),    

    (r'(.*)', 'pyshop.coreapp.views.index'),
)
