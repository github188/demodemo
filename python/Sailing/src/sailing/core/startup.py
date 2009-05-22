
from application import Application
import sys
from sailing.conf import settings

def main():
    if len(sys.argv) != 3 or sys.argv[1] not in ['start', 'stop' ]:
        print 'python -m sailing start|stop <application>'
    else:
        #loading settings
        settings.configure(sys.argv[2])
                
        getattr(Application(), sys.argv[1])()