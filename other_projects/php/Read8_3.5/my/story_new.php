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
	新建 - 短篇文学
*/

define('IN_UCP', true);
define('PHP_SELF_EXT', '_new');
define('PHP_SELF', basename(__FILE__, PHP_SELF_EXT.'.php'));


require_once '../include/init.php';
require_once ROOT.'my/category.inc.php';

_header(array($_SYSTEM['CATEGORY'][PHP_SELF]));

$tpl = new AOKOTpl('navigation_ucp');
$tpl->assign('categories', $category);
$tpl->output();


$tpl = new AOKOTpl('my_'.PHP_SELF.PHP_SELF_EXT);
$tpl->assign(array(
	'subjects'  => $_CATEGORY['STORY']['subject'],
	'contents'  => $_CATEGORY['STORY']['content'],
	'states'    => $_CATEGORY['SYSTEM']['state'],
));
$tpl->output();

_footer();