# -*- coding: utf-8 -*-

from django import http, template
def add(r ):
    return ("cztv/add_new_house.html", {'form': 'this is test',
                                        'title': "添加房产"})