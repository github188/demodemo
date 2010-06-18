
from simpleviews import SimpleViews, SimpleUrl
from logging import getLogger
import hashlib

def user_post_syncdb(sender, app, created_models, verbosity, interactive, **kvargs):
    logger = getLogger('sql.post_syncdb')
    sender = sender.__name__.replace('.models', '')
    app = app.__name__.replace('.models', '')
    try:
        mod = __import__(app, {}, {}, ['initialize'])
        if hasattr(mod, 'initialize') and callable(getattr(mod, 'initialize')):
            logger.debug('initializing %s', app)
            getattr(mod, 'initialize')()
        else:
            logger.debug('skipping %s', app)
    except:
        logger.error(trackable('failed to initialize %s' % app))

import traceback        
def trackable(msg):
    return '%s\n%s' % (type(msg) == type('') and msg or str(msg), traceback.format_exc())


def list2table(list, cols=3):
    return [ list[i * cols: (i+1) * cols] for i in range(len(list) / cols) ]

def md5(str): return hashlib.md5(str).hexdigest()
    