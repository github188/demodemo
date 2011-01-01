'''
Created on 2011-1-1

@author: deon
'''

import wx
from core import SETTINGS
from goku.gui import MainFrame

class GoKuApp(wx.App):
    def OnInit(self):
        frame = MainFrame(None, -1, "Hello from wxPython")
        frame.Show(True)
        self.SetTopWindow(frame)
        return True

def main():
    data = SETTINGS['center_server']
    print "center_server:%s" % str(data)
    
    app = GoKuApp(0)
    app.MainLoop()
    
if "__main__" == __name__:
    main()

