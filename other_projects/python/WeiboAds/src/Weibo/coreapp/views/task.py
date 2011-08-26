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
    
def my_task(r, page=0, limit=50):
    if not is_login(r):return HttpResponseRedirect("%s/index" % APP_ROOT)
    
    list = WeiboTask.objects.filter(user=r.cur_user).order_by()
    
    cur_page = list[page * limit: (page + 1) * limit]
    
    return render_to_response("weibo/weibo_my_task.html", 
                              {'data_list': cur_page},
                              context_instance=template.RequestContext(r)
                              )  
    
        
def task_search(r, tag='', order_by='-update_time', page=0, limit=50):
    if not is_login(r):return HttpResponseRedirect("%s/index" % APP_ROOT)
    
    
    list = WeiboTask.objects.all().order_by(order_by)
    
    cur_page = list[page * limit: (page + 1) * limit]
    #[page * limit: (page + 1) * limit]
    pass
    
    return render_to_response("weibo/weibo_task_search.html", 
                              {'data_list': cur_page},
                              context_instance=template.RequestContext(r)
                              )  
    
def task_detail(r, tid):
    if not is_login(r):return HttpResponseRedirect("%s/index" % APP_ROOT)
        
    task = WeiboTask.objects.get(id=tid)
    
    comment_list = task.taskcomment_set.all().order_by('-create_time')[:50]
        
    return render_to_response("weibo/weibo_task_details.html", 
                              {'task': task,
                               'comments': comment_list,
                               },
                              context_instance=template.RequestContext(r)
                              )

def task_comment(r, ):
    if not is_login(r):return HttpResponseRedirect("%s/index" % APP_ROOT)
    
    if r.method == 'POST':
        task = WeiboTask.objects.get(id=r.POST['tid'])
        comment = TaskComment(task=task, user=r.cur_user)
        comment.desc = r.POST.get('content')
        comment.save()
        return HttpResponseRedirect("/task/%s" % task.id)  
    else:
        return HttpResponseRedirect("/index")
