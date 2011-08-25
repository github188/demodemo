
from django.shortcuts import render_to_response
import logging

def guest_index(r):
    r.session['xxx'] = r.GET.get("user",)
    
    logging.info("xxx:%s" % r.session['xxx'])
    return render_to_response("weibo/weibo_base.html", {'user': r.GET.get("user",)})

def login_index(r):
    #r.session['xxx'] = "xxxxx"
    user = r.session['xxx']
    logging.info("loging:%s" % r.session['xxx'])
    
    return render_to_response("weibo/weibo_base.html", {'user': user})