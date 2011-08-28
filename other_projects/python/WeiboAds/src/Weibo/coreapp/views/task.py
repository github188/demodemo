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
        task = WeiboTask()
        task.user = r.cur_user
        for e in ['task_type', 'content', 'image_url', 'tags', 'price_1', 'price_2', 'price_3',
                  'desc']:
            value = r.POST.get(e)
            if value: setattr(task, e, value)
        if (task.task_type == 'post' and task.content) or \
           (task.task_type == 'reply' and task.image_url) :
            
            #更新系统标签列表。
            TagContent.update_tag_info(task.tags)
            task.save()
        return HttpResponseRedirect("%s/my" % APP_ROOT) 
    else:
        form = TaskForm(instance=WeiboTask(), )
        form.init_bounds()
        form.update_attr()
    
    return render_to_response("weibo/seller/weibo_task_form.html", 
                              {'form': form},
                              context_instance=template.RequestContext(r)
                              )
    
class TaskForm(ModelForm):
    class Meta:
        model = WeiboTask
    
    def update_attr(self):
        self.form_fields['content'].field.widget.attrs.update({'cols': '60', 'rows': '3'})
        self.form_fields['desc'].field.widget.attrs.update({'cols': '60', 'rows': '5'})
        
        self.form_fields['price_1'].field.widget.input_type = 'number'
        self.form_fields['price_1'].field.widget.attrs.update({'min': '0', 'max': '100000'})
        self.form_fields['price_2'].field.widget.input_type = 'number'
        self.form_fields['price_2'].field.widget.attrs.update({'min': '0', 'max': '100000'})
        self.form_fields['price_3'].field.widget.input_type = 'number'
        self.form_fields['price_3'].field.widget.attrs.update({'min': '0', 'max': '100000'})
        
        self.form_fields['image_url'].field.widget.input_type = 'url'

    
    def init_bounds(self):
        """ 把值和字段绑定，可以在界面直接显示 """
        self.form_fields = dict(((e.name, e) for e in iter(self)))
        
    #self._bound_fields = dict(((e.name, e) for e in iter(self.form)))   
    
def my_task(r, page=0, limit=50):
    if not is_login(r):return HttpResponseRedirect("%s/index" % APP_ROOT)
    
    list = WeiboTask.objects.filter(user=r.cur_user).order_by("-create_time")
    
    cur_page = list[page * limit: (page + 1) * limit]
    
    return render_to_response("weibo/weibo_my_task.html", 
                              {'data_list': cur_page},
                              context_instance=template.RequestContext(r)
                              ) 

def my_do_task(r, page=0, limit=50):
    if not is_login(r):return HttpResponseRedirect("%s/index" % APP_ROOT)
    
    list = TaskContract.objects.filter(user=r.cur_user).order_by("-create_time")
    
    cur_page = list[page * limit: (page + 1) * limit]
    
    return render_to_response("weibo/custom/my_do_task.html", 
                              {'data_list': cur_page},
                              context_instance=template.RequestContext(r)
                              ) 

def task_custom_list(r, tid):    
    if not is_login(r):return HttpResponseRedirect("%s/index" % APP_ROOT)
    task = WeiboTask.objects.get(id= (tid or r.REQUEST['tid']))
    if task.user != r.cur_user: return HttpResponseRedirect("%s/index" % APP_ROOT)
    contract_list = task.active_contract()
    
    return render_to_response("weibo/seller/task_contract_list.html", 
                              {'task': task,
                               'contract_list': contract_list,
                               },
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
        return HttpResponseRedirect("%s/task/%s" % (APP_ROOT, task.id))
    else:
        return HttpResponseRedirect("%s/index" % (APP_ROOT, ))
    
def do_task(r, tid):
    """参加竞标任务"""
    if not is_login(r): return HttpResponseRedirect("%s/index" % APP_ROOT)
    
    task = WeiboTask.objects.get(id= (tid or r.REQUEST['tid']))
    if r.method == 'POST':
        price = r.POST['price']
        desc = r.POST['desc']
        weibo = r.POST['weibo']
        contract = TaskContract(user=r.cur_user, task=task)
        contract.price = int(price)
        contract.desc = desc
        contract.weibo = weibo
        contract.save()
        
        comment = TaskComment(task=task, user=r.cur_user)
        comment.contract = contract
        comment.comment_type = 'S2'
        comment.desc = desc
        comment.save()
        
        return HttpResponseRedirect("%s/task/%s" % (APP_ROOT, task.id))
    else:
        weibo_list = r.cur_user.weiboprofile_set.all()
    
    return render_to_response("weibo/weibo_task_contract_form.html", 
                              {'task': task,
                               'weibo_list': weibo_list,
                               },
                              context_instance=template.RequestContext(r)
                              )
    
