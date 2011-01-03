
import wx
import wx.lib.agw.hypertreelist as HTL

class AlarmPanel(wx.Panel):
    def __init__(self, parent, ):
        wx.Panel.__init__(self, parent, )
        self.create_detail_page()
                
    def create_detail_page(self):
        self.sizer = wx.BoxSizer(wx.VERTICAL)
        
        self.alarmTable = AlarmTable(self)
        self.sizer.Add(self.alarmTable, 0, wx.EXPAND|wx.ALL, 0)
        
        self.SetSizer(self.sizer)
        self.Layout()

class AlarmTable(HTL.HyperTreeList):
    def __init__(self, parent, **kwargs):
        HTL.HyperTreeList.__init__(self, parent, -1, 
                                   style=wx.TR_HIDE_ROOT |
                                   wx.SUNKEN_BORDER | 
                                   wx.TR_HAS_BUTTONS | 
                                   wx.TR_ROW_LINES |
                                   wx.TR_COLUMN_LINES |
                                   wx.TR_HAS_VARIABLE_ROW_HEIGHT)
        
        self.AddColumn(_("Location"))
        self.AddColumn(_("BTSType"))
        self.AddColumn(_("AlarmType"))
        self.AddColumn(_("BTS"))
        self.AddColumn(_("StartTime"))
        self.AddColumn(_("EndTime"))
        self.AddColumn(_("Status"))
        #self.AddColumn("Hex")
        self.SetMainColumn(0)
        self.SetColumnWidth(0, 80)
        self.SetColumnWidth(1, 80)
        self.SetColumnWidth(2, 120)
        self.SetColumnWidth(3, 120)
        self.SetColumnWidth(4, 120)
        self.SetColumnWidth(5, 120)
        self.SetColumnWidth(6, 60)
        
        
