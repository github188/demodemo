
from sailing.core import Sailor, new_task
from sailing.core.web.website import WebSite
from http_client import HTTPClient
from sailing.conf import settings
from sailing.common.common import join_path

class Spider(Sailor):
    pass
    
    def ready(self):
        self.http = HTTPClient()
        self.http.set_proxy(settings.PROXY)

    def start(self, t):
        
        site = WebSite(t.header('Site'), "", "worker")
        
        next_task = new_task('worker')
        next_task.header('Site', t.header('Site'))
        
        for l in t.list_actions():
            url, save_as = l.split("==>")
            url, save_as = url.strip(), save_as.strip()
            status = self.http.download(url.strip(), self._save_as(site, save_as))
            next_task.add_action("%s %s %s" % (status, url, save_as))
            
        next_task.status = 'waiting'

    def _save_as(self, site, path):
        #path = path.strip().split("/")
        return site.real_path(path)
    