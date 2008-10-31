from __future__ import with_statement
#-*- encoding:utf-8 -*-

from portal import render, redirect
from portal.model import cur_pm
import types
import portal.models.resource as model
import web
import os
import base64
from time import localtime, strftime
import md5


class Sales(object):
    
    urls = ()
    def __init__(self):
        urls = []
        urls.extend(self.urls)
        for name in [ e for e in  dir(self) if not e.startswith("_") and \
                     isinstance(getattr(self, e), types.MethodType) ]:
            urls.append(name)
            urls.append(name)
        
        self.urls = urls
        
    def index(self):
        render('index', '中文')
        
    def story(self, conn_id='test'):
        """ 
        @param: conn_id
        """
        render('story_list', '中文')

    def reg(self, name='', password='', email=''):
        """ 
        @param: name, password, email
        """
        if web.ctx.method == 'POST':
            u = model.User(name, email, password)          
            self.pm.persist(u)
        
        render('reg')
        
    def study(self):
                        
        o = self.pm.load_obj('User', uid)
        if not o.order_id:
            redirect('/sales/pay')
        
        #study
        # 1。阅读
        # 3. 评分。
        # 2. 工作记录
        # 3. 感悟 
        
        render('study')
        
    def pay(self, c='100'):
        """
        @param: c
        """
                
        v_mid = "20452979"
        v_key = "myww028.comtest"
        #20081008-20452979-214
        date = strftime('%Y%m%d', localtime())
        v_oid = "%s-%s-a%s" % (date, v_mid, self.auth.user())
        v_url = "http://www.bestsales.com/sales/pay_back"
        text = c + 'CNY' + v_oid + v_mid + v_url + v_key
        
        v_md5 = md5.new(text).hexdigest().upper()

        pay = {'v_mid':v_mid, 'v_oid':v_oid, 
               'v_amount':c, 'v_url':v_url,
               'v_md5info':v_md5 
               }
          
        render('pay', pay)
        
    def pay_back(self, v_oid='00', v_pstatus='30'):
        """
        @param: v_oid, v_pstatus
        """
        
        if v_pstatus == '20':
            uid = v_oid.split('a')[-1]
            o = self.pm.load_obj('User', uid)
            o.order_id = v_oid
            o.order_date = strftime('%m/%d/%Y', localtime())
            
            self.pm.persist(o)
            
        redirect('/sales/study')
        
        
    def story_detail(self, id='01'):
        """ 
        @param: id
        """
        import config
        webroot = config.views_config['template']
        
        story_file = os.path.join(webroot, 'book', "%s.txt" % id)
        if not os.path.isfile(story_file):
            return "Not found id %s" % id
        
        #f = open("story_file")
        content = []
        with open(story_file) as f:
            for line in f:
                content.append(line)
        
        render('story', content)        
        
    def __auto__(self):
        self.pm = cur_pm()
        self.auth = Auth()
        
        gest = self.auth.user()
        if not gest: self.auth.user('1')
        render('common_header')

    def __post__(self):
        render('common_footer')
        self.auth.update_session()
        
        
import random
class Auth():
    def user(self, name=''):
        if name not in [ None, '' ]:
            web.setcookie('sid', self._set_username(name), 60 * 30)
        else:
            i = web.input()
            sid = web.cookies(sid='').sid
            if not sid and i.has_key('sid'): sid = i['sid']
                            
            return self._get_username(sid) or '01'
    
    def update_session(self):
        u = self.user()
        if u: self.user(u)
    
    def _get_username(self, sid):
        if sid in [ None, '' ]: return None
        
        data = base64.b64decode(sid)
        
        return "%s" % (int(data.split(' ')[0]))

        #session = web.cookies(sid='0')
        
    
    def _set_username(self, name):
        return base64.b64encode('%05d %03d' % (int(name), random.randrange(100, 999)))
    
if __name__ == "__main__":
    auth = Auth()
    print auth._set_username('2')
    print auth._get_username('MDAwMDIgNTk4MDAwMDIgNTk4MDAwMDIgNTk4MDAwMDIgNTk4')
    
    v_md5 = md5.new("MDAwMDIgNTk4").hexdigest()
    print "MD5:%s" % v_md5

    
    
