# -*- coding: utf-8 -*-

from manage import ContentManage
import logging
from manage import parameter_checking as __validation__
from utils.paging import Paging
manage = ContentManage()


def IP(r): return r.META["REMOTE_ADDR"]

def index(r, cate='lei', lang='', tag='', mode='list', track='web'):
    return list_message(r, "lei", lang, mode=mode)

def list_message(r, cate='lei', lang='', tag='', offset=1, limit=10, mode='list', track=''):
    cate = cate or 'lei'
    lang = lang or 'zh'
    
    cate_count, cate_list = manage.cate_list(lang)
    cate = manage.load_category(cate, lang)
    tag_count, tags_list = manage.tag_list(cate, 1, 100,)
    
    page_url = ""
    if tag:
        message_count, message_list = manage.tag_message(cate, tag, offset, limit, mode, track, IP(r))
        page_url = "/dyd/tag/%s/%s/PAGE" % (cate.code, tag)
    else:
        message_count, message_list = manage.category_message(cate, offset, limit, mode, track, IP(r))
        page_url = "/dyd/cate/%s/PAGE" % (cate.code, )
        
    site_title = "%s-%s" % (cate.name, tag)
    
    keywords = [ e.name for e in tags_list[:10] ]
    
    #message_count 
    message_page = Paging(message_count, offset, page_url, limit)
    
    return ("dyd_index.html", {"lang": lang,
                               "site_title": site_title,
                               "keywords": ",".join(keywords),
                               "message_page_nav": message_page,
                               "cur_cate": cate,
                               "cur_tag": tag,
                               "tags_list": tags_list,
                               "cate_list": cate_list, 
                               "message_count": message_count,
                               "message_list": message_list                               
                               })
        
def post(r, cate='', lang='', message='', tags='', user='', session='', track='', ajax='N'):
    
    cate = manage.load_category(cate, lang)
    (status, err_msg) = manage.post(cate, message, tags, user, track, IP(r))
    
    logging.info("post status:%s, %s, ajax=%s." % (status, err_msg, ajax))
    
    if ajax == 'N':
        return ("redirect:/dyd/cate/%s?mode=new" % cate.code,);
    
    return {"status": status, "message": err_msg}

def vote(r, cate='', lang='', msg_id='', v='', msg='', user='', track=''):
    
    cate = manage.load_category(cate, lang)
    (status, err_msg) = manage.vote(cate, msg_id, v, msg, user, track, IP(r))
    
    return {"status": status, "message": err_msg}
    #return {"status": error, "message": err_msg}

def clear_cache(r, ):
    from google.appengine.api import memcache
    memcache.flush_all()
    return "OK"

def default_view(r):
    return list_message(r, "lei", 'zh')

def init_data(r, ):
    geyan = manage.load_category("geyan", "zh", u"格言")
    shici = manage.load_category("shici", "zh", u"诗词")
    xiao = manage.load_category("xiao", "zh", u"笑话")
    lei = manage.load_category("lei", "zh", u"雷人")
    qq = manage.load_category("qq", "zh", u"QQ签名")
    
    x = '''
    manage.load_tag(geyan, u"上大学 ")
    manage.load_tag(geyan, u"大学生")
    
    manage.load_tag(xiao, u"上大学 ")
    manage.load_tag(xiao, u"大学生")

    manage.load_tag(lei, u"上大学 ")
    manage.load_tag(lei, u"大学生")
    manage.load_tag(lei, u"大学生2")
    manage.load_tag(lei, u"大学生3")
    manage.load_tag(lei, u"大学生4")
    manage.load_tag(lei, u"大学生5")
    manage.load_tag(lei, u"大学生6")
    manage.load_tag(lei, u"大学生7")
    manage.load_tag(lei, u"大学生8")
    manage.load_tag(lei, u"大学生9")
    manage.load_tag(lei, u"大学生10")

    manage.load_tag(qq, u"上大学 ")
    manage.load_tag(qq, u"大学生")
    '''
    #ContentTag
    
    return "init data ok!"

