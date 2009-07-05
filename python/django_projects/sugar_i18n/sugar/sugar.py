
from django.db import connection, transaction
from models import *
from settings import UPLOADED_ROOT, PHOTO_URL_ROOT
import os

def check_out_quiz(request):
    
    param = request.REQUEST
    yes_count = 0
    for i in range(1, 11):
        if param.get("q%s" % i, None) == "Yes": yes_count += 1
    
    description = None
    answer = Answer.objects.filter(min_yes__lte=yes_count, max_yes__gte=yes_count)
    if answer:
        answer = answer[0]
        answer.user_count += 1
        description = answer.description
        answer.save()
    else:
        answer = None
        
    cursor = connection.cursor()
    cursor.execute("SELECT sum(user_count) FROM %s" % Answer._meta.db_table)
    (totally_count, ) = cursor.fetchone()
    
    answer_list = Answer.objects.all()

    return (answer, totally_count, answer_list)


def save_or_load_sugar(request, fb_uid):
    
    user, created = SugarUser.objects.get_or_create(fb_uid=fb_uid)
    if request.REQUEST.get("submit", None):
        file = request.FILES.get('photo', None)
        upload_file = save_uploaded_photo(file, fb_uid)
        if upload_file:
            user.photo = "%s/%s" % (PHOTO_URL_ROOT, upload_file)
        
        user.gender = request.REQUEST.get('gender', None)
        user.seeking = request.REQUEST.get('seeking', None)
        try:
            c = Country.objects.get(id=request.REQUEST.get('location', None))
            user.location = c
        except:
            pass
        user.description = request.REQUEST.get('description', "")
        user.joined = "Y"
        
        user.save()
    
    if not user.photo:
        user_info = request.facebook.fql.query(query="select pic_big from user where uid=%s" % request.facebook.uid or 614332206)[0]
        user.photo = user_info['pic_big']
        user.save()
        
    return user
    
def save_uploaded_photo(path, uid):
    
    if path is None: return None
    
    file, ext = os.path.splitext(path.name)
    file = "%s%s" % (uid, ext)
    
    save_as = os.path.join(UPLOADED_ROOT, file)
    f = open(save_as, "wb")
    f.write(path.read())
    f.close()
    
    return file

