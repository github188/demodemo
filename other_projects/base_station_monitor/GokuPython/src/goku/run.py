'''
Created on 2011-1-1

@author: deon
'''

import wx
from goku.gui import MainFrame

class GoKuApp(wx.App):
    def OnInit(self):
        frame = MainFrame(None, -1, "Hello from wxPython")
        frame.Show(True)
        self.SetTopWindow(frame)
        return True

def main():
    app = GoKuApp(0)
    app.MainLoop()
    
if "__main__" == __name__:
    main()

