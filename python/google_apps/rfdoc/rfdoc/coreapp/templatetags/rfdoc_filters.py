# Copyright 2009 Nokia Siemens Networks Oyj
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#     http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


from django import template
from django.template.defaultfilters import stringfilter
import re

register = template.Library()

@register.filter
@stringfilter
def nbsp(value):
    return value.replace(' ', '&nbsp;')

@register.filter
@stringfilter
def first_line(value):
    return value.split('\n')[0]

@register.filter
@stringfilter
def hightlight(value, args=''):
    if not args: return value
    args = args.replace("STE:", '')
    args = "|".join(re.split(r"[, ]", args))
    text = re.sub(r"(?i)(%s)" % args, 
                  lambda r: "<span class='hl'>%s</span>" % r.group(0),
                  value)
    return text

@register.filter
@stringfilter
def highlight_deprecated(value):
    text = re.sub(r"(?i)deprecated", 
                  lambda r: "<span class='h2'>%s</span>" % r.group(0),
                  value)
    return text
@register.filter
@stringfilter
def args(value, ):
    #args = value.split(",")
    def var_fmt(r):
        var = r.groups()
        #e.g: unit=NPGEP,0
        if not var[0][0].isalpha(): return"%s%s" % (var[0], var[1] or '')
        return"<span class='arg'>${%s}</span>%s" % (var[0], var[1] or '')
        
    text = re.sub(r"(?i)([\w_]+)(=[^,]*)?",
                  var_fmt,
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

