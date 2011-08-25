
from django.shortcuts import render_to_response

def guest_index(r):
    return render_to_response("weibo/weibo_base.html", {})