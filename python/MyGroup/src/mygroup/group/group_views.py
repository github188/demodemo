# -*- coding: utf-8 -*-
from django.http import HttpResponse, HttpResponseRedirect
from django.shortcuts import render_to_response

from datetime import datetime
from mygroup.group.views import cur_user
from mygroup.group.user_views import UserViews
from mygroup.group.models import *
import traceback

class GroupActions(object):
    
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
                action_args.append(Group.objects.get(id=args[0]))
                args.remove(args[0])
                if not self.is_member(user, action_args[-1]):
                    return self.group_guest(*action_args)
            action_args.extend(args)
            action = self.actions.get(action, self.group_unkown)
            return action(*action_args)
        
        except Exception, e:
           print "%s\n%s" %(e, traceback.format_exc())
           raise
           #return HttpResponse("Error")

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
        if Greeting.objects.filter(sender=user, group=group, 
                                   receiver_id=user_id,
                                   create_date__gt=today).count() > 0:
            return HttpResponse("每天只能问一次哦.")
        else:
            Greeting(sender=user, group=group, receiver_id=user_id).save()
            return HttpResponse("OK")
        
    def group_vote(self, request, user, group, msg_id, weight):
        msg = GroupMessage.objects.get(id=msg_id)
        vote, created = GroupVote.objects.get_or_create(voter=user, message=msg)
        if created:
            msg.weight += int(weight)
            msg.save()
            vote.value = weight
            vote.save()
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
                                   'my': UserViews(user), })
        
    
    def group_member_active(self, request, user, group):
        views = GroupViews(group, user)
        views.action = 'members'
        views.cur_actives = views.member_actives()
        
        return render_to_response('group_group_index.html', 
                                  {'v':views,
                                   'my': UserViews(user), })
    
    def group_important_news(self, request, user, group):
        views = GroupViews(group, user)
        views.action = 'news'
        views.cur_actives = views.import_news()
        
        return render_to_response('group_group_index.html', 
                                  {'v':views,
                                   'my': UserViews(user), })
    
    def group_memory(self, request, user, group):
        views = GroupViews(group, user)
        views.action = 'history'
        views.cur_actives = views.history_news()
        
        return render_to_response('group_group_index.html', 
                                  {'v':views,
                                   'my': UserViews(user), })
        
    def group_setting(self, request, user, group):
        
        setting = GroupMember.objects.get(group=group, member=user)
        if request.method == 'POST':
            f = request.REQUEST
            if f['truename']: setting.truename = f['truename']
            if f['email']: setting.email = f['email'] 
            if f['mobile']: setting.mobile = f['mobile'] 
            if f['qq']: setting.qq = f['qq']
            setting.save()
            self.is_invited(group, setting, True)
            
            return HttpResponseRedirect('/group/actives/%s' % group.id)
        
        return render_to_response('group_group_setting.html', 
                                  {'title': '修改圈子名片',
                                   'v':setting,
                                   'my': UserViews(user), })        
    
    def group_create(self, request, user):
        
        error_msg = None
        group = Group(status='1')
        if request.method == 'POST':
            error_msg = ""
            f = request.REQUEST
            group.name = f['name']
            group.description = f['description']
            group.status = f['status']
            
            if not group.name: error_msg = "圈子名字不能为空"
            elif not group.description: error_msg = "圈子描述不能为空"
            elif Group.objects.filter(name=group.name).count() > 0:
                error_msg = "同名圈子已存在!"
            else:
                group.creator = user
                group.save()
                self.group_join(request, user, group)
                return HttpResponseRedirect('/group/invite/%s' % group.id)
            
        return render_to_response('group_edit_or_add.html', 
                                  {'title': '创建新的圈子',
                                   'error_msg':error_msg,
                                   'group': group,
                                   'my': UserViews(user), })

    def group_doing(self, request, user, group, message=None):
        text = message or request.REQUEST['text'].strip()
        
        b_index = text.count("]") and text.index("]") or 0
        if text.startswith("[") and  b_index < 10:
            category = text[1: b_index]
            text = text[b_index + 1:].strip()
        else:
            category = "状态"
        
        gm = GroupMember.objects.get(group=group, member=user)
        m = GroupMessage(author=gm, group=group, category=category, text=text)
        m.save()
        gm.last_message_id = m.id
        gm.last_update = m.create_date
        gm.save()
        
        return self.group_last_active(request, user, group, ajax_html=True)
        #return HttpResponse("OK")
        
    def group_member_info(self, request, user, group, user_id):
        group_views = GroupViews(group)
        group_views.cur_actives = group_views.member_detail_actives(user_id, )
        
        memeber = User.objects.get(id=user_id)
        user_views = UserViews(memeber)
        memeber = GroupMember.objects.get(group=group, member=memeber)
         
        return render_to_response('group_member_info.html', 
                                  {'user':user_views,
                                   'v':group_views,
                                   'member': memeber,
                                   'my': UserViews(user), })
        #member_detail_actives
        #pass
    
    def group_edit(self, request, user, group):
        
        if user != group.creator:
            return HttpResponseRedirect('/group/actives/%s' % group.id)
        
        error_msg = ''
        
        if request.method == 'POST':
            error_msg = ""
            f = request.REQUEST
            #group.name = f['name']
            group.description = f['description']
            group.status = f['status']
            group.save()
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
            GroupMember(group=group, member=user).save()
            self.group_doing(request, user, group, message="我来啦。。。")
            return HttpResponse("OK")
        elif group.status == '2': #验证后才能加入
            return HttpResponse("WAIT")
        else:
            return HttpResponse("ERROR")

    def is_invited(self, group, user, update=False):
        for i in GroupInvitedMember.objects.filter(group=group):
            if user.truename and i.truename == user.truename or \
               user.email and i.email == user.email or \
               user.qq and i.qq == user.qq or\
               user.mobile and i.mobile == user.mobile:
                if update is True:
                    i.user_id = user.user_id
                    i.status = 1
                    i.save()
                return True
            
        return False
    
    def is_exists(self, group, user):
        return GroupMember.objects.filter(group=group, member=user).count() > 0
        
    def is_member(self, user, group):
        return GroupMember.objects.filter(group=group, member=user, status='0').count() > 0

    def group_invite_memebers(self, request, user, group):
        if request.method == 'POST':
            f = request.REQUEST
            m = GroupInvitedMember()
            m.truename = f['truename']
            m.email = f['email']
            m.mobile = f['mobile']
            m.qq = f['qq']
            if not m.truename:
                return HttpResponse("名字不能为空.")
            else:
                if GroupInvitedMember.objects.filter(group=group, truename=m.truename).count() > 0:
                    return HttpResponse("同名成员已存在.")
                else:
                    m.group = group
                    m.invitor = user
                    m.save()
                    return HttpResponse("OK")

        else:
            invite_list = GroupInvitedMember.objects.filter(group=group)
        return render_to_response('group_invite_memebers.html', {
                                                       'title': '管理圈子成员',
                                                       'group': group,
                                                       'invite_list':invite_list,
                                                       'my': UserViews(user)
                                                       })
        
class GroupViews(object):
    
    def __init__(self, group, user=None):
        self.group = group
        self.cur_user = user
        
    def last_actives(self, offset=0, limit=100,):
        
        offset, limit = int(offset), int(limit)
        return GroupMessage.objects.filter(group=self.group, 
                                      reply_id=0, 
                                      status='0').order_by('-create_date')\
                                      [offset: offset+limit]
        
    
    def member_actives(self):
        members = GroupMember.objects.filter(group=self.group, 
                                      status='0').order_by('-last_update')
        return (e.last_message for e in members)
    
    def import_news(self, offset=0, limit=100,):
        offset, limit = int(offset), int(limit)
        
        return GroupMessage.objects.filter(group=self.group, 
                                      reply_id=0,
                                      status='0',
                                      weight__gt=0).order_by('-weight',
                                      '-create_date')\
                                      [offset: offset+limit]
    
    def history_news(self, offset=0, limit=100,):
        offset, limit = int(offset), int(limit)
        
        return GroupMessage.objects.filter(group=self.group, 
                                      reply_id=0,
                                      status='0').order_by('create_date', 
                                      '-weight')\
                                      [offset: offset+limit]

    def member_detail_actives(self, user_id, offset=0, limit=100,):
        offset, limit = int(offset), int(limit)
        
        return GroupMessage.objects.filter(group=self.group,
                                      author__member__id=user_id, 
                                      reply_id=0,
                                      status='0').order_by('-create_date', 
                                      '-weight')\
                                      [offset: offset+limit]

    def have_no_register(self, offset=0, limit=100,):
        offset, limit = int(offset), int(limit)
        
        return GroupInvitedMember.objects.filter(group=self.group, 
                                      status='0').count() > 0

    def no_register(self, offset=0, limit=100,):
        offset, limit = int(offset), int(limit)
        
        return GroupInvitedMember.objects.filter(group=self.group, 
                                      status='0').order_by('-create_date')
                                      
    def monthly_ask_count(self,):
        from datetime import datetime
        now = datetime.now()
        today = datetime(now.year, now.month, 1)
        return Greeting.objects.filter(group=self.group, 
                                   receiver_id=self.cur_user.user_id,
                                   create_date__gt=today).count()

    def all_ask_count(self,):
        return Greeting.objects.filter(group=self.group, 
                                   receiver_id=self.cur_user.user_id).count()
                                   
    def empty(self):
        return not self.cur_actives or len(self.cur_actives) == 0                                   
    
    def __getattr__(self, name):
        if name == 'author':
            self.author = GroupMember.objects.get(group=self.group, member=self.group.creator)
            return self.author
        
    