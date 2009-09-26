# -*- coding: utf-8 -*-
from mygroup.group.models import *

def init_data(sender, app, created_models, verbosity, interactive, **kvargs):
    init_user()
    

def init_user():
    u = User(name='deonwu', truename='四无浪子', password='123', qq='81711368', mobile='13989836454')
    u.save()
    print "save init_user.............."

