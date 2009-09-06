
import datetime
from google.appengine.api import datastore

def index(r):
    return "hello, index"

	
def default_view(r):
    return ("deonwu_index.html", )

def myspace(r):
    return ("myspace/SeniorMatch.html", )

def featured(r):
	dummy_feature = [('http://s.commercialless.com/de/picsap8/9/979/130009979-452538i.jpg', 'butterfly48'), 
					 ('http://s.commercialless.com/de/picsap8/3/833/130008833-451787i.jpg', 'JennaBenna84'),
					 ('http://s.commercialless.com/de/picsap8/9/989/130009989-452544i.jpg', 'toxicxlipgloss'),
					 ('http://s.commercialless.com/de/picsap8/4/304/15710304-445088i.jpg', 'phor2zero'),
					 ('http://s.commercialless.com/de/picsap8/5/775/130005775-449598i.jpg', 'karaline2000'),
					 ('http://s.commercialless.com/de/picsap8/9/979/130009979-452538i.jpg', 'butterfly48'), 
					 ('http://s.commercialless.com/de/picsap8/3/833/130008833-451787i.jpg', 'JennaBenna84'),
					 ('http://s.commercialless.com/de/picsap8/9/989/130009989-452544i.jpg', 'toxicxlipgloss'),
					 ('http://s.commercialless.com/de/picsap8/4/304/15710304-445088i.jpg', 'phor2zero'),
					 ('http://s.commercialless.com/de/picsap8/5/775/130005775-449598i.jpg', 'karaline2000'),					 
					]
	return dummy_feature
	
    
def import_profile(r):
    
    data = r.POST or r.GET  
    
    if not data.has_key('id'): 
        return {"stauts": 'ERR', 
                "message":"The ID is must required!"}
    
    profile = datastore.Entity('MySpaceProfile')
    for k, v in data.iteritems():
        profile[k] = v
    profile['data_type'] = 'MySpaceProfile'
    profile['created'] = datetime.datetime.now()
    datastore.Put(profile)
    
    return {"stauts": 'OK'}


def list_myspace(r):
    query = datastore.Query('MySpaceProfile')
    query['data_type ='] = 'MySpaceProfile'
    result = query.Get(100)
        
    return ("myspace/my_profile_list.html", {"profiles":result})


