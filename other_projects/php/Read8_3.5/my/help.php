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
define('PHP_SELF', basename(__FILE__, '.php'));


require_once '../include/init.php';
require_once ROOT.'my/category.inc.php';


_header(array($_SYSTEM['CATEGORY'][PHP_SELF]));

$tpl = new AOKOTpl('navigation_ucp');
$tpl->assign('categories', $category);
$tpl->output();


$tpl = new AOKOTpl('my_'.PHP_SELF);

$tpl->assign(array(
	'current' => 1,
));
$tpl->output();

_footer();