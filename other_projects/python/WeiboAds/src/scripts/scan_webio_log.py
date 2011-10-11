# -*- coding: utf-8 -*-
"""
把围脖日志扫描到Solr数据库，进行建立索引。
"""
import os, urllib, sys, logging, re
import zipfile
try:
    import json
except:
    import simplejson as json

        
class WeiboAds(object):
    def __init__(self, path, solr):
        self.path = path
        self.solr = solr
        self.archive_dir = path        
        
    def archive_data_file(self, data_type, date, file):
        dst_path = os.path.join(self.archive_dir, data_type, date[:6], date[:6])
        import shutil
        if not os.path.isdir(dst_path):
            os.makedirs(dst_path, )
        
        dst = os.path.join(dst_path, os.path.basename(file))
        if os.path.isfile(dst):
            os.unlink(dst)
        
        logging.info("%s --> %s" % (file, dst))
        shutil.move(file, dst)

    def scan_import_directory(self, work_dir=None):
        """把工作目录下面:upload下面的所有文件，通过json请求发送到solr后，在把文件备份
        archive目录，按日期备份。
        
        upload:
            -<数据类型>_<日期20110811>[_其他].zip
        archive:
            -201108
                - archive file.zip
                
        数据类型：
        summary:
        <source:user_id>,<fans:number>,<friends:number>,<weibo:number>
        """
        work_dir = work_dir or self.path
        input_dir = os.path.join(work_dir, 'upload')
        self.archive_dir = os.path.join(work_dir, 'archive')
        
        logging.info("Start to scan input dir:%s" % input_dir)
        pattern = r"(\w+)_(\d{8})(.*?)\.zip"
        for name in os.listdir(input_dir):
            if name.startswith("."):continue
            fn = re.match(pattern, name, re.I)
            if not fn:
                logging.warn("invalid file:%s" % name)
                continue
            data_type, date = fn.group(1), fn.group(2)
            try:
                self.import_data_to_solr(data_type, date, 
                                        os.path.join(input_dir, name))
                #self.archive_data_file(data_type, date, 
                #                        os.path.join(input_dir, name))
            except Exception, e:
                logging.exception("eroror:%s" % e)        
        self.solr.commit()
        
                
    def import_data_to_solr(self, data_type, date, file):
        """
        统一的文件格式：
        
        key,f1:00,f2:00
        """
        zip = zipfile.ZipFile(file, "r")
        zip.testzip()        
        docs = []
        for name in zip.namelist():
            data = zip.read(name)
            for line in data.split("\n"):
                line = line.strip()
                if not line:continue
                if line[0] == '#':continue
                doc = {'cate': data_type, 'date': date}
                fields = line.split(",")
                doc['id'] = fields[0]
                for f in fields[1:]:
                    k, v = f.split(":", 1)
                    doc[k.strip()] = v.strip()
                docs.append(doc)
                if len(docs) > 100:
                    self.solr.add_documents(docs)
                    docs = []
        self.solr.add_documents(docs)

class SolrClient(object):
    def __init__(self, url):
        self.url = "http://127.0.0.1:8983/solr/core0/update/json"

    def add_documents(self, docs):        
        items = []
        for doc in docs:
            items.append('"add":' + json.dumps({'doc': doc}))
        post_data = "{%s}" % ",".join(items)
        logging.info("post_data:" + post_data)
        #data = urllib.urlopen(self.url, post_data, proxies={}).read()
        data = self._do_post_command(post_data)
        logging.info(data)
    
    def commit(self):
        self._do_command(commit={})
            
    def _do_command(self, **kw): 
        post_data = json.dumps(kw)

        logging.debug("do command:%s" % post_data)       
        
        data = self._do_post_command(post_data)
        #data = urllib.urlopen(self.url, post_data, proxies={}).read()
        logging.info(data)
        
    def _do_post_command(self, data):
        import httplib, urlparse
        url = urlparse.urlparse(self.url)
        h = httplib.HTTP(url.hostname, url.port)
        h.putrequest('POST', url.path)
        h.putheader('Content-type', 'application/json')
        h.putheader('Content-length', str(len(data)))
        h.endheaders()
        h.send(data)
        errcode, errmsg, headers = h.getreply()
        return h.file.read()
        

def main(argv):
    FORMAT = "%(asctime)s %(name)s T[%(thread)d]P[%(process)d] %(levelname)8s %(message)s"
    logging.basicConfig(level=logging.DEBUG, format=FORMAT, stream=sys.stdout)    
    
    solr = SolrClient(argv[2])
    ads = WeiboAds(argv[1], solr)
    
    ads.scan_import_directory()
        
    logging.debug("done....")
    

if '__main__' == __name__:    
    main(sys.argv)
    