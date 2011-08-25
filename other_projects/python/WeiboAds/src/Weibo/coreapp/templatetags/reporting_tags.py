from django import template
from django.template.defaultfilters import stringfilter
import re

register = template.Library()

@register.filter
@stringfilter
def cur_class(value, args):
    return value == args and "class=cur" or ""

@register.filter
@stringfilter
def hightlight(value, args=''):
    if not args: return value
    
    args = args.replace("*", ".*?")
    args = [ e.strip() for e in re.split(r"[|&]", args) ]
    args = "|".join([ e for e in args if e ])
    #raise Exception, str(args)
    if args:
        text = re.sub(r"(?i)(%s)" % args,
                      lambda r: "<span class='hl'>%s</span>" % r.group(0),
                      value)
    return text

@register.filter
@stringfilter
def break_line(value, len=50):
    text = re.sub(r"([^ <>]{%s})" % len,
                  lambda r: "<span>%s</span> " % r.group(0),
                  value)
    return text

def app_root():
    """
    Returns the string contained in the setting ADMIN_MEDIA_PREFIX.
    """
    try:
        from django.conf import settings
    except ImportError:
        return ''
    return settings.APP_ROOT
app_root = register.simple_tag(app_root)


#register.filter(robot_time2)
