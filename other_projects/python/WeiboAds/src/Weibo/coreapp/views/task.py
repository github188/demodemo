# -*- coding: utf-8 -*-

from django.shortcuts import render_to_response
from django.http import HttpResponse, HttpResponseRedirect
import logging
from Weibo.weibopy.auth import OAuthHandler
from Weibo.weibopy.api import API
from Weibo.coreapp.models import *
from Weibo.settings import APP_ROOT
from django import template
from django.forms import ModelForm

def is_login(r):
    r.cur_user = r.session.get('login_user',)
    return  r.session.get('login_user',)

def post_new_task(r):
    if not is_login(r):return HttpResponseRedirect("%s/index" % APP_ROOT)
    
    if r.method == 'POST':
        tasks = WeiboTask()
        tasks.user = r.cur_user
        for e in ['task_type', 'content', 'image_url', 'tags', 'price_1', 'price_2', 'price_3',
                  'desc']:
            value = r.POST.get(e)
            setattr(tasks, e, value)
        tasks.save()
        return HttpResponseRedirect("%s/my" % APP_ROOT)     
    else:
        form = TaskForm(instance=WeiboTask(), )
        form.init_bounds()
        form.update_attr()
    
    return render_to_response("weibo/weibo_task_form.html", 
                              {'form': form},
                              context_instance=template.RequestContext(r)
                              )
    
class TaskForm(ModelForm):
    class Meta:
        model = WeiboTask
    
    def update_attr(self):
        self.form_fields['content'].field.widget.attrs.update({'cols': '60', 'rows': '3'})
        self.form_fields['desc'].field.widget.attrs.update({'cols': '60', 'rows': '5'})
    
    def init_bounds(self):
        """ 把值和字段绑定，可以在界面直接显示 """
        self.form_fields = dict(((e.name, e) for e in iter(self)))
        
    #self._bound_fields = dict(((e.name, e) for e in iter(self.form)))        