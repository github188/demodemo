import os
from sailing.common.common import *

def change_status(old_st, new_st):
    
    path, ext = os.path.splitext(old_st)
    new_st = "%s.%s" % (path, new_st)
    move_to(old_st, new_st)
    
    return new_st