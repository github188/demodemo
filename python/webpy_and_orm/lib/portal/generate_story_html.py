import server

import portal.urls.sales as app
import urllib, urllib2
import config
import os
webroot = config.views_config['template']

for num in range(1, 20): 
    req = urllib2.Request('http://127.0.0.1:8080/sales/story_detail?id=%02d' % num)
    response = urllib2.urlopen(req)
    story_file = os.path.join(webroot, 's', "%02d.html" % num)
    f = open(story_file, 'w')
    f.write(response.read())
    f.close()
    print "done %s!" % num
