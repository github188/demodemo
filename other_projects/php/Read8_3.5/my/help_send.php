<?php
/**
*
*  Copyright:  (c) 2004 - 2008 [AOKOSOFT] Read8
*  Web Site:   http://www.aokosoft.com http://www.read8.net
*  Powered by: [AOKOSOFT] Read8 Group
*  Support:    http://bbs.read8.net
*  License:    http://www.read8.net/license
*
*/
/*
	求助 - 用户中心
*/

define('IN_UCP', true);
define('PHP_SELF_EXT', '_send');
define('PHP_SELF', basename(__FILE__, PHP_SELF_EXT.'.php'));


$title = $content = '';

require_once '../include/init.php';
require_once ROOT.'my/category.inc.php';


$db->single(__TAB_HELP__);

if(!$title)
	back('标题不能为空');

if(!$content)
	back('内容不能为空');


$db->insert(array(
	'uid'      => $Global['F_uid'],
	'title'    => $title,
	'content'  => $content,
	'dateline' => TIMESTAMP,
));

header('Location: help_check.php');
exit;