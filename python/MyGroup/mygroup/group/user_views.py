# -*- coding: utf-8 -*-
from django.http import HttpResponse, HttpResponseRedirect
from django.shortcuts import render_to_response

from datetime import datetime
from mygroup.group.views import cur_user
from mygroup.group.models import *
import traceback

class UserActions(object):
    
    def __init__(self, ):
        self.actions = {
        'groups': self.my_groups,
        
        'setting': self.my_setting, #发送消息 
        }
    
    def root(self, r, url):
        user = cur_user(r)
        if user is None: return HttpResponseRedirect('/login')
        args = url.split("/")
        action, args = args[0], args[1:]
        
        try:
            action_args = [r, user, ]
            action = self.actions.get(action, self.my_groups)
            return action(*action_args)
        
        except Exception, e:
           print "%s\n%s" %(e, traceback.format_exc())
           raise
           #return HttpResponse("Error")
    
    def my_groups(self, request, user, ajax_html=False):
        views = UserViews(user)
        views.action = 'sent'
        #views.cur_actives = views.my_sent()
        
        template = ajax_html and 'my_ajax_actives.html' or 'my_index.html'
            
        return render_to_response(template, 
                                  {'my': views, 
                                   'v':views, })

    def my_setting(self, request, user, ajax_html=False):
        
        setting = db.get(user.key())
        if request.method == 'POST':
            f = request.REQUEST
            if f['email']: setting.email = f['email'] 
            if f['mobile']: setting.mobile = f['mobile'] 
            if f['qq']: setting.qq = f['qq']
            if f['password']: setting.password = f['password']
            setting.put()
            
            return HttpResponseRedirect('/my')
        
        return render_to_response('my_setting.html', 
                                  {'title': '修改个人设置',
                                   'v':setting,
                                   'my': UserViews(user), })               

class UserViews(object):
    
    def __init__(self, user):
        self.user = user
        self.cur_actives = []
    
    def is_newcomer(self):
        pass
    
    def my_groups(self):
        return GroupMember.all().filter('member =', self.user).\
                                filter('status = ', '0').fetch(100)
        
    def new_groups(self):
        return self._new_group_query().fetch(100)
                                                                                
    def empty(self):
        return not self.cur_actives or len(self.cur_actives) == 0
    
    def monthly_ask_count(self,):
        from datetime import datetime
        now = datetime.now()
        today = datetime(now.year, now.month, 1)
        return Greeting.all().filter('receiver =', self.user).\
                              filter('create_date >=', today).count()

    def all_ask_count(self,):
        return Greeting.all().filter('receiver =', self.user).count()
    

    def recent_asked_user(self,):
        grettings = Greeting.all().filter('receiver =', self.user).\
                        order("-create_date").fetch(8)
                        
        return (e.sender for e in grettings)
    
