# -*- coding: utf-8 -*-

import sys
import re
from time import localtime, strftime
import logging
import json
import os    
import sqlite3
import time

def init_path():
    cur_dir = os.path.dirname(os.path.abspath(__file__))
    lib_path = os.path.join(cur_dir, 'libs')
    sys.path.insert(0, lib_path)
        
class GetTaokDetail(object):
    def __init__(self, http_client):
        from taobao import Taobao
        from oss.oss_api import *

        self.logger = logging.getLogger("taoke")
        self.taobao = Taobao('12570801', 'fbab4f2ded890ae889e876ae0eee90b9', http=http_client)
        
    def generate_cate_tree(self, site, http, pid):
    
        parents_id = [pid, ]
        while len(parents_id) > 0:
            tmp_list = parents_id
            parents_id = []
            for cid in tmp_list:
                sub_cate = self._get_cate_list(cid)
                parents_id += [ unicode(e['cid']) for e in sub_cate if e['is_parent'] ]                
                self._save_and_output(sub_cate)
                import time
                time.sleep(3) # taobao call limit.
        pass
        
    def _get_cate_list(self, cid):
        cate = self.taobao.taobao_itemcats_get(fields='cid,parent_cid,name,is_parent', parent_cid=cid)
        return cate["itemcats_get_response"]['item_cats']['item_cat']

    def _get_cate_props(self, cid):
        cate = self.taobao.taobao_itemprops_get(fields='pid,name,must,multi,prop_values', cid=cid)
        return cate["itemprops_get_response"]['item_props']['item_prop']
        
    def _save_props(self, cid, props):
        cid = unicode(cid)
        path = u"props/%s/%s.data" % (cid[-1], cid)
        
        values = {}
        for item in props:
            key = item['pid']            
            values[u'%s_name' % key] = item['name']
            if 'prop_values' not in item: continue
            for v in item['prop_values']['prop_value']:
                vk = u'%s_%s' % (key, v['vid'])
                values[vk] = v['name']
        
        import codecs
        if not os.path.isdir(os.path.dirname(path)):
            os.makedirs(os.path.dirname(path))    
        
        logging.info(u"save props:%s" % path)
        fd = codecs.open(path, "w", "utf-8")
        fd.write(json.dumps(values, encoding='utf-8', ensure_ascii=False))
        fd.close()

    
    def _save_and_output(self, cate_list):
        for c in cate_list:
            #self.fd.write(unicode(c['cid']))
            #self.fd.write("-->")
            #self.fd.write(c['name'])
            self.fd.write(u"%s-->%s --> %s\n" % (c['cid'], c['name'], c['parent_cid']))
            print (c['cid'], c['name'])
            if not c['is_parent']:
                try:
                    cid = unicode(c['cid'])
                    props = self._get_cate_props(cid)
                    self._save_props(cid, props)
                except Exception, e:
                    logging.error("get cate props error:%s" % e)
        
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
        
        http = HTTPClient()
        task = GetTaokDetail(http)
        if os.environ.get("http_proxy", ""):
            http.set_proxy({'http': os.environ.get("http_proxy", "")})
        task.open_file(u"%s_cate_output.txt" % pid)
        data = task.generate_cate_tree(Site(), http, pid)
        task.close()
    else:
        print "python pgenerate_cate_tree <pid>"

    print "done"
    
    