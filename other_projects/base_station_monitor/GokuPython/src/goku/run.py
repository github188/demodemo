'''
Created on 2011-1-1

@author: deon
'''

import wx, os
from core import SETTINGS
from goku.gui import MainFrame
import logging


class GoKuApp(wx.App):
    def OnInit(self):
        frame = MainFrame(None, -1, "Hello from wxPython")
        frame.Show(True)
        self.SetTopWindow(frame)
        frame.CenterOnScreen()
        return True

def main():
    data = SETTINGS['center_server']
    #print "center_server:%s" % str(data)
    _init_logging()
    
    app = GoKuApp(0)
    app.MainLoop()
    
def _init_logging():    
    log_path = os.path.join(SETTINGS['APP_ROOT'], "app.log")    
    logging.basicConfig(level = logging.DEBUG,
                        format = '%(asctime)s %(levelname)s %(message)s',
                        filename = log_path,
                        filemode = 'w')
    logging.info("starting Goku client....")
    
if "__main__" == __name__:
    main()

