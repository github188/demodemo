
from models import *
from rpc_proxy import JSON_Proxy

def connect(server_url, secondary_url=None):
    if not server_url.startswith("http://"):
        server_url = "http://%s" % server_url
    import goku.models as m

    proxy = JSON_Proxy(server_url)
    m.server = CenterServer(proxy)
