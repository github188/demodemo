import logging, os, sys

# Google App Engine imports.
from google.appengine.ext.webapp import util

# Remove the standard version of Django.
for k in [k for k in sys.modules if k.startswith('django')]:
  del sys.modules[k]

# Force sys.path to have our own directory first, in case we want to import
# from it.
sys.path.insert(0, os.path.join(os.path.abspath(os.path.dirname(__file__)), 'libs'))
def add_eggs_to_sys_path(arg, dirname, names):
  for f in names:
    path = os.path.join(dirname, f)
    if path.endswith('.egg'): sys.path.insert(0, path)
os.path.walk(os.path.join(os.path.abspath(os.path.dirname(__file__)), 'eggs'), add_eggs_to_sys_path, None)
sys.path.insert(0, os.path.abspath(os.path.dirname(__file__)))

# Force Django to reload its settings.
from django.conf import settings
settings._target = None

# Must set this env var before importing any part of Django
os.environ['DJANGO_SETTINGS_MODULE'] = 'rfdoc.settings'

import logging
import django.core.handlers.wsgi
import django.core.signals
import django.db
import django.dispatch.dispatcher

def log_exception(*args, **kwds):
  logging.exception('Exception in request:')

from django.core.signals import got_request_exception
# Log errors.
got_request_exception.connect(log_exception)
# Unregister the rollback event handler.
got_request_exception.disconnect(django.db._rollback_on_exception)

def main():
  # Create a Django application for WSGI.
  application = django.core.handlers.wsgi.WSGIHandler()

  # Run the WSGI CGI handler with that application.
  util.run_wsgi_app(application)

if __name__ == '__main__':
  main()