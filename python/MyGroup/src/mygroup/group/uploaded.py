
import os
from mygroup.settings import UPLOADED_ROOT
from mygroup.group.models import UploadedFile

class UploadUtil(object):
    
    @staticmethod
    def handle_uploaded_file(f, name, user, ):
        
        name, ext = os.path.splitext(name)
        ext = ext.lower()
        if ext in ['.gif', '.jpg', '.jpeg']:
            pass
        
        path = "images/show/%s/%06d%s" % (user.id % 10, user.id, ext)
        local_path = os.path.join(UPLOADED_ROOT, path)
        dir_name = os.path.dirname(local_path)
        if not os.path.isdir(dir_name):
            os.makedirs(dir_name)
        
        print "local_path:%s" % local_path
        fd = open(local_path, 'wb')
        
        if ext in ['.jpg', '.jpeg']:
            fd.write(UploadUtil.compress_jpeg(f.read()))
        else:
            fd.write(f.read())
        fd.close()
        
        upload = UploadedFile(owner=user, path=path)
        upload.save()
        
        user.logo = "/%s" % path
        user.save()
    
    @staticmethod    
    def compress_jpeg(data):
        return data
        
        