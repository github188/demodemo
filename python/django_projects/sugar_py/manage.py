#!/usr/bin/env python
from django.core.management import setup_environ
from django.core.management import ManagementUtility
try:
    import settings # Assumed to be in the same directory.
except ImportError:
    import sys
    sys.stderr.write("Error: Can't find the file 'settings.py' in the directory containing %r. It appears you've customized things.\nYou'll have to run django-admin.py, passing it your settings module.\n(If the file settings.py does indeed exist, it's causing an ImportError somehow.)\n" % __file__)
    sys.exit(1)
        
if __name__ == "__main__":
    import sys, os
    cwd = os.path.abspath(os.path.dirname(__file__))
    os.chdir(cwd)
    sys.path.remove(cwd)
    sys.path.append(cwd)
    
    setup_environ(settings)
        
    from django.db.models.signals import post_syncdb
    from sugar_py.utils import user_post_syncdb
    post_syncdb.connect(user_post_syncdb)

    ManagementUtility().execute()