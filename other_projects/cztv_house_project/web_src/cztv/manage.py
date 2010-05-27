#!/usr/bin/env python

from django.core.management import setup_environ
from django.core.management import ManagementUtility
import logging, sys, os
try:
    import settings # Assumed to be in the same directory.
except ImportError:
    import sys
    sys.stderr.write("Error: Can't find the file 'settings.py' in the directory containing %r. It appears you've customized things.\nYou'll have to run django-admin.py, passing it your settings module.\n(If the file settings.py does indeed exist, it's causing an ImportError somehow.)\n" % __file__)
    sys.exit(1)

def setpythonpath():
    lib_path = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'libs'))
    lib_path = os.path.normpath(lib_path)
    for e in os.listdir(lib_path):
        if e.endswith('.egg'):
            sys.path.append(os.path.join(lib_path, e))

if __name__ == "__main__":
    
    cwd = os.path.abspath(os.path.dirname(__file__))
    os.chdir(cwd)

    setpythonpath()    
    setup_environ(settings)
    
    FORMAT = "%(asctime)s-%(name)s:%(levelname)-6s %(message)s"
    logging.basicConfig(level=logging.INFO, format=FORMAT, stream=sys.stdout) 
    
    from django.db.models.signals import post_syncdb
    from cztv.utils import user_post_syncdb
    post_syncdb.connect(user_post_syncdb)

    ManagementUtility().execute()
