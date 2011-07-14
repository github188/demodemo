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
	收益 - 作家中心
*/

define('IN_AUCP', true);
define('PHP_SELF', basename(__FILE__, '.php'));


require_once '../include/init.php';
require_once ROOT.'author/category.inc.php';

_header(array($_SYSTEM['CATEGORY'][PHP_SELF]));

$tpl = new AOKOTpl('navigation_aucp');
$tpl->assign('categories', $category);
$tpl->output();

$tpl = new AOKOTpl('author_'.PHP_SELF);
$tpl->output();

_footer();