# Create your views here.

from django.http import HttpResponse, HttpResponseRedirect
from django.shortcuts import render_to_response
from django import template

from month_report import month_report

def index(r, *args):
    return HttpResponseRedirect("admin/")

