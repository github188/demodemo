
try:
    import hashlib
except:
    import md5 as hashlib

def md5(str): return hashlib.md5(str).hexdigest()

class Schema(object):
    def __init__(self, data, parent, path):
        self.path = path
        self.parent = parent
        self.data = data
        #self.longname = None
        self.index_field = [('path', 'NO'),
                            #('uuid', 'UN_TOKENIZED'),
                            #('parent', 'UN_TOKENIZED'),
                            ('name', 'UN_TOKENIZED'),
                            ('context', 'TOKENIZED'),
                            ]
        
        self.name = data.name
        self.context = None
        self.uuid = None
        
    def create_index_field(self):
        self.uuid = md5("%s.%s" % (self.parent, self.name))
        
    def index_data(self):
        fields = []
        self.create_index_field()
        for name, mode in self.index_field:
            v = getattr(self, name)
            if v: fields.append((name, v, mode))
        return fields
    
    def index_keyword(self, keywords):
        result = []
        for kw in keywords:            
            args = list(kw.args)
            args.insert(0, kw.name)
            result.append("#".join(args))
            if hasattr(kw, "keywords"):
                result.extend(self.index_keyword(kw.keywords))
                result.append("--end %s--" % kw.name)
        return result
        
class TestSuite(Schema):
    def __init__(self, suite, *args):
        Schema.__init__(self, suite, *args)
        self.index_field.append(("tags", "TOKENIZED"))
        self.force_tags = suite.force_tags or []

    def create_index_field(self):
        Schema.create_index_field(self)
        #if self.force_tags:
        self.tags = ";".join(self.force_tags)
        self.tags = self.tags.upper()
        
class TestCase(Schema):
    def __init__(self, test, *args):
        Schema.__init__(self, test, *args)
        self.index_field.append(("tags", "TOKENIZED"))
        self.test_tags = test.tags or []
        
    def create_index_field(self):
        Schema.create_index_field(self)
        
        self.tags = ";".join(self.test_tags)
        self.tags = self.tags.upper()
        
        keywords = self.index_keyword(self.data.keywords)
        self.context = "$".join(keywords)

class Keyword(Schema):
    def __init__(self, kw, *args):
        Schema.__init__(self, kw, *args)
        
    def create_index_field(self):
        Schema.create_index_field(self)
        
        keywords = self.index_keyword(self.data.keywords)
        self.context = "$".join(keywords)
    