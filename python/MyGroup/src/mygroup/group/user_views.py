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
        #'list': self.group_last_active, #最近消息
        'sent': self.my_sent,       #我发送的
        'followed': self.my_followed, #我关注的
        'follower': self.my_follower, #关注我的
        'reply': self.my_reply,       #回复我的
        
        'follow': self.do_follow, #关注 
        'message': self.send_message, #发送消息 
        
        'setting': self.my_setting, #发送消息 
        }
    
    def root(self, r, url):
        user = cur_user(r)
        if user is None: return HttpResponseRedirect('/login')
        args = url.split("/")
        action, args = args[0], args[1:]
        
        try:
            action_args = [r, user, ]
            action = self.actions.get(action, self.my_sent)
            return action(*action_args)
        
        except Exception, e:
           print "%s\n%s" %(e, traceback.format_exc())
           raise
           #return HttpResponse("Error")
    
    def my_sent(self, request, user, ajax_html=False):
        
        views = UserViews(user)
        views.action = 'sent'
        views.cur_actives = views.my_sent()
        
        template = ajax_html and 'my_ajax_actives.html' or 'my_index.html'
            
        return render_to_response(template, 
                                  {'my': views, 
                                   'v':views, })

    def my_followed(self, request, user, ajax_html=False):
        views = UserViews(user)
        views.action = 'followed'
        views.cur_actives = views.my_followed()
        
        template = ajax_html and 'my_ajax_actives.html' or 'my_index.html'
            
        return render_to_response(template, 
                                  {'my': views, 
                                   'v':views, })  
        
    def my_follower(self, request, user, ajax_html=False):
        views = UserViews(user)
        views.action = 'follower'
        views.cur_actives = views.my_follower()
        
        template = ajax_html and 'my_ajax_actives.html' or 'my_index.html'
            
        return render_to_response(template, 
                                  {'my': views, 
                                   'v':views, })
        
    def my_reply(self, request, user, ajax_html=False):
        views = UserViews(user)
        views.action = 'reply'
        views.cur_actives = views.my_reply()
        
        template = ajax_html and 'my_ajax_actives.html' or 'my_index.html'
            
        return render_to_response(template, 
                                  {'my': views, 
                                   'v':views, })
        
    def do_follow(self):
        pass    
    
    def send_message(self, request, user, message='', ajax_html=False):
        text = message or request.REQUEST['text'].strip()
        
        b_index = text.count("]") and text.index("]") or 0
        if text.startswith("[") and  b_index < 10:
            category = text[1: b_index]
            text = text[b_index + 1:].strip()
        else:
            category = "状态"
        
        m = MyMessage(author=user, category=category, text=text)
        m.save()        
        return self.my_sent(request, user, ajax_html=True)

    def my_setting(self, request, user, ajax_html=False):
        
        setting = user
        if request.method == 'POST':
            f = request.REQUEST
            if f['email']: setting.email = f['email'] 
            if f['mobile']: setting.mobile = f['mobile'] 
            if f['qq']: setting.qq = f['qq']
            if f['password']: setting.password = f['password']
            setting.save()
            
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
        return GroupMember.objects.filter(member=self.user, status='0')

    def have_new_groups(self):
        from datetime import datetime, timedelta
        escape = timedelta(30)
        old_time = datetime.now() - escape

        return GroupMember.objects.filter(member=self.user, 
                                          status='0', 
                                          create_date__gt=old_time).count() > 0
        
    def new_groups(self):
        from datetime import datetime, timedelta
        escape = timedelta(30)
        old_time = datetime.now() - escape

        return GroupMember.objects.filter(member=self.user, 
                                          status='0', 
                                          create_date__gt=old_time)
    
    def my_sent(self, offset=0, limit=100,):
        
        offset, limit = int(offset), int(limit)
        
        return MyMessage.objects.filter(author=self.user, 
                                      reply_id=0, 
                                      followed_id=0,
                                      status='0').order_by('-create_date')\
                                      [offset: offset+limit]
        
    def my_followed(self, offset=0, limit=100,):
        
        offset, limit = int(offset), int(limit)
        
        return MyMessage.objects.filter(author=self.user, 
                                      reply_id=0, 
                                      followed_id=0,
                                      status='0').order_by('-create_date')\
                                      [offset: offset+limit]
    
    def my_follower(self, offset=0, limit=100,):
        
        offset, limit = int(offset), int(limit)
        
        return MyMessage.objects.filter(author=self.user, 
                                      reply_id=0, 
                                      followed_id=0,
                                      status='0').order_by('-create_date')\
                                      [offset: offset+limit]    
                                      
    def my_reply(self, offset=0, limit=100,):
        
        offset, limit = int(offset), int(limit)
        
        return MyMessage.objects.filter(author=self.user, 
                                      reply_id=0, 
                                      followed_id=0,
                                      status='0').order_by('-create_date')\
                                      [offset: offset+limit]
                                                                                
    def empty(self):
        return not self.cur_actives or len(self.cur_actives) == 0
    
    def monthly_ask_count(self,):
        from datetime import datetime
        now = datetime.now()
        today = datetime(now.year, now.month, 1)
        return Greeting.objects.filter(receiver_id=self.user.user_id,
                                       create_date__gt=today).count()

    def all_ask_count(self,):
        return Greeting.objects.filter(receiver_id=self.user.user_id,).count()
    

    def recent_asked_user(self,):
        grettings = Greeting.objects.filter(receiver_id=self.user.user_id).\
                        order_by("-create_date")[:8]
        return (e.sender for e in grettings)
    
