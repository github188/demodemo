from django import template

register = template.Library()


@register.filter(name='fb_canvas_uid')
def fb_canvas_uid(r, ):
    return r.facebook.canvas.user 

