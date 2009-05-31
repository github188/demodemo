# -*- coding: utf-8 -*-
from django.http import HttpResponse, HttpResponseRedirect
from django.shortcuts import render_to_response

from datetime import datetime
from mygroup.group.models import *

def index(r, ):
    return render_to_response('group_index.html')

def my(r, ):
    u = cur_user(r)
    if u is None: return HttpResponseRedirect('/login')
    
    return HttpResponseRedirect("/my/sent")

def login(request):
    if request.method == 'POST':
        error_msg = ""
        f = request.REQUEST
        name = f['name']
        password = f['password']
        if not name: error_msg = "名字不能为空"
        elif not password: error_msg = "密码不能为空"
        else:
            u = User.objects.filter(name=name)
            if len(u) <= 0:
                error_msg = "用户名不存在."
                u = None
            else:
                u = u[0]
                
            if u and u.password == password:
                u.last_login = datetime.now()
                u.save()
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
    
    
def reg(r):
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
            u.save()
            cur_user(r, u)
            __add_invited_groups(r, u)
            return HttpResponseRedirect('/my')
        else:
            return render_to_response('group_reg.html', {'error_msg':error_msg})
    else:
        return render_to_response('group_reg.html')
    
def __add_invited_groups(r, user):
    for g in Group.objects.all():
        if group.is_invited(g, user):
            group.group_join(r, user, g)

def cur_user(r, user=None, remove=False):
    
    if remove:
        try:
            del r.session['cur_user_id']
        except KeyError:
            pass
    elif user:
        r.session['cur_user_id'] = user.id
        return user
    elif 'cur_user_id' in r.session:
        try:
            return User.objects.get(id=r.session['cur_user_id'])
        except:
            pass
    else:
        print "not fond cur_user_id"
        
    return None

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

        