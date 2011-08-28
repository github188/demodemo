# -*- coding: utf-8 -*-

class Credit(object):    
    @staticmethod
    def get_profile_credit(profile, param=None):
        """计算用户的信用评分 """
        if not param: param = Credit.credit_param()        
        result = 0.0
        
        result_details = {}
        for factor, p in param.iteritems():
            user_data = profile.get_credit_data(factor)
            if not user_data:
                c_data = p.default_value
            else:
                c_data = p.get_data_level(user_data)
            result += c_data * p.weight
            result_details[factor + "_level"] = c_data
        profile.credit_level = result
        profile.update_credit_details(result_details)
        
        return result
    
    @staticmethod
    def credit_param(source='weibo'):
        """ 取得信用评分参数             
            参数名：[1,2,3,4,5,6,7,10]
        """
        from models import CreditParameter
        param_list = CreditParameter.objects.filter(data_source=source)                
        data = {}
        for p in param_list:
            data[p.name] = p
        return data
        
def update_access_key(instance=None, **kw):
    from models import APPAccessKey, CreditQueue
    def f():
        ap, c = APPAccessKey.objects.get_or_create(app_key=instance.app_key, user_id=instance.weibo_id)
        ap.app_secret = instance.app_token
        ap.access_key = instance.authon_key
        ap.access_secret = instance.authon_token
        ap.save()        
        _update_simple_credit(instance)
        #添加到信用评分更新队列等待更新。
        q, c = CreditQueue.objects.get_or_create(uuid='%s:%s' % (instance.weibo_source,
                                                                 instance.weibo_id))
        if q.priorty < 1000:
            q.priorty = 1000
            q.save()
    import thread
    thread.start_new_thread(f, ())

def update_credit_report(instance=None, **kw):
    if instance.status != 'updated' or instance.credit_level <= 0: return
    from models import WeiboProfile    
    source, id = instance.uuid.split(":", 1)
    WeiboProfile.objects.filter(weibo_id=id, weibo_source=source).\
        update(credit_level=instance.credit_level)
        
def _update_simple_credit(r):
    from models import ProfileCredit
    p, c = ProfileCredit.objects.get_or_create(uuid='%s:%s' % (r.weibo_source,
                                                               r.weibo_id))
    p.update_credit_data({'fans': r.fans_count, 
                          'friends':  r.friends_count,
                          'weibo':  r.weibo_count})
    Credit.get_profile_credit(p, )
    p.save()

