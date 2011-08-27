import re

#from XXXTool.coreapp.models import *
import logging

def tep_context(request):
    url_path = request.path.strip("/")
    url_path = url_path.split("/")[-1]
    
    #result = re.search(r"hudson/(?P<hudson_name>[^/]+)/(?P<hudson_view>[^/?]+)", 
    #                   url_path, re.I)
    
    #cate_list = Category.objects.filter(status='active').order_by("view_order")

    #cate_list = Category.load_for_nav(request)
    
    #tep = Project.tep_project()
            
    return {
        'cur_user': request.session.get("login_user"),
        'cur_weibo': request.session.get("login_weibo"),
        'param': request.REQUEST,
        'url_path': url_path
    }
