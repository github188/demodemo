
from django.shortcuts import render_to_response
from django.http import HttpResponse, HttpResponseRedirect
import logging
from Weibo.weibopy.auth import OAuthHandler
from Weibo.weibopy.api import API
from Weibo.coreapp.models import WeiboProfile


def guest_index(r):
    r.session['xxx'] = r.GET.get("user",)
    
    logging.info("xxx:%s" % r.session['xxx'])
    return render_to_response("weibo/weibo_base.html", 
                              {'user': r.GET.get("user",)})

def login_index(r):
    #r.session['xxx'] = "xxxxx"
    user = r.session['xxx']
    logging.info("loging:%s" % r.session['xxx'])
    
    return render_to_response("weibo/weibo_base.html", {'user': user})

def callback(r):
    auth = OAuthHandler("2453342288", "2c545e783036afe3ae1cfef1e24ba9fb", )

    verifier = r.GET['oauth_verifier']
    rtoken = r.session['request_token']
    auth.set_request_token(rtoken.key, rtoken.secret)
    access_token = auth.get_access_token(verifier)
    
    api = API(auth)
    user = api.verify_credentials()    
    logging.info("user:%s" % str(user))
    
    WeiboProfile.import_from_sina(user, auth)
    
    return HttpResponseRedirect("/v/my")
    #return render_to_response("weibo/weibo_base.html", {'user': ''})

def auth_sina(r):
    #callback    
    auth = OAuthHandler("2453342288", "2c545e783036afe3ae1cfef1e24ba9fb", "http://127.0.0.1:8000/v/callback")
    auth_url = auth.get_authorization_url()
    r.session['request_token'] = auth.request_token
    logging.info("url:%s" % auth_url)
    logging.info("request_token:%s" % str(auth.request_token))
    
    return HttpResponseRedirect(auth_url)
    