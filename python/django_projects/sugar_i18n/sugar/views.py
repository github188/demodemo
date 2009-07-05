# Import the Django helpers
import facebook.djangofb as facebook
from models import *
from utils import list2table
from sugar import check_out_quiz, save_or_load_sugar
from settings import SUGARS_ACTION 

@facebook.require_login()
def home(request):
    
    featured_count = 10
    
    featured = SugarUser.objects.filter(featured="Y").order_by("-join_date")[:featured_count]
    
    if 0 < len(featured) < featured_count:
        featured = [ e for e in featured ]
        featured.extend(featured[:1] * (featured_count - len(featured)))
    
    return ('home.fbml', {'r': request,
                          'tag': 'home',
                          'featured': list2table(featured, 5)
                          })

def quiz(request, submit=""):
    if request.REQUEST.get('submit', None) is None:
        question = Question.objects.all()
        return ('quiz.fbml', {'r': request,
                              'tag': 'quiz',
                              'question_list':question})
    else:
        (a, t, list) = check_out_quiz(request)
        return ('quiz_result.fbml', {'r': request,
                                     'tag': 'quiz',
                                     'description': a.description,
                                     'totally_count': t,
                                     'answer_list': list})

def post(request):
    return ('post.fbml', {'r': request, 'tag': 'post',})

from django.forms import ModelForm
class SugarUserForm(ModelForm):
    class Meta:
        model = SugarUser

def sugars(request, sg_uid=614332206):
    
    list_count = 10
    
    new_sugars = SugarUser.objects.all().order_by("-join_date")[:list_count]
    if 0 < len(new_sugars) < list_count:
        new_sugars = [ e for e in new_sugars ]
        new_sugars.extend(new_sugars[:1] * (list_count - len(new_sugars)))
    
    sugar = save_or_load_sugar(request, sg_uid)
    
    if request.REQUEST.get("submit", None):
        return (request.facebook.get_app_url("sugars"), )
    
    return ('sugars.fbml', {'r': request,
                            'tag': 'sugars',
                            'new_sugars': list2table(new_sugars, 5),
                            'form': SugarUserForm(instance=sugar),
                            'sugars_action': SUGARS_ACTION
                           })
    
def sugar(request, sg_uid):
    
    return ('sugar.fbml', {'r': request,
                           'tag': 'sugars',
                           'sugar': SugarUser.objects.get(fb_uid=sg_uid)})


def invite(request):
    return ('invite.fbml', {'r': request})

@facebook.require_login()
def callback(request):
	pass

