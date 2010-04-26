# Create your views here.

from django.shortcuts import render_to_response

import pyshop.coreapp.models as m
def index(r, x):
    categors = m.Classifier.objects.all().order_by("name")

    return render_to_response("projects.html", {"categors": categors})
    
