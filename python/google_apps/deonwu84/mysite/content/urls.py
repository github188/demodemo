from django.conf.urls.defaults import *

from utils import SimpleViews, SimpleUrl

urlpatterns = patterns('',
    (r'^tag/(?P<cate>\w+?)/(?P<tag>.*)', SimpleUrl("mysite.content.webViews.list_message")),
    (r'^cate/(?P<cate>.*)', SimpleUrl("mysite.content.webViews.list_message")),
    (r'^vote/(?P<cate>.*)/(?P<msg_id>\d+)', SimpleUrl("mysite.content.webViews.vote")),
    (r'(.*)', SimpleViews("mysite.content.webViews")),
)