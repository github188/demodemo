
from images import ArtProvider
from components.treeabstract import ToolBarTree
from components.action import ToolActionItem, MenuActionItem
import os, logging
import wx
from goku.models import *

class TreeImageList(wx.ImageList):

    def __init__(self):
        size = (16, 16)
        wx.ImageList.__init__(self, *size)
        self.norm_location_icon = self.Add(ArtProvider.CreateBitmap(ArtProvider.FOLDER_CLOSE))
        self.exp_location_icon = self.Add(ArtProvider.CreateBitmap(ArtProvider.FOLDER_OPEN))
        self.norm_bts_icon = self.Add(ArtProvider.CreateBitmap(ArtProvider.EXECPLAN))
        
    def _get_image(self, name, size):
        return wx.ArtProvider_GetBitmap(name, wx.ART_OTHER, size)

_AddLocation = ('Add Location', 'AddLocation', ArtProvider.SUBJECT_ADD)
_AddBTS = ('Add BTS', 'AddBTS', ArtProvider.INSTANCE_ADD)
_Delete = ('Delete', 'Delete', ArtProvider.DELETE)

#| wx.TR_HIDE_ROOT

class NavigationBar(ToolBarTree):
    def __init__(self, parent, editor_panel, style=wx.TR_DEFAULT_STYLE | wx.TR_MULTIPLE):
        self.mainframe=parent
        ToolBarTree.__init__(self, parent, editor_panel, style)        
        self._multi_choice_disable_actions = ['Add ManagedObject', 'Delete', 'Export RNWPlan']
        self._start_up_disable_actions = ['Add ManagedObject', 'Delete', 'Export RNWPlan']
        self._need_disable_actions()
        #self.root_template = TextTemplate("root")
        self.pupulate_tree()
        self.loadTreeData(LocationNode("001", "Root"))
        self.loaded = False
        #self.Bind(wx.EVT_SHOW, self.LoadBTSTree)
        self.LoadBTSTree(3)
        
    def init_toolbar_actions(self):
        _ToolData = [ ToolActionItem(*args) for args in
                      [_AddLocation, _AddBTS, _Delete ]]
        return _ToolData
    
    def init_tree_popup_actions(self):
        _ProjectPopMenu = []
        return _ProjectPopMenu
    
    def pupulate_tree(self, templates_dir=None):
        self._images = TreeImageList()
        self.tree.SetImageList(self._images)
        
    def LoadBTSTree(self, e):
        logging.info("LoadBTSTree")
        if not self.loaded: 
            self.loaded = True
            from goku.models import server
            root = server.load_bts_list()
            self.loadTreeData(root)        
        
    def cur_select_node(self):
        nodes = self.tree.GetSelections()
        if len(nodes) == 1 and nodes[0].IsOk():
            return nodes[0]
        return None
    
    def _select_node(self, node):
        data = self.tree.GetItemPyData(node)
        if data is None: return
        #self._show_correct_editor(data, node)    
    
    def OnAddLocation(self, e):
        parent_node = self.cur_select_node() or self._root         
        data = self.tree.GetItemPyData(parent_node)
        
        if data.is_leaf:
            wx.MessageBox("Can't add location", 'Error', wx.ICON_ERROR)
        else:
            dlg = wx.TextEntryDialog(None, 'Name', 'New Text Location')
            if dlg.ShowModal() == wx.ID_OK:
                name = str(dlg.GetValue())
                node_data = LocationNode("", name)
                data.add_children(node_data)
                
                node = self._create_tree_node(parent_node, node_data, )
                self._select_node(node)
            dlg.Destroy()
            wx.YieldIfNeeded()
    
    def OnAddBTS(self, e):
        parent_node = self.cur_select_node() or self._root         
        data = self.tree.GetItemPyData(parent_node)
        
        if data.is_leaf:
            wx.MessageBox("Can't add location", 'Error', wx.ICON_ERROR)        
        else:
            dlg = wx.TextEntryDialog(None, 'Name', 'New BTS')
            if dlg.ShowModal() == wx.ID_OK:
                name = str(dlg.GetValue())
                node_data = BTSNode("", name)
                data.add_children(node_data)
                
                node = self._create_tree_node(parent_node, node_data, )
                self._select_node(node)
            dlg.Destroy()
            wx.YieldIfNeeded()
        
    def OnDelete(self, e):
        pass
    
    def loadTreeData(self, data):
        self.root = data
        self.tree.DeleteAllItems()
        self._root = self.tree.AddRoot(data.view_label)
        self.tree.SetPyData(self._root, data)
        self.tree.SetItemImage(self._root, self._images.norm_location_icon, 
                               wx.TreeItemIcon_Normal)
        self.tree.SetItemImage(self._root, self._images.exp_location_icon, 
                               wx.TreeItemIcon_Expanded)
        
        self._import_tree_data(self._root, data.children)
        
    def _import_tree_data(self, parent, child_list):
        for node in child_list:
            sub_node = self._create_tree_node(parent, node,)
            if isinstance(node, (LocationNode, )):
                self._import_tree_data(sub_node, node.children)
        
    def _create_tree_node(self, parent_node, pydata,
                          norm_icon_index=None, exp_icon_index=None):        
        if not norm_icon_index:
            if isinstance(pydata, (LocationNode, )):
                norm_icon_index = self._images.norm_location_icon
            else:
                norm_icon_index = self._images.norm_bts_icon
        if not exp_icon_index and isinstance(pydata, (LocationNode, )):
            norm_icon_index = self._images.exp_location_icon
        
        node = self.tree.AppendItem(parent_node, pydata.view_label)
        self.tree.SetPyData(node, pydata)
        if norm_icon_index is not None:
            self.tree.SetItemImage(node, norm_icon_index, wx.TreeItemIcon_Normal)
        if exp_icon_index is not None:
            self.tree.SetItemImage(node, exp_icon_index, wx.TreeItemIcon_Expanded)
            
        self.tree.SetPyData(node, pydata)
        
        self.tree.Expand(parent_node)
        return node
    
