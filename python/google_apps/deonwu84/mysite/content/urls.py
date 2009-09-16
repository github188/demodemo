from django.conf.urls.defaults import *

from utils import SimpleViews, SimpleUrl

urlpatterns = patterns('',
    (r'^tag/(?P<tag>.*)', SimpleUrl("mysite.content.webViews.list_tag_message")),
    (r'^cate/(?P<cate>.*)', SimpleUrl("mysite.content.webViews.list_category_message")),
    (r'(.*)', SimpleViews("mysite.content.webViews")),
)