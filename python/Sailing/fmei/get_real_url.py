#!/bin/python
# -*- coding: utf-8 -*-

import logging, os, sys

def init_path():
    cur_dir = os.path.dirname(os.path.abspath(__file__))
    lib_path = os.path.join(cur_dir, 'libs')
    sys.path.insert(0, lib_path)

if __name__ == "__main__":
    init_path()
    FORMAT = "%(asctime)s %(name)s T[%(thread)d]P[%(process)d] %(levelname)8s %(message)s"
    logging.basicConfig(level=logging.DEBUG, format=FORMAT, stream=sys.stdout)
    from http_client import HTTPClient
    
        
    http = HTTPClient()
    if os.environ.get("http_proxy", ""):
        http.set_proxy({'http': os.environ.get("http_proxy", "")})
    
    http.get_real_url("http://t.cn/aCAHu3")
    