
from django.shortcuts import render_to_response
from django.http import HttpResponse, HttpResponseRedirect
import logging
from Weibo.weibopy.auth import OAuthHandler
from Weibo.weibopy.api import API
from Weibo.coreapp.models import WeiboProfile
from Weibo.settings import APP_ROOT
from django import template

def is_login(r): return  r.session.get('login_user',)

def guest_index(r):
    #r.session['xxx'] = r.GET.get("user",)
    
    #logging.info("xxx:%s" % r.session['xxx'])
    if is_login(r):return HttpResponseRedirect("%s/my" % APP_ROOT)
    
    return render_to_response("weibo/weibo_base.html", 
                              {'user': r.GET.get("user",)})

def login_index(r):
    if not is_login(r):return HttpResponseRedirect("%s/index" % APP_ROOT)
    #r.session['xxx'] = "xxxxx"
    user = r.session.get('login_user',)
    logging.info(u"loging:%s" % user)
    
    return render_to_response("weibo/login_index.html", {},
                              context_instance=template.RequestContext(r)
                              )

def callback(r):
    auth = OAuthHandler("2453342288", "2c545e783036afe3ae1cfef1e24ba9fb", )

    verifier = r.GET['oauth_verifier']
    rtoken = r.session['request_token']
    auth.set_request_token(rtoken.key, rtoken.secret)
    access_token = auth.get_access_token(verifier)
    
    api = API(auth)
    user = api.verify_credentials()    
    logging.info("user:%s" % str(user))
    
    p = WeiboProfile.import_from_sina(user, auth)
    r.session['login_user'] = p.user
    r.session['login_weibo'] = p
    
    return HttpResponseRedirect("%s/my" % APP_ROOT)
    #return render_to_response("weibo/weibo_base.html", {'user': ''})

def auth_sina(r):
    #callback    
    auth = OAuthHandler("2453342288", "2c545e783036afe3ae1cfef1e24ba9fb", "%s/callback" % APP_ROOT)
    auth_url = auth.get_authorization_url()
    r.session['request_token'] = auth.request_token
    logging.info("url:%s" % auth_url)
    logging.info("request_token:%s" % str(auth.request_token))
    
    return HttpResponseRedirect(auth_url)

def logout(r):
    r.session.flush()
    return HttpResponseRedirect("%s/index" % APP_ROOT)
    