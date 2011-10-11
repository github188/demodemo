# -*- coding: utf-8 -*-

from django.shortcuts import render_to_response
from django.http import HttpResponse, HttpResponseRedirect
import logging
from Weibo.weibopy.auth import OAuthHandler
from Weibo.weibopy.api import API
from Weibo.params.models import *
from Weibo.settings import APP_ROOT
from django import template
from django.db.models import Q

from django.forms.forms import BoundField

try:
    import json
except:
    import simplejson as json
    
from django.forms import Form

class ParamForm(Form):
    pass
    
def load_params(r, pid=''):
    param = message = None
    
    params = {'status': 'ok', }
    try:
        pid = pid or r.REQUEST.get('pid', 0)
        param = ParameterValue.objects.get(id=pid)
        params['data'] = param.dict_values or {}
    except Exception, e:
        params['msg'] = "load pid=%s, error:%s" % (pid, e)
        params['status'] = 'err'        
    
    return HttpResponse(json.dumps(params))
    
        
def config(r, pid=0):
    
    if r.user.is_anonymous() :return HttpResponseRedirect("%s/admin/" % APP_ROOT)    
    pid = pid or r.REQUEST.get('pid', 0)
    
    param = message = None
    try:
        param = ParameterValue.objects.get(id=pid)
    except Exception, e:
        message = "load pid=%s, error:%s" % (pid, e)        
    
    if param and r.method == 'POST':
        #r.POST.
        param.update_values(dict(r.POST.items()))
        param.save()
        param.clean_site_cache()
        message = '参数保存成功！'
    
    if param:
        param_define = ParameterDefine.load_param_group(param.group)
        param_values = {}
        for e in param_define:
            param_values[e.uuid] = e.default_value
                
        param_values.update(param.dict_values)
        
        param_list = []
        form = ParamForm(param_values)    
        for e in param_define:
            param_list.append(BoundField(form, e.get_field(), e.uuid))
        
    return render_to_response("config/site_parameter_value.html", 
                              locals(),
                              context_instance=template.RequestContext(r)
                              )
    
    
    
    
    
    