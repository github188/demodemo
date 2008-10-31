
import web
import config

_global_render_ = web.template.render(config.views_config['template'])

def render(name, *args):
    #if config.run_mode is 'debug':
    _global_render_ = web.template.render(config.views_config['template'])
    template = getattr(_global_render_, name)
    web.header('Content-Type', 'text/html')
    web.output(template(*args))    
    return None

def forward(url):
    return web.seeother(url)

def redirect(url):
    return web.redirect(url)