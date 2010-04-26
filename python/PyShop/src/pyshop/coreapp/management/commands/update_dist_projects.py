from django.core.management.base import NoArgsCommand

from optparse import make_option
import traceback
from pyshop.settings import DIST_ROOT
from pyshop.coreapp.project_importor import ProjectImportor

class Command(NoArgsCommand):
    help = "update dist project list."
    
    option_list = NoArgsCommand.option_list

    requires_model_validation = False
    
    def handle_noargs(self, **kw):
        print "starting update dist projects..."
        pi = ProjectImportor()
        pi.update_dist(DIST_ROOT)
        pi.remove_expire_project()
        print "done!"
    
