from django.conf.urls.defaults import *

urlpatterns = patterns('',
    (r'^my',  'Weibo.coreapp.views.login_index'),   
   (r'^index',  'Weibo.coreapp.views.guest_index'),
   (r'^auth_sina',  'Weibo.coreapp.views.auth_sina'),
   (r'^callback',  'Weibo.coreapp.views.callback'),   
   (r'^logout',  'Weibo.coreapp.views.logout'),   
   
   (r'^task/post',  'Weibo.coreapp.views.post_new_task'),   
)