from django.conf.urls.defaults import *

urlpatterns = patterns('',
    (r'^my',  'Weibo.coreapp.views.login_index'),   
   (r'^index',  'Weibo.coreapp.views.guest_index'),
   (r'^auth_sina',  'Weibo.coreapp.views.auth_sina'),
   (r'^callback',  'Weibo.coreapp.views.callback'),   
   (r'^logout',  'Weibo.coreapp.views.logout'),   
   
   (r'^task/post',  'Weibo.coreapp.views.post_new_task'), 
   (r'^task/list',  'Weibo.coreapp.views.task_search'),
   (r'^task/my_task',  'Weibo.coreapp.views.my_task'),
   
   (r'^task/(\d+)',  'Weibo.coreapp.views.task_detail'),
   (r'^do_task/(\d+)',  'Weibo.coreapp.views.do_task'),   
   (r'^task/comment',  'Weibo.coreapp.views.task_comment'),
      
)