from django import template
from mysite.fb_book.book.enum_code import enum as load_enum
register = template.Library()

@register.filter(name='enum')
def enum(v, name='bill_type'):
    return load_enum(name).value(v).desc

@register.filter(name='next_page')
def next_page(r, label='next page'):
    (query_string, cur_offset, limit) = parse_url_param(r)
        
    return "<a href='?%s&offset=%s'>%s</a>" % (query_string, cur_offset + limit, label)

@register.filter(name='pre_page')
def pre_page(r, label='previous page'):
    (query_string, cur_offset, limit) = parse_url_param(r)
        
    return "<a href='?%s&offset=%s'>%s</a>" % (query_string, max(cur_offset - limit, 0), label)

def parse_url_param(r):
    import re
    query = r.META['QUERY_STRING'] or ''
        
    #    offset=0, limit=30
    cur_page = re.search("offset=([-]?\d+)", query, re.I)
    if cur_page:
        query = re.sub("offset=([-]?\d+)&?", "", query)
        cur_page = int(cur_page.group(1))
    else:
        cur_page = 0

    limit = re.search("limit=(\d+)", query, re.I)
    limit = limit and int(limit.group(1)) or 30
    
    return (query.strip("&"), cur_page, limit)

@register.filter(name='bugtype')
def bugtype(v):
    return {"P":"Pronto", "TA":"Ipa Ta"}.get(v, v)

@register.filter(name='s2t')
def sec2time(v):
    from ipata.analyser.utils import TimeUtil
    return TimeUtil.secs_to_timestr(v) 

#register.filter('rbtime', robot_time)

#register.filter(robot_time2)
