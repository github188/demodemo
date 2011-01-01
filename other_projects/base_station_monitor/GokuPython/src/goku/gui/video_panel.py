
import wx
class BasicVideoPlan(wx.Panel):
    def __init__(self, *args, **kw):
        wx.Panel.__init__(self, *args, **kw)
        
        self.windows = [(VideoWindow(self, -1), 0, wx.EXPAND),
                        (VideoWindow(self, -1), 0, wx.EXPAND),
                        (VideoWindow(self, -1), 0, wx.EXPAND),
                        (VideoWindow(self, -1), 0, wx.EXPAND),
                        ]
        self.SetBackgroundColour('#c56c00')
        self._init_layout()
        
    def _init_layout(self):
        gs = wx.GridSizer(4, 4, 3, 3)
        gs.AddMany(self.windows + 
                   [(wx.Button(self, -1, 'Cls'), 0, wx.EXPAND),
                    (wx.Button(self, -1, 'Bck'), 0, wx.EXPAND),
                    (wx.StaticText(self, -1, ''), 0, wx.EXPAND),
                    ])
        
        self.SetSizer(gs)
        
    def get_video_window(self, index):
        return self.windows[index][0]
    
    
class VideoWindow(wx.Window):
    def __init__(self, *args, **kw):
        wx.Window.__init__(self, *args, **kw)
        self.SetBackgroundColour('#9e4757')
        
