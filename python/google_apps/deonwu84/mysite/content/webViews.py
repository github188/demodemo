# -*- coding: utf-8 -*-

from manage import ContentManage

from manager import parameter_checking as __validation__
manage = ContentManage()

def IP(r): return r.META["REMOTE_ADDR"]

def index(r, cate='lei', lang='', tag='', offset=0, limit=50, mode='list', track=''):
    return cate(r, "lei", lang, mode=mode)

def cate(r, cate='lei', lang='', offset=0, limit=50, mode='list', track=''):
    
    cate = manage.load_category(cate, lang)
    cate_count, cate_list = manage.cate_list(lang)
    tag_count, tags_list = manage.tag_list(cate, 0, 100,)
    
    message_count, message_list = manage.category_message(cate, offset, limit, mode, track, IP(r))
    
    return ("dyd_index.html", {"lang": lang,
                               "tags_list": tags_list,
                               "cate_list": cate_list, 
                               "message_count": message_count,
                               "message_list": message_list
                               })

def tag(r, cate='', lang='', tag='', offset=0, limit=50, mode='list', track=''):
        
    return (count, r)

        
def post(r, cate='', lang='', message='', tags='', user='', session='', track=''):
    
    cate = manage.load_category(cate, lang)
    (status, message) = manage.post(category, message, tags, user, track, IP(r))
    
    return {"status": error, "message": message}

def init_data(r, ):
    geyan = manage.load_category("geyan", "zh", "格言")
    xiao = manage.load_category("xiao", "zh", "笑话")
    lei = manage.load_category("lei", "zh", "雷人")
    qq = manage.load_category("qq", "zh", "QQ签名")
    
    manage.load_tag(geyan, "上大学 ")
    manage.load_tag(geyan, "大学生")
    
    manage.load_tag(xiao, "上大学 ")
    manage.load_tag(xiao, "大学生")

    manage.load_tag(lei, "上大学 ")
    manage.load_tag(lei, "大学生")
    manage.load_tag(lei, "大学生2")
    manage.load_tag(lei, "大学生3")
    manage.load_tag(lei, "大学生4")
    manage.load_tag(lei, "大学生5")
    manage.load_tag(lei, "大学生6")
    manage.load_tag(lei, "大学生7")
    manage.load_tag(lei, "大学生8")
    manage.load_tag(lei, "大学生9")
    manage.load_tag(lei, "大学生10")

    manage.load_tag(qq, "上大学 ")
    manage.load_tag(qq, "大学生")
    #ContentTag
    
    return (count, r)
