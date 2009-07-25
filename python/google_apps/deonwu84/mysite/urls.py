from django.conf.urls.defaults import *

from utils import SimpleViews, SimpleUrl

urlpatterns = patterns('',

    (r'^foo/(.*)', SimpleViews("mysite.foo.views")),
	(r'^demos/(.*)', SimpleViews("mysite.demos.views")),
    
    (r'^rf_trac/v/(.*)', SimpleViews("mysite.rf_trac.adminViews")),
    (r'^rf_trac/(.*)', SimpleViews("mysite.rf_trac.views")),
)
