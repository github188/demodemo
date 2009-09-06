

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
	
