import wx.lib.sized_controls as sc
import wx

import logging
    
class BaseDialog(wx.Dialog):
    def __init__(self, parent, id, title,  
                       size = (350, 200), 
                       pos = wx.DefaultPosition,
                       style = wx.DEFAULT_DIALOG_STYLE, useMetal = False):
        self.inputs = []

        pre = wx.PreDialog()
        pre.Create(parent, id, title, pos, size, style)
        self.PostCreate(pre)
        self.CreateSizer()
        
        self.InitValue()
        
        
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
        
    def InitValue(self):pass
        
        
class LoginDialog(BaseDialog):
    
    def title_msg(self):
        return _("Please Input name and password")    
         
    def dataEntries(self):
        return ((_('UserName'), 0, None),(_('Password'), wx.TE_PASSWORD, None))
                                    
    def dataButtons(self):
        return ((-1, _('Login'), self.OnLogin), 
                (wx.ID_CANCEL, _('Cancel'), None), 
                (-1, _('Config...'), self.OnShowConfig))
    
    def OnLogin(self, e):
        username = self.inputs[0].GetValue()
        password = self.inputs[1].GetValue()
        from goku.models import server
        
        if not (username and password):
            wx.MessageBox(_('need username and password'), _('Error'), wx.ICON_ERROR)
            return
        else:
            code = server.login(username, password)
        
        self.login_ok = False
        if code == '0':
            self.login_ok = True
            self.Close()
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
        
    def InitValue(self):
        self.login_ok = False
            
class ConfigDialog(BaseDialog):
    
    def title_msg(self):
        return _("Input Server Address")
    
    def dataEntries(self):
        return ((_('MasterServer'), 0, None), (_('SecondServer'), 0, None))
                                    
    def dataButtons(self):
        return ((-1, _('OK'), self.OnSaveSettings), (wx.ID_CANCEL, _('Cancel'), None))
    
    def InitValue(self):
        from goku.core import SETTINGS
        self.inputs[0].SetValue(SETTINGS["center_server"])
        self.inputs[1].SetValue(SETTINGS["center_server2"])
    
    def OnSaveSettings(self, e):
        from goku.core import SETTINGS
        SETTINGS["center_server"] = self.inputs[0].GetValue()
        SETTINGS["center_server2"] = self.inputs[1].GetValue()
        
        logging.info("config primary_server:%s" % SETTINGS["center_server"])
        logging.info("config secondary_server:%s" % SETTINGS["center_server2"])
        from goku.models import connect
        
        connect(SETTINGS["center_server"], SETTINGS["center_server2"])
                
        self.Close()
    

