from django.conf.urls.defaults import *

from utils import SimpleViews, SimpleUrl

urlpatterns = patterns('',

    (r'^foo/(.*)', SimpleViews("mysite.foo.views")),
)
