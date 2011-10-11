#!/usr/local/bin/python

import os, sys

#sys.path.insert(0, "/home/username/django/")
#sys.path.insert(0, "/home/username/django/projects")
#sys.path.insert(0, "/home/deon/works/ipata_workspace/WeiboAds/src/Weibo")
#sys.path.insert(0, "/home/deon/works/ipata_workspace/WeiboAds/src")

def setpythonpath():
    lib_path = os.path.abspath(os.path.join(os.path.dirname(__file__)))
    lib_path = os.path.normpath(lib_path)
    for e in os.listdir(lib_path):
        if e.endswith('.egg'):
            sys.path.insert(0, os.path.join(lib_path, e))
setpythonpath()

import django.core.handlers.wsgi

#os.chdir("/home/username/django/projects/newproject") # optional
os.environ['DJANGO_SETTINGS_MODULE'] = "Weibo.settings"

def runcgi():
    environ = dict(os.environ.items())
    environ['wsgi.input'] = sys.stdin
    environ['wsgi.errors'] = sys.stderr
    environ['wsgi.version'] = (1,0)
    environ['wsgi.multithread'] = False
    environ['wsgi.multiprocess'] = True
    environ['wsgi.run_once'] = True

    application = django.core.handlers.wsgi.WSGIHandler()

    if environ.get('HTTPS','off') in ('on','1'):
        environ['wsgi.url_scheme'] = 'https'
    else:
        environ['wsgi.url_scheme'] = 'http'

    headers_set = []
    headers_sent = []
    
    def write(data):
        if not headers_set:
            raise AssertionError("write() before start_response()")    
        elif not headers_sent:
            # Before the first output, send the stored headers
            status, response_headers = headers_sent[:] = headers_set
            sys.stdout.write('Status: %s\r\n' % status)
            for header in response_headers:
                sys.stdout.write('%s: %s\r\n' % header)
            sys.stdout.write('\r\n')
    
        sys.stdout.write(data)
        sys.stdout.flush()
    
    def start_response(status,response_headers,exc_info=None):
        if exc_info:
            try:
                if headers_sent:
                    # Re-raise original exception if headers sent
                    raise exc_info[0], exc_info[1], exc_info[2]
            finally:
                exc_info = None # avoid dangling circular ref
        elif headers_set:
            raise AssertionError("Headers already set!")
        
        headers_set[:] = [status,response_headers]
        return write
    
    result = application(environ, start_response)
    try:
        for data in result:
            if data: # don't send headers until body appears
                write(data)
        if not headers_sent:
            write('') # send headers now if body was empty
    finally:
        if hasattr(result,'close'):
            result.close()
                        
#print "Content-type: text/plain\n\n"
runcgi()
