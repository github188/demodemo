from django import template
from django.template.defaultfilters import stringfilter
import re

register = template.Library()

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
