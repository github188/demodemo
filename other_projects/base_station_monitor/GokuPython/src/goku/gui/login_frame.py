
import wx
from dailogs import ConfigDialog

class LoginFrame(wx.Frame):
    def __init__(self, parent, ID, title):
        wx.Frame.__init__(self, parent, ID, title, size=(350, 200))
        self.app = None
        self.inputs = []
        self.CreateSizer()
        
    def CreateSizer(self):
        sizer = wx.BoxSizer(wx.VERTICAL)
        sizer.Add(wx.StaticText(self, -1, self.title_msg()), 0, wx.ALIGN_CENTER | wx.ALL, 5)
        
        for eachLabel, eachStyle, eachHandler in self.dataEntries():
            self.CreateEntry(sizer, eachLabel, eachStyle, eachHandler)
            
        sizer.Add(wx.StaticLine(self, -1, size=(20, -1), style=wx.LI_HORIZONTAL), 
                            0, wx.GROW | wx.ALIGN_CENTER_VERTICAL | wx.RIGHT | wx.TOP, 5)
        
        btnsizer = wx.BoxSizer(wx.HORIZONTAL)
        #btnsizer = wx.StdDialogButtonSizer()
        for eachId, eachLabel, handler in self.dataButtons():
            self.CreateButton(btnsizer, eachId, eachLabel, handler)
            
        #btnsizer.Realize()
        sizer.Add(btnsizer, 0, wx.ALIGN_CENTER , 5)
        
        self.SetSizer(sizer)
        #sizer.Fit(self)
            
    def CreateEntry(self, sizer, label, style, handler):
        box = wx.BoxSizer(wx.HORIZONTAL)
        box.Add(wx.StaticText(self, -1, label), 0, wx.ALIGN_CENTER | wx.ALL, 5)
        text = wx.TextCtrl(self, -1, "", size = (80, -1), style = style)
        if handler:
            text.Bind(wx.EVT_TEXT, handler)
        self.inputs.append(text)
        box.Add(text, 1, wx.ALIGN_CENTER | wx.ALL, 5)
        sizer.Add(box, 0, wx.GROW | wx.ALIGN_CENTER_VERTICAL | wx.ALL, 5)
        
    def CreateButton(self, btnsizer, id, label, handler):
        button = wx.Button(self, id, label)
        if handler:
            button.Bind(wx.EVT_BUTTON, handler)
        btnsizer.Add(button, 0, wx.ALIGN_CENTER, 10)
        
        
    def title_msg(self):
        return _("Please Input name and password")    
         
    def dataEntries(self):
        return ((_('UserName'), 0, None),(_('Password'), wx.TE_PASSWORD, None))
                                    
    def dataButtons(self):
        return ((-1, _('Login'), self.OnLogin), 
                (wx.ID_CANCEL, _('Cancel'), self.OnExitLogin), 
                (-1, _('Config...'), self.OnShowConfig))
        
    def OnExitLogin(self, e):
        self.Close()
        self.Destroy()        
    
    def OnLogin(self, e):
        username = self.inputs[0].GetValue()
        password = self.inputs[1].GetValue()
        from goku.models import server
        
        if not (username and password):
            wx.MessageBox(_('need username and password'), _('Error'), wx.ICON_ERROR)
            return
        else:
            code = server.login(username, password)
        
        if code == '0':
            self.show_main_frame()
            #self.Close()
        elif code == '1':
             wx.MessageBox(_('account not exist'), _('Error'), wx.ICON_ERROR)
        elif code == '2':
            wx.MessageBox(_('password error'), _('Error'), wx.ICON_ERROR)
        elif code == '-2':
            wx.MessageBox(_('need username and password'), _('Error'), wx.ICON_ERROR)
            
    def OnShowConfig(self, e):
        config = ConfigDialog(self,  -1, _("Config Server"),
                              size = (350, 170)
                              )
        config.ShowModal()
        config.Destroy()
        
    def show_main_frame(self,):        
        self.login_ok = True
        self.Close()
        self.Destroy()
        
        frame = self.app.MainFrame(None, -1, _("Application Title"))
        frame.Show(True)
        self.app.SetTopWindow(frame)
        frame.CenterOnScreen()
                
    def InitValue(self):
        self.login_ok = False
        
                