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

def accept_contract(r, tid):
    
    if not is_login(r):return HttpResponseRedirect("%s/index" % APP_ROOT)
    contract = TaskContract.objects.get(id= (tid or r.REQUEST['tid']))    
    if contract.task.user != r.cur_user: return HttpResponseRedirect("%s/index" % APP_ROOT)
    contract.status = 'S2'
    contract.save()
    
    #/task/4/contracts
    
    return HttpResponseRedirect("%s/task/%s/contracts" % (APP_ROOT,
                                                          contract.task.id))
def submit_contract(r, tid):
    """提交完成后的任务"""
    if not is_login(r):return HttpResponseRedirect("%s/index" % APP_ROOT)
    contract = TaskContract.objects.get(id= (tid or r.REQUEST['tid']))
    if contract.user != r.cur_user: return HttpResponseRedirect("%s/index" % APP_ROOT)
    
    if r.method == 'POST':
        files = []
        for i in range(5):
            key = 'file%s' % i
            if not r.FILES.has_key(key): continue
            file = _upload_file(r, r.FILES[key], r.POST[key + "_desc"], 
                                "contract:%s" % contract.id,
                                "%s_%s" % (contract.id, key),
                                )
            if not file:continue
            files.append(file)
        
        contract.task_deliver = ";".join(["file:%s" % e.id for e in files])
        contract.status = 'S3'
        contract.save()
    
        return HttpResponseRedirect("%s/task/my_do_task" % (APP_ROOT, ))
    else:
        return render_to_response("weibo/custom/submit_done_task.html", 
                                  {'contract': contract,
                                   },
                                  context_instance=template.RequestContext(r)
                                  )

def _upload_file(r, file, desc, ref_id='', name=''):
    import os
    from Weibo.settings import UPLOAD_ROOT 
    #丢弃大文件
    if file.size > 1024 * 1024: return None
    n, _ext = os.path.splitext(file.name)
    if _ext not in ['.jpg', '.jpeg', '.gif', '.png']: return None
    cat, id = ref_id.split(":", 1)
    path = os.path.join(cat, id[-1], id[-2:])
    local_path = os.path.join(UPLOAD_ROOT, path)
    
    if not os.path.isdir(local_path):
        os.makedirs(local_path)
        
    fd = open(os.path.join(local_path, name + _ext), 'wb')
    fd.write(file.read())
    fd.close()
    
    uploaded = UploadFile(user=r.cur_user, ref_id=ref_id)
    uploaded.path = os.path.join(path, name + _ext)
    uploaded.desc = desc
    uploaded.external_link = "/uploaded/%s" % uploaded.path  
    uploaded.save()
    
    return uploaded
