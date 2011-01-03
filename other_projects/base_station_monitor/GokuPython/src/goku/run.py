'''
Created on 2011-1-1

@author: deon
'''

import wx, os, sys
import logging
import gettext

class GoKuApp(wx.App):
    def OnInit(self):
        #frame = GoKuApp.LoginFrame(None, -1, _("Application Title"))
        from goku.models import server
        server.login("test1", "pass")
        frame = GoKuApp.MainFrame(None, -1, _("Application Title"))
        frame.app = self
        frame.Show(True)
        self.SetTopWindow(frame)
        frame.CenterOnScreen()
        return True

def main():
    frozen = getattr(sys, 'frozen', '')
    if not frozen:
        app_root = os.path.dirname(os.path.dirname(__file__))
    elif frozen in ('dll', 'console_exe', 'windows_exe'):
        app_root = os.path.join(os.path.dirname(sys.executable))
    sys.path.insert(0, app_root)
    
    from goku.core import SETTINGS
    from goku.gui import MainFrame
    from goku.gui.login_frame import LoginFrame
    from goku.models import connect
    GoKuApp.MainFrame = MainFrame
    GoKuApp.LoginFrame = LoginFrame
    
    #data = SETTINGS['center_server']
    SETTINGS.set("APP_ROOT", app_root, False, True)
    _init_logging(SETTINGS)
    _init_i18n(app_root, SETTINGS['lang'])
    
    connect(SETTINGS['center_server'])
    app = GoKuApp(0)
    app.MainLoop()
    
def _init_i18n(app_root, lang):
    #parent_dir= os.path.join(os.path.dirname(__file__), '..')
    locale_path = os.path.join(app_root, 'resource', 'locale')
    logging.info("init i18n locale path:%s, lang:%s" % (locale_path, lang))
    try:
        gettext.install('lang', locale_path, unicode=False)
        gettext.translation('lang', locale_path, 
                            languages = [lang, ]).install(True)
    except Exception, e:
        logging.exception(e)
        globals()['_'] = lambda x: x
    
def _init_logging(settings):    
    log_path = os.path.join(settings['DATA_ROOT'], "app.log")
    if os.getenv("dev_mode", None):
        logging.basicConfig(level = logging.DEBUG,
                            format = '%(asctime)s %(levelname)s %(message)s',
                            stream=sys.stdout)
    else:
        logging.basicConfig(level = logging.DEBUG,
                            format = '%(asctime)s %(levelname)s %(message)s',
                            filename = log_path,
                            filemode = 'w')
    logging.info("starting Goku client....")
    logging.info("starting app root:%s" % settings['APP_ROOT'])
    
if "__main__" == __name__:
    main()

