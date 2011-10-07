
from django.shortcuts import render_to_response
from django.http import HttpResponse, HttpResponseRedirect
import logging
from Weibo.weibopy.auth import OAuthHandler
from Weibo.weibopy.api import API
from Weibo.params.models import *
from Weibo.settings import APP_ROOT
from django import template

try:
    import json
except:
    import simplejson as json
    
def load_params(r, uid=''):
    pass


def config(r, pid):
    pass