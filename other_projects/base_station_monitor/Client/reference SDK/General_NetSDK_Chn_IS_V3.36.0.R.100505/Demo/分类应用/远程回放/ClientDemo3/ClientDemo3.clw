; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CPlaybackByTime
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "clientdemo3.h"
LastPage=0

ClassCount=6
Class1=CClientDemo3App
Class2=CAboutDlg
Class3=CClientDemo3Dlg
Class4=CExSliderCtrl
Class5=CPlaybackByFile
Class6=CPlaybackByTime

ResourceCount=4
Resource1=IDD_PLAYBACK_RECORDFILE
Resource2=IDD_CLIENTDEMO3_DIALOG
Resource3=IDD_ABOUTBOX
Resource4=IDD_PLAYBACK_BYTIME

[CLS:CClientDemo3App]
Type=0
BaseClass=CWinApp
HeaderFile=ClientDemo3.h
ImplementationFile=ClientDemo3.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=ClientDemo3Dlg.cpp
ImplementationFile=ClientDemo3Dlg.cpp

[CLS:CClientDemo3Dlg]
Type=0
BaseClass=CDialog
HeaderFile=ClientDemo3Dlg.h
ImplementationFile=ClientDemo3Dlg.cpp

[CLS:CExSliderCtrl]
Type=0
BaseClass=CSliderCtrl
HeaderFile=ExSliderCtrl.h
ImplementationFile=ExSliderCtrl.cpp

[CLS:CPlaybackByFile]
Type=0
BaseClass=CDialog
HeaderFile=PlaybackByFile.h
ImplementationFile=PlaybackByFile.cpp
Filter=D
VirtualFilter=dWC

[CLS:CPlaybackByTime]
Type=0
BaseClass=CDialog
HeaderFile=PlaybackByTime.h
ImplementationFile=PlaybackByTime.cpp
Filter=D
VirtualFilter=dWC
LastObject=IDC_DATE_FROM

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_CLIENTDEMO3_DIALOG]
Type=1
Class=CClientDemo3Dlg
ControlCount=26
Control1=IDC_BTN_LOGIN,button,1342242817
Control2=IDC_BTN_LOGOUT,button,1476460544
Control3=IDC_IPADDRESS_DVRIP,SysIPAddress32,1342242816
Control4=IDC_EDIT_PORT,edit,1350639744
Control5=IDC_EDIT_USERNAME,edit,1350631552
Control6=IDC_EDIT_DVRPWD,edit,1350631584
Control7=IDC_STATIC,button,1342177287
Control8=IDC_STATIC,static,1342308354
Control9=IDC_STATIC,static,1342308354
Control10=IDC_STATIC,static,1342308354
Control11=IDC_STATIC,static,1342308354
Control12=IDC_STATIC,button,1342177287
Control13=IDC_SCREEN_PLAYBACK,static,1342177284
Control14=IDC_STATIC,static,1342181383
Control15=IDC_TAB1,SysTabControl32,1342177280
Control16=IDC_STATIC,button,1342177287
Control17=IDC_SLIDER_PLAYPOSITION,msctls_trackbar32,1342242872
Control18=IDC_BUTTON_PLAY,button,1342242816
Control19=IDC_BUTTON_STOP,button,1342242816
Control20=IDC_BUTTON_FAST,button,1342242816
Control21=IDC_BUTTON_SLOW,button,1342242816
Control22=IDC_BUTTON_RESET,button,1476460544
Control23=IDC_LAST_ERROR,static,1342308352
Control24=IDC_STATIC,static,1342308354
Control25=IDC_BUTTON_FRAMETO,button,1342242816
Control26=IDC_BUTTON_NORMALSPEED,button,1342242816

[DLG:IDD_PLAYBACK_RECORDFILE]
Type=1
Class=CPlaybackByFile
ControlCount=23
Control1=IDC_BUTTON_QUERY,button,1342242817
Control2=IDC_BUTTON_PLAYBYFILE,button,1342242816
Control3=IDC_BUTTON_CLEANLIST,button,1342242816
Control4=IDC_COMBO_CHANNELBYFILE,combobox,1344339971
Control5=IDC_COMBO_QUERYTYPE,combobox,1344339971
Control6=IDC_DATE_FROM,SysDateTimePick32,1342242852
Control7=IDC_TIME_FROM,SysDateTimePick32,1342242857
Control8=IDC_DATE_TO,SysDateTimePick32,1342242852
Control9=IDC_TIME_TO,SysDateTimePick32,1342242857
Control10=IDC_STATIC,static,1342308354
Control11=IDC_STATIC,static,1342308354
Control12=IDC_STATIC,static,1342308354
Control13=IDC_STATIC,static,1342308354
Control14=IDC_STATIC,button,1342177287
Control15=IDC_STATIC,button,1342177287
Control16=IDC_LIST_QUERYINFO,SysListView32,1350631437
Control17=IDC_STATIC,button,1342177287
Control18=IDC_BUTTON_DOWNLOADBYFILE,button,1342242816
Control19=IDC_PROGRESS_BYFILE,msctls_progress32,1350565888
Control20=IDC_COMBO_PLAYMODE,combobox,1344339971
Control21=IDC_STATIC,static,1342308354
Control22=IDC_EDIT_CARD,edit,1484849280
Control23=IDC_BUTTON_FARTHESTRECORD,button,1342242816

[DLG:IDD_PLAYBACK_BYTIME]
Type=1
Class=CPlaybackByTime
ControlCount=24
Control1=IDC_STATIC,button,1342177287
Control2=IDC_STATIC,static,1342308354
Control3=IDC_COMBO_CHANNEL,combobox,1344339971
Control4=IDC_STATIC,static,1342308354
Control5=IDC_DATE_FROM,SysDateTimePick32,1342242852
Control6=IDC_TIME_FROM,SysDateTimePick32,1342242857
Control7=IDC_STATIC,static,1342308354
Control8=IDC_DATE_TO,SysDateTimePick32,1342242852
Control9=IDC_TIME_TO,SysDateTimePick32,1342242857
Control10=IDC_BUTTON_PLAY,button,1342242817
Control11=IDC_STATIC,button,1342177287
Control12=IDC_STATIC,static,1342308354
Control13=IDC_DATE_LOAD_FROM,SysDateTimePick32,1342242852
Control14=IDC_TIME_LOAD_FROM,SysDateTimePick32,1342242857
Control15=IDC_STATIC,static,1342308354
Control16=IDC_DATE_LOAD_TO,SysDateTimePick32,1342242852
Control17=IDC_TIME_LOAD_TO,SysDateTimePick32,1342242857
Control18=IDC_BUTTON_DOWNLOAD,button,1342242816
Control19=IDC_PROGRESS_BYTIME,msctls_progress32,1350565888
Control20=IDC_STATIC,static,1342181383
Control21=IDC_STATIC,static,1342308354
Control22=IDC_COMBO_LOADCHANNEL,combobox,1344339971
Control23=IDC_STATIC,static,1342308354
Control24=IDC_COMBO_TIMEPLAYMODE,combobox,1344339971

