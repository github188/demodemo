
from images import ArtProvider
from components.treeabstract import ToolBarTree
from components.action import ToolActionItem, MenuActionItem
import os
import wx

class TreeImageList(wx.ImageList):

    def __init__(self):
        size = (16, 16)
        wx.ImageList.__init__(self, *size)
        self.plan_open = self.Add(ArtProvider.CreateBitmap(ArtProvider.PROJECT_OPEN))
        self.plan_closed = self.Add(ArtProvider.CreateBitmap(ArtProvider.PROJECT_CLOSED))
        self.motype_open = self.Add(ArtProvider.CreateBitmap(ArtProvider.FOLDER_OPEN))
        self.motype_closed = self.Add(ArtProvider.CreateBitmap(ArtProvider.FOLDER_CLOSE))
        self.template = self.Add(ArtProvider.CreateBitmap(ArtProvider.EXECPLAN))
        self.managedobject = self.Add(ArtProvider.CreateBitmap(ArtProvider.SUITE))
        
        self.save = self.Add(ArtProvider.CreateBitmap(ArtProvider.SAVE))
        self.save_as = self.Add(ArtProvider.CreateBitmap(ArtProvider.SAVE_AS))
        self.clone_create = self.Add(ArtProvider.CreateBitmap(ArtProvider.CLONE_SECTION))
        
    def _get_image(self, name, size):
        return wx.ArtProvider_GetBitmap(name, wx.ART_OTHER, size)

_DeleteItemData = ('Delete', 'Delete Section')
_ExportTemplate = ('Export Section', 'Export')
_PreviewTemplate = ('PreView', 'PreView')

_OpenTemplateItemData = ('Open', 'Open Section')
_SaveTemplateItemData = ('Save', 'Save Section')
_SaveAsTemplateItemData = ('Save As', 'Save Section As')
_CloneTemplateItemData = ('Clone Section', 'Clone Section')

_ImportTemplateItemData = ('Import Section', 'Import Section')
_MoveUpTemplateItemData = ('Move Up Section', 'Move Up Section')
_MoveDownTemplateItemData = ('Move Down Section', 'Move Down Section')

class NavigationBar(ToolBarTree):
    def __init__(self, parent, editor_panel, style=wx.TR_DEFAULT_STYLE | wx.TR_HIDE_ROOT | wx.TR_MULTIPLE):
        self.mainframe=parent
        ToolBarTree.__init__(self, parent, editor_panel, style)        
        self._multi_choice_disable_actions = ['Add ManagedObject', 'Delete', 'Export RNWPlan']
        self._start_up_disable_actions = ['Add ManagedObject', 'Delete', 'Export RNWPlan']
        self._need_disable_actions()
        
    def init_toolbar_actions(self):
        _ToolData = [ ToolActionItem(*args) for args in
                        [_DeleteItemData + (ArtProvider.DELETE,),
                         _CloneTemplateItemData + (ArtProvider.CLONE_SECTION,),
                         ("---",),
                         _ExportTemplate + (ArtProvider.TO_REPORT,),
                    ]]
        return _ToolData
    
    def init_tree_popup_actions(self):
        _ProjectPopMenu = []
        return _ProjectPopMenu
    
    def OnNewRNWPlan(self,evt):
        pass
        
    def OnAddManagedObject(self,evt):
        pass
    
    def OnDelete(self,evt):
        pass
    
    def OnExportRNWPlan(self,evt):
        pass
    
    def pupulate_toolbar(self):
        pass
    
    def pupulate_tree(self, templates_dir=None):
        pass
        
    def OnLeftDown(self,event):
        pass
    
    def OnSelChanged(self,event):  
        pass
    
    def OnChangeTemplatesPool(self,event):
        pass
    
