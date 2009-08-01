
# support Google App Engine.  GAE does not have a working urllib.urlopen.
try:
    from google.appengine.api import urlfetch

    def urlread(url, data=None, headers=None):
        if data is not None:
            if headers is None:
                headers = {"Content-type": "application/x-www-form-urlencoded"}
            method = urlfetch.POST
        else:
            if headers is None:
                headers = {}
            method = urlfetch.GET

        result = urlfetch.fetch(url, method=method,
                                payload=data, headers=headers)
        
        if result.status_code == 200:
            return result.content
        else:
            raise urllib2.URLError("fetch error url=%s, code=%d" % (url, result.status_code))

except ImportError:
    def urlread(url, data=None):
        res = urllib2.urlopen(url, data=data)
        return res.read()
    