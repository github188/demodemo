
import os
#from mygroup.settings import UPLOADED_ROOT
#from mygroup.group.models import UploadedFile
from google.appengine.api import images
from google.appengine.ext import db
import logging

class UploadUtil(object):
    
    @staticmethod
    def handle_uploaded_file(f, name, user, ):
        logging.debug("uploaded image:%s, file size:%s" % (name, f.size))
        user = db.get(user.key())
        data = images.resize(f.read(), 40, 40)
        if data is None:
            logging.warning("fail to convert image, '%s'" % name)
        else:
            user.logo = db.Blob(data)
            user.put()

        
        