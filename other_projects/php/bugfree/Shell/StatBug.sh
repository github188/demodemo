#!/bin/bash
# File to Mail bugs assigned to somebody of BugFree system.
#
# BugFree is free software under the terms of the FreeBSD License.
#
# @link        http://www.bugfree.org.cn
# @package     BugFree
#
ServerName="http://bugfree.corp.taobao.com"
/opt/lampp/bin/php /opt/lampp/htdocs/bugfree/StatBug.php $ServerName
