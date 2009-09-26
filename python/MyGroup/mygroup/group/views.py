# -*- coding: utf-8 -*-
from django.http import HttpResponse, HttpResponseRedirect
from django.shortcuts import render_to_response

from datetime import datetime
from mygroup.group.models import *
import logging
from google.appengine.ext import db

def index(r, ):
    return render_to_response('group_index.html')

def my(r, ):
    u = cur_user(r)
    if u is None: return HttpResponseRedirect('/login')
    
    return HttpResponseRedirect("/my/groups")

def login(request):
    if request.method == 'POST':
        error_msg = ""
        f = request.REQUEST
        name = f['name']
        password = f['password']
        if not name: error_msg = "名字不能为空"
        elif not password: error_msg = "密码不能为空"
        else:
            query = User.all().filter('name =', name)
            if query.count() <= 0:
                error_msg = "用户名不存在."
                u = None
            else:
                u = query.fetch(1)[0]
                
            if u and u.password == password:
                u.last_login2 = u.last_login 
                u.last_login = datetime.now()
                u.put()
                cur_user(request, u)             
                return HttpResponseRedirect('/my')
            elif not error_msg:
                error_msg = "密码错误."
        
        return render_to_response('group_index.html', {'error_msg':error_msg,
                                                       'name':name,
                                                       'password':password,
                                                       })
    else:
        return render_to_response('group_index.html')

def logout(r ):
    cur_user(r, remove=True)
    return HttpResponseRedirect('/index')

def user_images(r ):
    key = r.REQUEST['id']
    user = User.load(key)
    if user.logo is None:
        data = open('header.gif', 'rb').read()
    else:
        data = user.logo
        
    return HttpResponse(data, mimetype='image/png')
    
    
    
def reg(r):
    if r.REQUEST.has_key("gid"): 
        gid = r.REQUEST['gid']
    else:
        gid = ""
    if r.method == 'POST':
        f = r.REQUEST
        name = f['name']
        truename = f['truename']
        password = f['password']
        email = f['email']
        mobile = f['mobile']
        qq = f['qq']
        error_msg = ""
        
        if not name: error_msg = "名字不能为空"
        elif not password: error_msg = "密码不能为空"
        elif not truename: error_msg = "真实姓名不能为空"
        
        if not error_msg:
            u = User(name=name, truename=truename, password=password, 
                     email=email, mobile=mobile, qq=qq)
            u.put()
            cur_user(r, u)
            __add_invited_groups(r, u)
            try:
                if gid.isdigit():
                    g = Group.load(gid)
                    GroupMember(parent=g, member=user).put()
                    group.group_doing(r, u, g, message=u"我来啦。。。")
            except Exception, e:
                logging.error(str(e))
            
            return HttpResponseRedirect('/my')
        else:
            return render_to_response('group_reg.html', {'error_msg':error_msg,
                                                         "gid":gid
                                                        })
    else:
        return render_to_response('group_reg.html', {"gid":gid})
    
def __add_invited_groups(r, user):
    for g in Group.all():
        if group.is_invited(g, user):
            group.group_join(r, user, g)

def cur_user(r, user=None, remove=False):
    from google.appengine.api import memcache
    
    sessionId = r.COOKIES.get('sessionid', None)
    
    if not sessionId: return None
    
    logging.debug("cur user session id:%s" % sessionId)
    if user is not None:
        logging.debug("save current user:%s" % user)
        memcache.add(key=sessionId, value=user, time=3600)
    
    if remove is True:
        memcache.delete(key=sessionId)
        r.COOKIES['sessionid'] = ''
        return None
    
    user = memcache.get(sessionId)
    logging.debug("retrieve current user:%s" % user)
    return user

def upload_show(request, ):
    u = cur_user(request)
    if u is None: return HttpResponseRedirect('/login')
    
    from uploaded import UploadUtil
    if request.method == 'POST':
        filename = request.FILES['file'].name
        UploadUtil.handle_uploaded_file(request.FILES['file'], filename, u)
        return HttpResponseRedirect('/my/setting')
    
    return HttpResponseRedirect('/my/setting')

from group_views import GroupActions
group = GroupActions()
from user_views import UserActions
user = UserActions()

def test(r):
    if not r.GET.has_key('id'):
        return HttpResponse("id:%s" % cur_user(r).id)
    else:
        return HttpResponse("user:%s" % User.load(r.GET['id']))
    #return 
        