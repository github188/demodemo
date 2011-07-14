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
	删除 - 短篇文学
*/

define('IN_UCP', true);
define('PHP_SELF_EXT', '_delete');
define('PHP_SELF', basename(__FILE__, PHP_SELF_EXT.'.php'));


$bid = '';

require_once '../include/init.php';
require_once ROOT.'my/category.inc.php';


$book = story_exists($bid);
if(!$book) {
	back('无此书信息');
}


include _config('story');

if(!$_SYSTEM['STORY']['open'])
	back('此功能未开放');


$db->update(array(
	'state' => 0,
), array(
	'id'    => $bid,
), __TAB_STORY__);


header('Location: story.php');
exit;
