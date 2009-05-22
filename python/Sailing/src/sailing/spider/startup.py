
from sailing.core.task import FileTask
from sailing.conf import settings
from sailing.spider.http_client import HTTPClient
from sailing.common.common import *
        
class Spider(FileTask):
    
    def __init__(self, input):
        FileTask.__init__(self, input)
        self.http = HTTPClient()
        self.http.set_proxy(settings.PROXY)
    
    def process(self, l):
        url, save_as = l.split("==>")
        status = self.http.download(url.strip(), self._save_as(save_as))

    
    def _save_as(self, path):
        path = path.strip().split("/")
        return join_path("worker", *path)
     
    def post_process(self):
        pass


def main(path):
    #print "spider:%s" % path
    
    sp = Spider(path)
    sp.start()
    sp.close()
    

if __name__ == '__main__':
    import sys
    if len(sys.argv) != 2:
        print 'python -m spider <path>'
    else:
        settings.configure('spider')
        main(sys.argv[1])