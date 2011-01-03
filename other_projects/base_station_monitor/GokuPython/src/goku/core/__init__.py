from settings import Settings


SETTINGS = Settings()

SETTINGS.set("center_server", "127.0.0.1:8080", autosave=False, override=False)
SETTINGS.set("center_server2", "127.0.0.1:8080", autosave=False, override=False)

SETTINGS.set("mainframe position", (100, 50), autosave=False, override=False)
SETTINGS.set("mainframe size", (800, 600), autosave=False, override=False)
SETTINGS.set("lang", "cn", autosave=False, override=False)

SETTINGS.save()