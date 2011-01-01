'''
Created on 2011-1-1

@author: deon
'''

#from wxPython.wx import *
import wx
import wx.aui
import wx.lib.scrolledpanel as scroll
try:
    from wx.lib.agw import flatnotebook as fnb
except ImportError:
    from wx.lib import flatnotebook as fnb

ID_ABOUT = 101
ID_EXIT  = 102

class MainFrame(wx.Frame):
    def __init__(self, parent, ID, title):
        wx.Frame.__init__(self, parent, ID, title, size=(800, 600))
        self.CreateStatusBar()
        self.SetStatusText("This is the statusbar")

        menu = wx.Menu()
        menu.Append(ID_ABOUT, "&About",
                    "More information about this program")
        menu.AppendSeparator()
        menu.Append(ID_EXIT, "E&xit", "Terminate the program")

        menuBar = wx.MenuBar()
        menuBar.Append(menu, "&File");

        self._perspectives = []
        self.SetMenuBar(menuBar)
        self._init_layout()
    
    def _init_layout(self):
        self._mgr = wx.aui.AuiManager()
        self._mgr.SetManagedWindow(self)
        
        self._mgr.AddPane(self._createCenterPane(), wx.aui.AuiPaneInfo().Name("Editor").
                          CenterPane())
        
        self._mgr.AddPane(self._createNavigationTree(), wx.aui.AuiPaneInfo().Name("NavigationBar").
                           Caption("Navigation Bar").Left().Layer(0).Position(0).
                           CloseButton(True).MaximizeButton(True))
        
        all_panes = self._mgr.GetAllPanes()
        for ii in xrange(len(all_panes)):
            if not all_panes[ii].IsToolbar():
                all_panes[ii].Hide()
        self._mgr.GetPane("NavigationBar").Show().Left().Layer(0).Row(0).Position(0)
        self._mgr.GetPane("Editor").Show()
        perspective_all = self._mgr.SavePerspective()
        
        self._perspectives.append(perspective_all)

        self._mgr.Update()        
    
    def _createCenterPane(self):
        self.notebook = NoteBook(self, None)
        self._editor_panel = EditorPanel(self.notebook)
        self.notebook.AddPage(self._editor_panel, "Edit")
        sizer = wx.BoxSizer()
        sizer.Add(self._editor_panel, 1, wx.EXPAND)
        return self.notebook
    
    def _createNavigationTree(self):
        from navigation import NavigationBar
        self.navigation_tree = NavigationBar(self, self._editor_panel)
        return self.navigation_tree    
        
class EditorPanel(scroll.ScrolledPanel):

    def __init__(self, parent):
        scroll.ScrolledPanel.__init__(self, parent, style=wx.SUNKEN_BORDER)
        self.sizer = wx.BoxSizer(wx.VERTICAL)
        self.SetSizer(self.sizer)
        self.editor = None
    
    def set_editor(self, editor):
        if self.editor:
            self.editor.close()
            self.sizer.Clear()
        editor.Show(True)
        self.sizer.Add(editor, 1, wx.ALL | wx.EXPAND)
        self.Layout()
        self.editor = editor
        self.SetAutoLayout(1)
        self.SetupScrolling(True,True)

    def get_editor(self):
        return self.editor
    
    def ScrollChildIntoView(self,child):
        pass
    
    def save(self, message=None):
        if hasattr(self.editor, 'save'):
            self.editor.save()        

class NoteBook(fnb.FlatNotebook):

    def __init__(self, parent, app, size=(200, 250), style=None):
        self._parent = parent
        style = style and style or fnb.FNB_NODRAG | fnb.FNB_HIDE_ON_SINGLE_TAB | fnb.FNB_VC8
        fnb.FlatNotebook.__init__(self, parent, style=style, size=size)
        self.Bind(fnb.EVT_FLATNOTEBOOK_PAGE_CHANGING, self.OnPageChanging)
        self.Bind(fnb.EVT_FLATNOTEBOOK_PAGE_CLOSING, self.OnPageClosing)

    def OnPageChanging(self, event):
        if not self.GetPageCount():
            return

    def OnPageClosing(self, event):
        pass
        