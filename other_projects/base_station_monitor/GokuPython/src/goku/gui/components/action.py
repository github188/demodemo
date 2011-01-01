'''
Created on 2010-3-10

@author: wei.jin@nsn.com
'''
from goku.gui.images import ArtProvider
import os, logging
import wx

def _(x): return x

class _ActionItem(object):

    def __init__(self, name, doc, art=""):
        self.name = name
        self.doc = doc
        self.art = art
        self.id = wx.NewId()
        self.handler_name = 'On%s' % self.name.replace(' ', '')

    def bind(self, parent, component):
        try:
            parent.Bind(self.event, getattr(parent, self.handler_name), source=component)
        except Exception, e:
            logging.warn("Failed to bind action:%s" % str(e))
        
class MenuActionItem(_ActionItem):

    event = wx.EVT_MENU

    def __init__(self, name, doc='', accelerator='', shortcut='', art=''):
        _ActionItem.__init__(self, name, doc, art)
        self.menu_entry = self._get_menu_entry(name, accelerator, shortcut)
        self.is_separator = self.menu_entry == '---'

    def _get_menu_entry(self, name, accelerator, shortcut):
        menu_name = self._get_menu_name(name, accelerator)
        if shortcut:
            menu_name = '%s\t%s' % (menu_name, shortcut)
        return menu_name

    def _get_menu_name(self, name, accelerator):
        if not accelerator:
            return _(name)
        start, end = name.split(accelerator, 1)
        return '%s&%s%s' % (start, accelerator, end)

class PopMenuActionItem(MenuActionItem):
    pass

class ToolActionItem(_ActionItem):

    event = wx.EVT_TOOL

    def __init__(self, name, doc="", art=""):
        _ActionItem.__init__(self, name, doc)
        self.is_separator = self.name == '---'
        self.art = art

class Menu(wx.Menu):

    def __init__(self, parent, items):
        wx.Menu.__init__(self)
        for item in items:
            if item.is_separator:
                self.AppendSeparator()
            else:
                self._create_menu_item(parent, item)

    def _create_menu_item(self, parent, item):
        menuitem = self.Append(item.id, item.menu_entry, _(item.doc))
        if item.art:
            bmp = ArtProvider.CreateBitmap(item.art)
            menuitem.SetBitmap(bmp)
        item.bind(parent, menuitem)
        
class PanelToolBar(wx.ToolBar):

    def __init__(self, parent,tool_data):
        wx.ToolBar.__init__(self, parent, style=wx.TB_HORIZONTAL)
        self.SetToolBitmapSize((16, 16))
        for item in tool_data:
            if item.is_separator:
                self.AddSeparator()
            else:
                self.create_tool(item, parent)
        self.Realize()

    def create_tool(self, item, parent):
        bmp = ArtProvider.CreateBitmap(item.art)
        bmp_path=ArtProvider.GetBitmapPath(item.art)
        ret= os.path.splitext(bmp_path)
        disbmppath=ret[0]+"_dis.gif"
        disbmp=None
        if os.path.exists(disbmppath):
            disbmp=ArtProvider.CreateBitmapByPath(disbmppath)
#        bmp = wx.ArtProvider.GetBitmap(item.art, wx.ART_TOOLBAR, (16, 16))
        if disbmp:
            tool = self.AddLabelTool(item.id, item.name, bmp,bmpDisabled=disbmp,
                                 shortHelp=_(item.name), longHelp=_(item.doc))
        else:
            tool = self.AddLabelTool(item.id, item.name, bmp,
                                 shortHelp=_(item.name), longHelp=_(item.doc))
        item.bind(parent, tool)
        