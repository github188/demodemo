#!/usr/local/bin/python
try:
    from fcgi import WSGIServer
    def myapp(environ, start_response):
        start_response('200 OK', [('Content-Type', 'text/plain')])
        return ['Hello World!\n']
    WSGIServer(myapp).run()
except Exception, e:
    print 'Content-Type: text/plain\r\n\r\n',
    print 'Oops...'
    print
    print 'Trac detected an internal error:'
    print
    print e