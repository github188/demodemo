

class Player(object):
    def __init__(self, win):
        self.win = win
    
    def open_file(self, name):
        hwnd = self.win.GetHandle()
        print "open file on hwnd:%s" % hwnd