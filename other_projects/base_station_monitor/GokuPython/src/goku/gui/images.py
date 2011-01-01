'''
Created on 2010-3-9

@author: wei.jin@nsn.com
'''
import os
import wx
import sys

frozen = getattr(sys, 'frozen', '')
if not frozen:
    cur_dir = os.path.dirname(__file__)
elif frozen in ('dll', 'console_exe', 'windows_exe'):
    cur_dir = os.path.join(os.path.dirname(sys.executable), "rnwplan", "ui")

class ArtProvider():
    SUBJECT_ADD='1001'
    DELETE='1002'
    CASE_ADD='1003'
    FILTERS='1004'
    REFRESH='1005'
    EXEPLAN_ADD='1006'
    FOLDER_OPEN='1007'
    FOLDER_CLOSE='1008'
    PROJECT_OPEN='1009'
    PROJECT_CLOSED='1010'
    EXECPLAN='1011'
    ADD_INSTANCE='1012'
    RUN_EXECPLAN='1013'
    DETAIL='1014'
    TO_REPORT='1015'
    CONFIGURE='1016'
    PREVIOUS='1017'
    NEXT='1018'
    FIRST='1019'
    LAST='1020'
    SUITE='1021'
    TESTCASE='1022'
    ATTACHMENT_UPLOAD='1023'
    ATTACHMENT_MAPPING='1024'
    ATTACHMENT_DOWNLOAD='1025'
    INSTANCE_ADD='1026'
    SAVE='1027'
    SAVE_AS='1028'
    CLONE_SECTION='1029'
    MOVE_UP='1030'
    MOVE_DOWN='1031'
    IMPORT_SECTION='1032'
    IMAGES = {SUBJECT_ADD:'subject_add.png',DELETE:'delete.gif',CASE_ADD:'case_add.png',\
              FILTERS:'filter.gif',REFRESH:'refresh.png',EXEPLAN_ADD:'plan_add.png',
              FOLDER_OPEN:'folder_open.gif',FOLDER_CLOSE:'folder_closed.gif',PROJECT_OPEN:'project_opened.gif',\
              PROJECT_CLOSED:'project_closed.gif',EXECPLAN:'exec_plan.png',ADD_INSTANCE:'add_instance.gif'
              ,RUN_EXECPLAN:'run_execplan.gif',DETAIL:'detail.png',TO_REPORT:'to_report.png'\
              ,CONFIGURE:'config.gif',PREVIOUS:'previous.gif',NEXT:'next.gif',FIRST:'first.gif',\
              LAST:'last.gif',SUITE:'suite.gif',TESTCASE:'testcase.gif',INSTANCE_ADD:'instance_add.png',\
              ATTACHMENT_UPLOAD:'attachment.gif',ATTACHMENT_MAPPING:'attachment_mapping.gif',ATTACHMENT_DOWNLOAD:'download.gif',\
              SAVE:'save.png', SAVE_AS:'save_as.png', CLONE_SECTION:'copy.gif',
              MOVE_UP:'move_up.png', MOVE_DOWN:'move_down.png', IMPORT_SECTION:'import_section.png',
              }
    @staticmethod
    def CreateBitmap(artid, size=(16, 16)):
        image_root = os.path.join(cur_dir, 'localimages')
        path = os.path.join(image_root, ArtProvider.IMAGES.get(artid))
        bmp = wx.Image(path, wx.BITMAP_TYPE_ANY).ConvertToBitmap()

        return bmp
    @staticmethod
    def GetBitmapPath(artid):
        image_root = os.path.join(cur_dir, 'localimages')
        path = os.path.join(image_root, ArtProvider.IMAGES.get(artid))
        return path
    @staticmethod
    def CreateBitmapByPath(path, size=(16, 16)):
        bmp = wx.Image(path, wx.BITMAP_TYPE_ANY).ConvertToBitmap()
        return bmp
