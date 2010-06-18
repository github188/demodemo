from django.conf.urls.defaults import *

from utils import SimpleViews, SimpleUrl
from rfdoc.coreapp import views

urlpatterns = patterns('',
    #(r'^/dyd/(.*)', SimpleViews("mysite.deonwu.views")),
    
    (r'^upload/?$', views.upload),
    (r'^search/?$', views.search),
    (r'^lib/(.*)', views.library),
    (r'^help', views.help),
    (r'^$', views.index),    
    #(r'(.*)', SimpleViews("rfdoc.coreapp.views"))
    #(r'(.*)', SimpleViews("mysite.deonwu.views")),
)
