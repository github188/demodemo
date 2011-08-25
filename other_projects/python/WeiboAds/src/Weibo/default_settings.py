import os
DEBUG = True

DATABASE_ENGINE = 'mysql'           # 'postgresql_psycopg2', 'postgresql', 'mysql', 'sqlite3' or 'oracle'.
DATABASE_NAME = 'weibo_ads'             # Or path to database file if using sqlite3.
DATABASE_USER = 'root'             # Not used with sqlite3.
DATABASE_PASSWORD = ''         # Not used with sqlite3.
DATABASE_HOST = '127.0.0.1'             # Set to empty string for localhost. Not used with sqlite3.
DATABASE_PORT = '3306'             # Set to empty string for default. Not used with sqlite3.

TEST_DATABASE_ENGINE = 'sqlite3'   # 'postgresql_psycopg2', 'postgresql', 'mysql', 'sqlite3' or 'oracle'.
TEST_DATABASE_NAME = 'test_tep'

TIME_ZONE = 'PRC' 
ADMIN_MEDIA_PREFIX = '/admin'

APP_ROOT = 'http://127.0.0.1:8000'
WWW_ROOT = "."