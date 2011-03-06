# Create your views here.

from django.http import HttpResponse, HttpResponseRedirect
from django.shortcuts import render_to_response
from django import template

def index(r, *args):
    return HttpResponseRedirect("admin/")

def dev_cfg(r, id=''):
    return render_to_response("dev_ocx.html",
                              context_instance=template.RequestContext(r) 
                              )
