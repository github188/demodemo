
from logging import getLogger

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
    except Exception, e:
        logger.exception(e)
        raise