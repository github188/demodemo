'''
Created on 2010-3-9

@author: wei.jin@nsn.com
'''
import wx
from action import *
class ToolBarTree(wx.Panel):
    def __init__(self, parent, editor_panel, style=wx.TR_DEFAULT_STYLE | wx.TR_HIDE_ROOT | wx.TR_MULTIPLE):
        wx.Panel.__init__(self, parent, wx.ID_ANY,size=wx.Size(250, 250))
        self._root = None
        self._parent=parent
        self._editor = None
        self.tree_style=style
        self.editor_panel=editor_panel
        self.sizer=sizer=wx.BoxSizer(wx.VERTICAL)
        self.SetSizer(sizer)
        self.actions={}
        self._popup_actions_isenable = {}
        self.tree_actions=self.init_tree_popup_actions()
        self.toolbar_actions=self.init_toolbar_actions()
        self._register_actions()
        self.init_toolbar()
        self.init_tree()
    
    def init_toolbar(self):
        self.toolbar_panel = toolbar_panel = PanelToolBar(self,self.toolbar_actions)
        self.pupulate_toolbar()
        self.sizer.Add(toolbar_panel,0,wx.EXPAND)
    
    def init_tree(self):
        self.tree=tree=wx.TreeCtrl(self,-1,style=self.tree_style)
        self.Bind(wx.EVT_CONTEXT_MENU, self.OnContextMenu)
        self.Bind(wx.EVT_LEFT_DOWN, self.OnLeftDown)
        self.Bind(wx.EVT_TREE_SEL_CHANGED, self.OnSelChanged)
        if self._root:
            tree.DeleteAllItems()
        self._root = tree.AddRoot('xxxxx')
        self.pupulate_tree()
        self.sizer.Add(tree,1,wx.EXPAND)
    
    def OnContextMenu(self,evt):
        self._need_disable_actions()
        menu = Menu(self, self.tree_actions)
        for id in self._popup_actions_isenable.keys():
            m_item = menu.FindItemById(id)
            m_item.Enable(self._popup_actions_isenable[id])
        self.PopupMenu(menu)
        menu.Destroy()
        
    def _need_disable_actions(self):
        pass
        
    def OnLeftDown(self,event):
        pass
    
    def OnSelChanged(self,event):
        pass
    
    def init_toolbar_actions(self):
        return []
    
    def init_tree_popup_actions(self):
        return []
    
    def pupulate_toolbar(self):
        pass
    
    def pupulate_tree(self):
        pass
    
    def _register_actions(self):
        for action in self.tree_actions + self.toolbar_actions:
            if action.is_separator: continue
            if not self.actions.has_key(action.name):
                self.actions[action.name]=[action]
            else:
                self.actions[action.name].append(action)
                
    def EnableAllActions(self):    
        for key in self.actions.keys():
            for item in self.actions[key]:
                if isinstance(item, PopMenuActionItem):
                    self._popup_actions_isenable[item.id]=True
                elif isinstance(item, ToolActionItem):
                    self.toolbar_panel.EnableTool(item.id,True)

    def SetActionsDisable(self, action_names):
        self.EnableAllActions()
        for disitem in action_names:
            if self.actions.has_key(disitem):
                items=self.actions[disitem]
                for item in items:
                    if isinstance(item, PopMenuActionItem):
                        self._popup_actions_isenable[item.id]=False                        
                    elif isinstance(item, ToolActionItem):
                        self.toolbar_panel.EnableTool(item.id,False)
