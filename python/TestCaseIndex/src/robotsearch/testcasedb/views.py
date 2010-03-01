# Create your views here.
from django.shortcuts import render_to_response
import re
import time
from settings import CONFIG

class Benchmark(object):
    def __init__(self, logger):
        self.logger = logger
        self.marks = []
        
    def start_mark(self, message=None):
        self.marks.append((message, time.time()))
        self.logger.debug("start %s ..." % message)
        
    def stop_mark(self, ):
        message, st = self.marks.pop()
        et = time.time() - st
        self.logger.debug("since '%s' elapsed '%s'" % (message, et))
        return et

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
    import logging
    logger = logging.getLogger("search")
    bench = Benchmark(logger)
    from lucene import IndexSearcher, StandardAnalyzer, FSDirectory, QueryParser, File, Hit
    import lucene, os
    os.environ['JAVA_HOME'] = '/usr/local/jdk1.6.0_17'
    lucene.initVM(lucene.CLASSPATH)         
    
    directory = FSDirectory.open(File(CONFIG.INDEX_PATH))
    ROBOT_INDEX = IndexSearcher(directory, True)
    ROBOT_ANALYZER = StandardAnalyzer()

    keyword = keyword or r.GET['keyword']
    query = QueryParser("context", ROBOT_ANALYZER)
    query = query.parse('"%s"' % keyword, )
    
    bench.start_mark("search")
    hits = ROBOT_INDEX.search(query)
    count = len(hits)
    result = [ ]
    i = 0
    for hit in hits:
        i += 1
        if i > 100: break
        doc = Hit.cast_(hit).getDocument()
        result.append(SearchResult(doc, i, keyword))
    ROBOT_INDEX.close()
    
    et = bench.stop_mark()

    return render_to_response("robot_search_result.html", {"result":result,
                                                           "count": count,
                                                           "elaspe": et})
        
    


