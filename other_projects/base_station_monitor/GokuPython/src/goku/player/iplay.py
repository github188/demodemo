
from ctypes import *

class Player(object):
    def __init__(self, win):
        self.win = win
        self.winDll = WinDLL("iPlay")
    
    def open_file(self, name):
        hwnd = self.win.GetHandle()
        nPort = c_long(4)
        sFile = c_char_p("test04.h264")
        d = self.winDll.PLAY_OpenFile(nPort, sFile)
        print "open file on hwnd:%s" % type(hwnd)
                
        b = self.winDll.PLAY_Play(nPort, hwnd)     
        
        print "open file on hwnd:%s" % hwnd