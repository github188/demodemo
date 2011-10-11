# Django settings for Weibo project.

import os
import default_settings
try:
    import local_settings
except:
    local_settings = default_settings
def s(n):
    try:
        return getattr(local_settings, n)
    except AttributeError, e:
        return getattr(default_settings, n)

DEBUG = s('DEBUG')
TEMPLATE_DEBUG = DEBUG

ADMINS = (
    ('Your Name', 'your_email@domain.com'),
)

MANAGERS = ADMINS

DATABASES = {
    'default': {
        'NAME': s('DATABASE_NAME'),
        'ENGINE': s('DATABASE_ENGINE'),
        'USER': s('DATABASE_USER'),
        'PASSWORD': s('DATABASE_PASSWORD'),
        'HOST': s('DATABASE_HOST'),
        'PORT': s('DATABASE_PORT'),
    },
}

TEST_DATABASE_ENGINE = s('TEST_DATABASE_ENGINE')           # 'postgresql_psycopg2', 'postgresql', 'mysql', 'sqlite3' or 'oracle'.
TEST_DATABASE_NAME = s('TEST_DATABASE_NAME')             # Or path to database file if using sqlite3.

TIME_ZONE = s('TIME_ZONE')

# Language code for this installation. All choices can be found here:
# http://www.i18nguy.com/unicode/language-identifiers.html
LANGUAGE_CODE = 'zh-CN'

SITE_ID = 1

# If you set this to False, Django will make some optimizations so as not
# to load the internationalization machinery.
USE_I18N = True

# Absolute path to the directory that holds media.
# Example: "/home/media/media.lawrence.com/"
MEDIA_ROOT = ''

# URL that handles the media served from MEDIA_ROOT. Make sure to use a
# trailing slash if there is a path component (optional in other cases).
# Examples: "http://media.lawrence.com", "http://example.com/media/"
MEDIA_URL = ''

# URL prefix for admin media -- CSS, JavaScript and images. Make sure to use a
# trailing slash.
# Examples: "http://foo.com/media/", "/media/".
ADMIN_MEDIA_PREFIX = 'http://1.littlefun.sinaapp.com/django1.3/media/'

# Make this unique, and don't share it with anybody.
SECRET_KEY = 'xi85492#z7aqt)e&=czb9bm1u35*yznvh^84cxg4p!2icjr2go'

# List of callables that know how to import templates from various sources.
TEMPLATE_LOADERS = (
    ('django.template.loaders.cached.Loader', (
        'django.template.loaders.eggs.Loader',
        'django.template.loaders.app_directories.Loader',
      )),
    'django.template.loaders.filesystem.Loader',
    'django.template.loaders.app_directories.Loader',
)

MIDDLEWARE_CLASSES = (
    'django.middleware.common.CommonMiddleware',
    'django.contrib.sessions.middleware.SessionMiddleware',
    'django.contrib.auth.middleware.AuthenticationMiddleware',
)

TEMPLATE_CONTEXT_PROCESSORS = (
    'django.core.context_processors.auth',
    'django.core.context_processors.debug',
    'django.core.context_processors.i18n',
    'django.core.context_processors.media',
    'Weibo.coreapp.context_processors.tep_context',   
)

ROOT_URLCONF = 'Weibo.urls'

TEMPLATE_DIRS = (
    # Put strings here, like "/home/html/django_templates" or "C:/www/django/templates".
    # Always use forward slashes, even on Windows.
    # Don't forget to use absolute paths, not relative paths.
    os.path.join(os.path.dirname(os.path.abspath(__file__)), "templates")
)

INSTALLED_APPS = (
    'django.contrib.auth',
    'django.contrib.contenttypes',
    'django.contrib.sessions',
    'django.contrib.sites',
    'django.contrib.admin',
    'Weibo.coreapp',
    'Weibo.credit',
    'Weibo.params',
)


APP_ROOT = s('APP_ROOT')
WWW_ROOT = s('WWW_ROOT')
UPLOAD_ROOT = s('UPLOAD_ROOT')

