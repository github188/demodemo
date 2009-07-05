from django.conf.urls.defaults import *

from sugar_py.utils import SimpleViews, SimpleUrl

# Uncomment the next two lines to enable the admin:
from django.contrib import admin
admin.autodiscover()
#from django.contrib import admin
#from mygroup.group.views import group, user
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
    (r'^admin/(.*)', admin.site.root),
	
    (r'^canvas$', SimpleUrl("sugar_py.sugar.views.home")),
	(r'^canvas/(.*)', SimpleViews("sugar_py.sugar.views")),
    (r'^canvas(.*)', SimpleViews("sugar_py.sugar.views")),
)
