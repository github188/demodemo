# -*- coding: utf-8 -*-
from django.http import HttpResponse, HttpResponseRedirect
from django.shortcuts import render_to_response

from datetime import datetime
from mygroup.group.views import cur_user
from mygroup.group.user_views import UserViews
from mygroup.group.models import *
import traceback
from google.appengine.ext import db

from baseaction import BaseAction
import logging

class GroupActions(BaseAction):
    
    def __init__(self, ):
        self.actions = {
        'actives': self.group_last_active,
        'members': self.group_member_active,
        'news': self.group_important_news,
        'history': self.group_memory,
        'create': self.group_create,
        'edit': self.group_edit,
        'join': self.group_join,
        'invite': self.group_invite_memebers, 
        'doing': self.group_doing,
        'ask': self.group_ask,
        'vote': self.group_vote,        
        
        'guest': self.group_guest, #测试接口.
        'setting': self.group_setting,
        'member': self.group_member_info,  
        }
    
    def root(self, r, url):
        user = cur_user(r)
        if user is None: return HttpResponseRedirect('/login')
        args = url.split("/")
        action, args = args[0], args[1:]
        
        try:
            action_args = [r, user, ]
            if len(args) > 0:
                action_args.append(Group.load(args[0]))
                args.remove(args[0])
                if not self.is_member(user, action_args[-1]) and 'join' != action:
                    return self.group_guest(*action_args)
            action_args.extend(args)
            action = self.actions.get(action, self.group_unkown)
            return action(*action_args)
        
        except Exception, e:
            logging.error(str(e))
            raise
            #return "ERROR"
           #return HttpResponse("Error")
    
    def handler_mapping(r, url):
        handler = self.url_mapping.get(url, self.no_handler)
        
        return handler
    
    def url_parameter(self, url):
        return ()
    
    def group_unkown(self, *p, **kw):
        return HttpResponseRedirect("/my")

    def group_guest(self, request, user, group):
        views = GroupViews(group)
        return render_to_response("group_guest.html", 
                                  {'v':views,
                                   'my': UserViews(user), })
        
    def group_ask(self, request, user, group, user_id):
        from datetime import datetime
        now = datetime.now()
        today = datetime(now.year, now.month, now.day)
        
        receiver = User.load(user_id)
        
        q = Greeting.all()
        q.ancestor(group.key())
        q.filter("sender =", user)
        q.filter("receiver =", receiver)
        q.filter("create_date >", today)
        
        if q.count() > 0:
            return HttpResponse("每天只能问一次哦.")
        else:
            Greeting(parent=group, sender=user, receiver= receiver).put()
            return HttpResponse("OK")
        
    def group_vote(self, request, user, group, msg_id, weight):
        
        msg = GroupMessage.load(group.id, msg_id)
        q = GroupVote.all()
        q.filter("voter =", user)
        q.filter("message =", msg)
        vote = q.get()
        if vote is None:
            GroupVote(parent=group, message=msg, voter=user, value=int(weight)).put()
            msg.weight += int(weight)
            msg.put()
            return HttpResponse("OK")
        else:
            return HttpResponse(vote.value)
        
    def group_last_active(self, request, user, group, ajax_html=False):
        
        views = GroupViews(group, user)
        views.action = 'actives'
        views.cur_actives = views.last_actives()
        
        template = ajax_html and 'group_ajax_actives.html' or 'group_group_index.html'
            
        return render_to_response(template, 
                                  {'v':views,
                                   'my': UserViews(user),
                                   'invate_url': self.invate_url(request,group)
                                  })
        
    
    def group_member_active(self, request, user, group):
        views = GroupViews(group, user)
        views.action = 'members'
        views.cur_actives = views.member_actives()
        
        return render_to_response('group_group_index.html', 
                                  {'v':views,
                                   'my': UserViews(user),
                                   'invate_url': self.invate_url(request, group)
                                   })
    
    def group_important_news(self, request, user, group):
        views = GroupViews(group, user)
        views.action = 'news'
        views.cur_actives = views.import_news()
        
        return render_to_response('group_group_index.html', 
                                  {'v':views,
                                   'my': UserViews(user),
                                   'invate_url': self.invate_url(request,group)                                   
                                  })
    
    def group_memory(self, request, user, group):
        views = GroupViews(group, user)
        views.action = 'history'
        views.cur_actives = views.history_news()
        
        return render_to_response('group_group_index.html', 
                                  {'v':views,
                                   'my': UserViews(user),
                                   'invate_url': self.invate_url(request,group)
                                  })
        
    def group_setting(self, request, user, group):
        
        setting = GroupMember.all().ancestor(group.key()).filter('member =', user).get()
        if request.method == 'POST':
            f = request.REQUEST
            if f['truename']: setting.truename = f['truename']
            if f['email']: setting.email = f['email'] 
            if f['mobile']: setting.mobile = f['mobile'] 
            if f['qq']: setting.qq = f['qq']
            setting.put()
            self.is_invited(group, setting, True)
            
            return HttpResponseRedirect('/group/actives/%s' % group.id)
        
        return render_to_response('group_group_setting.html', 
                                  {'title': '修改圈子名片',
                                   'v':setting,
                                   'my': UserViews(user), })        
    
    def group_create(self, request, user):
        
        error_msg = None
        group = Group(name=u'圈子名', status='1', description="")
        if request.method == 'POST':
            error_msg = ""
            f = request.REQUEST
            group.name = unicode(f['name'])
            group.description = unicode(f['description'])
            group.status = f['status']
            
            if not group.name: error_msg = "圈子名字不能为空"
            elif not group.description: error_msg = "圈子描述不能为空"
            elif Group.all().filter('name =', group.name).count() > 0:
                error_msg = "同名圈子已存在!"
            else:
                group.creator = user
                group.put()
                self.group_join(request, user, group)
                return HttpResponseRedirect('/group/invite/%s' % group.id)
            
        return render_to_response('group_edit_or_add.html', 
                                  {'title': '创建新的圈子',
                                   'error_msg':error_msg,
                                   'group': group,
                                   'my': UserViews(user), })

    def group_doing(self, request, user, group, message=None):
        text = message or request.REQUEST['text'].strip()
        text = unicode(text)
        
        b_index = text.count("]") and text.index("]") or 0
        if text.startswith("[") and  b_index < 10:
            category = text[1: b_index]
            text = text[b_index + 1:].strip()
        else:
            category = u"状态"
        
        gm = GroupMember.all().ancestor(group.key()).filter("member =", user).get()
        m = GroupMessage(parent=group, author=gm, category=category, text=text)
        m.put()
        gm.last_message_id = str(m.id)
        gm.last_update = m.create_date
        gm.put()
        
        return self.group_last_active(request, user, group, ajax_html=True)
        #return HttpResponse("OK")
        
    def group_member_info(self, request, user, group, user_id):
        group_views = GroupViews(group)
        group_views.cur_actives = group_views.member_detail_actives(user_id, )
        
        #memeber = User.objects.get(id=user_id)
        memeber = User.load(user_id)
        user_views = UserViews(memeber)
        memeber = GroupMember.all().ancestor(group.key()).filter('member =', memeber).get()
         
        return render_to_response('group_member_info.html', 
                                  {'user':user_views,
                                   'v':group_views,
                                   'member': memeber,
                                   'my': UserViews(user),
                                   'invate_url': self.invate_url(request,group)
                                  })
        #member_detail_actives
        #pass
    
    def group_edit(self, request, user, group):
        
        if user.key() != group.creator.key():
            return HttpResponseRedirect('/group/actives/%s' % group.id)
        
        error_msg = ''
        
        if request.method == 'POST':
            error_msg = ""
            f = request.REQUEST
            #group.name = f['name']
            group.description = f['description']
            group.status = f['status']
            group.put()
            return HttpResponseRedirect('/group/actives/%s' % group.id)
            
        return render_to_response('group_edit_or_add.html', 
                                  {'title': '修改圈子描述信息',
                                   'error_msg':error_msg,
                                   'group': group,
                                   'my': UserViews(user), })
            
    def group_join(self, request, user, group):
        
        if self.is_exists(group, user):
            return HttpResponse("EXISTS")
        
        if group.creator == user or \
           self.is_invited(group, user, True) or \
           group.status == '0': #任何人都可以加入
            GroupMember(parent=group, member=user).put()
            self.group_doing(request, user, group, message=u"我来啦。。。")
            return HttpResponse("OK")
        elif group.status == '2': #验证后才能加入
            return HttpResponse("WAIT")
        else:
            return HttpResponse("ERROR")

    def is_invited(self, group, user, update=False):
        for i in GroupInvitedMember.all().ancestor(group.key()):
            if user.truename and i.truename == user.truename or \
               user.email and i.email == user.email or \
               user.qq and i.qq == user.qq or\
               user.mobile and i.mobile == user.mobile:
                if update is True:
                    i.user_id = str(user.user_id)
                    i.status = '1'
                    i.put()
                return True
            
        return False
    
    def is_exists(self, group, user):
        return self._group_member_query(user, group).count() > 0
        
    def is_member(self, user, group):
        return self._group_member_query(user, group).filter('status =', '0').count() > 0
    
    def _group_member_query(self, user, group):
        return GroupMember.all().ancestor(group.key()).filter('member =', user)

    def group_invite_memebers(self, request, user, group):
        if request.method == 'POST':
            f = request.REQUEST
            m = GroupInvitedMember(parent=group, invitor=user)
            m.truename = f['truename']
            m.email = f['email']
            m.mobile = f['mobile']
            m.qq = f['qq']
            if not m.truename:
                return HttpResponse("名字不能为空.")
            else:
                if GroupInvitedMember.all().ancestor(group.key()).\
                     filter('truename = ', m.truename).count() > 0:
                    return HttpResponse("同名成员已存在.")
                else:
                    m.put()
                    return HttpResponse("OK")

        else:
            invite_list = GroupInvitedMember.all().ancestor(group.key()).fetch(500)
        return render_to_response('group_invite_memebers.html', {
                                                       'title': '管理圈子成员',
                                                       'group': group,
                                                       'invite_list':invite_list,
                                                       'my': UserViews(user)
                                                       })
    
    def invate_url(self, r, group):
        host = (r.META["SERVER_NAME"])
        port = (r.META["SERVER_PORT"])
        port = port != '80' and ":%s" % port or ""
        return "http://%s%s/reg?gid=%s" % (host, port, group.id)
        
class GroupViews(object):
    
    def __init__(self, group, user=None):
        self.group = group
        self.cur_user = user
        
    def _base_message_query(self):
        return GroupMessage.all().ancestor(self.group.key()).\
                                  filter('reply =', None).\
                                  filter('status =', '0')        
        
    def last_actives(self, offset=0, limit=100,):
        offset, limit = int(offset), int(limit)
        return self._base_message_query().order('-create_date').fetch(limit, offset)
        
    
    def member_actives(self):
        members = GroupMember.all().ancestor(self.group.key()).\
                                    filter('status =', '0').order('-last_update').fetch(100)
        
        return [e.last_message for e in members if e.last_message]
    
    def import_news(self, offset=0, limit=100,):
        offset, limit = int(offset), int(limit)

        return self._base_message_query().filter('weight >', 0).\
                        order('-weight').fetch(limit, offset)
    
    def history_news(self, offset=0, limit=100,):
        offset, limit = int(offset), int(limit)
        
        return self._base_message_query().\
                        order('create_date').fetch(limit, offset)

    def member_detail_actives(self, user_id, offset=0, limit=100,):
        offset, limit = int(offset), int(limit)
        query = self._base_message_query()
        gm = GroupMember.all().ancestor(self.group.key()).filter('member =', User.load(user_id)).get()
        query.filter('author =', gm)
        
        return query.order('-create_date').fetch(limit, offset)

    def have_no_register(self, offset=0, limit=100,):
        return self._no_register_query().count() > 0
        
    def no_register(self, offset=0, limit=100,):
        offset, limit = int(offset), int(limit)
        return self._no_register_query().fetch(limit, offset)
                                      
    def _no_register_query(self):
        return GroupInvitedMember.all().ancestor(self.group.key())\
                                      .filter('status =', '0').order('-create_date')
                                      
    def monthly_ask_count(self,):
        from datetime import datetime
        now = datetime.now()
        today = datetime(now.year, now.month, 1)
        return self._greeting_query().filter('create_date >', today).count()

    def all_ask_count(self,):
        return self._greeting_query().count()
                                   
    def _greeting_query(self):
        return Greeting.all().ancestor(self.group.key())\
                             .filter('receiver =', self.cur_user)
                                   
    def empty(self):
        return not self.cur_actives or len(self.cur_actives) == 0
    
    def is_creator(self):
        return self.group.creator.key() == self.cur_user.key()
    
    #def invate_                       
    
    def __getattr__(self, name):
        if name == 'author':
            self.author = GroupMember.all().ancestor(self.group.key())\
                .filter("member =", self.group.creator).get()
            return self.author
        
    