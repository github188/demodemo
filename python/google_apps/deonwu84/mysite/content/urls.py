from django.conf.urls.defaults import *

from utils import SimpleViews, SimpleUrl

urlpatterns = patterns('',
    (r'^(tag)/(?P<tag>.*)', SimpleViews("mysite.content.webViews")),
    (r'^(cate)/(?P<cate>.*)', SimpleViews("mysite.content.webViews")),
)