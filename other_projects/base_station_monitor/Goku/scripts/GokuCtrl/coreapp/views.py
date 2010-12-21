# Create your views here.

from django.http import HttpResponse, HttpResponseRedirect
def index(r, *args):
    return HttpResponseRedirect("/admin")