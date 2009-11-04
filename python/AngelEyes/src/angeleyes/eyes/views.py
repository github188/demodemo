from datetime import datetime
from models import Activity

def IP(r): return r.META["REMOTE_ADDR"]

def activity(r, u='', action='', group=''):
    timestamp = datetime.now().strftime('%Y%m%d%H%M%S')
    act = Activity(believer=u, group=group, timestamp=timestamp,
                   action=action, ipaddress=IP(r),
                   source='web', category='track')
    act.save(True)
    return 'OK'