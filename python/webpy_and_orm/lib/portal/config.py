import os

server_root = os.path.abspath(__file__)
(base_dir, file_name) = os.path.split(server_root)
server_root = os.path.abspath(os.path.join(server_root, '..', '..', '..'))

import portal.models
db_config = {
    'default':{
        'module':portal.models,
        'dbn':'sqlite',
        'db':os.path.join(server_root, 'db', 'best_sales.db'),
        'auto_schema':'create'
    }
 }

views_config = {
    'template':os.path.join(server_root, 'web')
}

import portal.urls
url_config = ('/', portal.urls)

listen_port=8080

run_mode='debug'

#used for static serve
web_root = (os.path.join(server_root, 'web'), )

robot_scripts_dir = os.path.join(server_root, 'scripts', 'robot_scripts')
robot_logs_dir = os.path.join(server_root, 'logs')



