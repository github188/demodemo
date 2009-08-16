from django.conf.urls.defaults import *

from utils import SimpleViews, SimpleUrl

urlpatterns = patterns('',

    (r'^foo/(.*)', SimpleViews("mysite.foo.views")),
	(r'^demos/(.*)', SimpleViews("mysite.demos.views")),
    
    (r'^wiki/(.*)', SimpleViews("mysite.wiki.views")),
    
    (r'^rf_trac/r/(.*)', SimpleViews("mysite.rf_trac.reportViews")),
    (r'^rf_trac/v/(.*)', SimpleViews("mysite.rf_trac.adminViews")),
    (r'^rf_trac/(.*)', SimpleViews("mysite.rf_trac.views")),
    
    (r'^fb_book/(.*)', SimpleViews("mysite.fb_book.views")),
)
