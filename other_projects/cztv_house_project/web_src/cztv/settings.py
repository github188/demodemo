# Django settings for cztv project.
import os

import cztv_settings_default
try:
    import cztv_settings
except ImportError:
    cztv_settings = None


# Path to the directory containing this file. Don't edit!
_BASEDIR = os.path.dirname(os.path.abspath(__file__))

# Helper method to get local settings either from `rfdocsettings.py`
# or `rfdocsettings_defaults.py`.
def localsetting(name):
    try:
        return getattr(cztv_settings, name)
    except AttributeError:
        return getattr(cztv_settings_default, name)


DEBUG = localsetting('DEBUG')
TEMPLATE_DEBUG = DEBUG

ADMINS = (
    # ('Your Name', 'your_email@domain.com'),
)

MANAGERS = ADMINS

DATABASE_ENGINE = localsetting('DATABASE_ENGINE') #'mysql' 
DATABASE_NAME = localsetting('DATABASE_NAME')
TIME_ZONE = localsetting('TIME_ZONE')

DATABASE_USER = localsetting('DATABASE_USER')
DATABASE_PASSWORD = localsetting('DATABASE_PASSWORD') 
DATABASE_HOST = localsetting('DATABASE_HOST')
DATABASE_PORT = localsetting('DATABASE_PORT')

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
ADMIN_MEDIA_PREFIX = '/media/'

# Make this unique, and don't share it with anybody.
SECRET_KEY = 'ya36z9oc0f4$$$2*)=btouk%td7$35#45at%1s#)=q@x4%@_8@'

# List of callables that know how to import templates from various sources.
TEMPLATE_LOADERS = (
    'django.template.loaders.filesystem.load_template_source',
    'django.template.loaders.app_directories.load_template_source',
#     'django.template.loaders.eggs.load_template_source',
)

MIDDLEWARE_CLASSES = (
    'django.middleware.common.CommonMiddleware',
    'django.contrib.sessions.middleware.SessionMiddleware',
    'django.contrib.auth.middleware.AuthenticationMiddleware',
)

ROOT_URLCONF = 'cztv.urls'

TEMPLATE_DIRS = (
    # Put strings here, like "/home/html/django_templates" or "C:/www/django/templates".
    # Always use forward slashes, even on Windows.
    # Don't forget to use absolute paths, not relative paths.
    os.path.join(_BASEDIR, 'templates').replace('\\', '/'),
)

INSTALLED_APPS = (
    'django.contrib.auth',
    'django.contrib.contenttypes',
    'django.contrib.sessions',
    'django.contrib.sites',
    'django.contrib.admin', 
    'cztv.coreapp',
    'cztv.houseadmin',
)

#DATA_ROOT = localsetting('DATA_ROOT')
WWW_ROOT = os.path.normpath(os.path.join(_BASEDIR, '..', 'www_root'))
APP_ROOT = localsetting('APP_ROOT')

