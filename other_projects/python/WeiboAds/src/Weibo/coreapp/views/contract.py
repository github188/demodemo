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
    
