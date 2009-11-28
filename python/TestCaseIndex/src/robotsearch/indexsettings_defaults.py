import os

def join_path(*args): return os.path.join(*args)

LOGGING_INI = os.path.normpath(os.path.join(os.path.dirname(__file__), 'logging.ini'))
DATA_ROOT = os.path.normpath(os.path.join(os.path.dirname(__file__), '..', '..', 'data'))

RUNNING_FLAG = join_path(DATA_ROOT, ".index_running")
CLOSING_FLAG = join_path(DATA_ROOT, ".index_closing")

TESTCASE_ROOT = r"C:\USERS\_work\workspace_pydev\TestCaseIndex\data\TRM"
INDEX_PATH = join_path(DATA_ROOT, "index_data")

LAST_UPDATED = join_path(DATA_ROOT, ".last_update_svn")

POLLING_INTERVAL = 60

AUTO_UPDATE = False