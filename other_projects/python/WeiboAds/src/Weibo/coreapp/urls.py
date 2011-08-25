from django.conf.urls.defaults import *

urlpatterns = patterns('',  
    
    (r'.*',  'Weibo.coreapp.views.guest_index'),
)