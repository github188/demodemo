from django.conf.urls.defaults import *

urlpatterns = patterns('',
    (r'^my',  'Weibo.coreapp.views.login_index'),    
   (r'^index',  'Weibo.coreapp.views.guest_index'),
)