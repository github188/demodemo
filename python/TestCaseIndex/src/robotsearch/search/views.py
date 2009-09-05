# Create your views here.
from robotsearch.settings import INDEX_ROOT
from django.shortcuts import render_to_response
from PyLucene import QueryParser
import re
from time import time

class SearchResult(object):
    def __init__(self, doc, i, search=""):
    #    self.hit = hit
        self.hits = i
        self.doc = doc
        self.search = search
        
        self.hits = self.hits
        self.path = self.path
        self.name = self.name
        self.context = self.context     
    
    def __getattr__(self, name):            
        value = self.doc.get(name)
        if name == 'context':
            value = self.format(value)
        return value
    
    def format(self, text):
        def x(r):
            try:
                repl = r.group(0)
                if repl.startswith('$'): return "<br/>"
                else:
                    return "<b>%s</b>" % repl
            except Exception, e:
                print "!!!%s!!!" % e
                return "!!!%s!!!" % e
        
        text = re.sub(r"(?i)(?:\$(?!\{)|(%s))" % self.search, x, text)
        return text
        #re.sub(r"\$([\$\w])", x, text)

def search(r, keyword=""):
    
    keyword = keyword or r.GET['keyword']
    
    from PyLucene import IndexSearcher, StandardAnalyzer, FSDirectory
    ROBOT_INDEX = IndexSearcher(FSDirectory.getDirectory(INDEX_ROOT, False))
    ROBOT_ANALYZER = StandardAnalyzer()
    
    query = QueryParser("context", ROBOT_ANALYZER)
    query = query.parse('"%s"' % keyword, )
    
    print "starting search..."
    st = time()
    hits = ROBOT_INDEX.search(query)
    
    print "end search, count %s ." % len(hits)
    
    count = len(hits)
    
    result = [ ]
    for i, doc in hits:
        if i > 100: break
        result.append(SearchResult(doc, i, keyword))
    ROBOT_INDEX.close()
    
    elaspe = time() - st
    
    return render_to_response("robot_search_result.html", {"result":result,
                                                           "count": count,
                                                           "elaspe": elaspe})

import threading
INDEX_LOCK = threading.BoundedSemaphore(1)
