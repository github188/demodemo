# -*- coding: utf-8 -*-

import sys
import re
from time import localtime, strftime
import logging
import json
import os    
import sqlite3

def init_path():
    cur_dir = os.path.dirname(os.path.abspath(__file__))
    lib_path = os.path.join(cur_dir, 'libs')
    sys.path.insert(0, lib_path)
        
class GetTaokDetail(object):
    def __init__(self, ):
        from taobao import Taobao
        from oss.oss_api import *

        self.logger = logging.getLogger("taoke")
        self.taobao = Taobao('12395385', '53697d99eccd670191af0603d7256f77')
        
    def generate_cate_tree(self, site, http, pid):
    
        parents_id = [pid, ]
        while len(parents_id) > 0:
            tmp_list = parents_id
            parents_id = []
            for cid in tmp_list:
                sub_cate = self._get_cate_list(cid)
                parents_id += [ unicode(e['cid']) for e in sub_cate if e['is_parent'] ]
                self._save_and_output(sub_cate)
        
        cate = self.tabao.taobao_itemcats_get(fields='cid,parent_cid,name,is_parent', parent_cid=pid)
        pass
        
    def _get_cate_list(self, cid):
        cate = self.taobao.taobao_itemcats_get(fields='cid,parent_cid,name,is_parent', parent_cid=cid)
        return cate["itemcats_get_response"]['item_cats']['item_cat']
        
    
    def _save_and_output(self, cate_list):
        for c in cate_list:
            #self.fd.write(unicode(c['cid']))
            #self.fd.write("-->")
            #self.fd.write(c['name'])
            self.fd.write(u"%s-->%s --> %s\n" % (c['cid'], c['name'], c['parent_cid']))
            print (c['cid'], c['name'])
        
        
    def open_file(self, path):
        import codecs
        self.fd = codecs.open(path, "w", "utf-8")
        #self.fd = open(path, 'w')
        
    def close(self):
        self.fd.close()

class Site(object):
    def __init__(self, ):
        self.root = os.environ.get("WORKSPACE", ".")
        self.hostname = "fmei.sinaapp.com"
        
    def real_path(self, path):
        return os.path.join(self.root, path)
    
if __name__ == "__main__":
    init_path()
    FORMAT = "%(asctime)s %(name)s T[%(thread)d]P[%(process)d] %(levelname)8s %(message)s"
    logging.basicConfig(level=logging.DEBUG, format=FORMAT, stream=sys.stdout)
    from http_client import HTTPClient
    
    if len(sys.argv) == 2:
        pid = sys.argv[1]
        
        task = GetTaokDetail()
        http = HTTPClient()
        if os.environ.get("http_proxy", ""):
            http.set_proxy({'http': os.environ.get("http_proxy", "")})
        task.open_file("cate_output.txt")
        data = task.generate_cate_tree(Site(), http, pid)
        task.close()
    else:
        print "python pgenerate_cate_tree <pid>"

    print "done"
    
    