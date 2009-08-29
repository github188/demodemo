

def index(r):
    return "hello, index"

	
def myip(r, json=''):
    if json == '1':
        return {"remote_addr":r.META["REMOTE_ADDR"], }
    else:
        return "My ip address:%s" % (r.META["REMOTE_ADDR"])
	