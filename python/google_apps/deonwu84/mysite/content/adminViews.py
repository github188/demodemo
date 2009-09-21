
from manage import ContentManage
manage = ContentManage()

from manage import parameter_checking as __validation__

def tag_list(r, cate='lei', lang='zh'):    
    cate = manage.load_category(cate, lang)
    tag_count, tags_list = manage.tag_list(cate, 1, 100,)
    cate_count, cate_list = manage.cate_list(lang)
    
    return ("dyd_admin_tag.html", {
                                   'cur_cate': cate,
                                   "tags_list": tags_list,
                                   "cate_list": cate_list,                                    
                                   })
    
    
def update_tag(r, cate='', lang='', tag='', shape=''):
    cate = manage.load_category(cate, lang)
    tag = manage.load_tag(cate, tag)
    tag.shape = shape
    tag.put()
    manage.clear_cache()
    
    return ("redirect:/dyd/admin/tag_list", ) #tag_list(r, cate.code, lang)