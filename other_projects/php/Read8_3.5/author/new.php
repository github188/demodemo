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
	新书 - 作家中心
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
$tpl->assign(array(
	'pseudonym' => $_AUTHOR['pseudonym'],
	'subjects'  => $_CATEGORY['NOVEL']['subject'],
	'contents'  => $_CATEGORY['NOVEL']['content'],
	'styles'    => $_CATEGORY['SYSTEM']['style'],
	'licenses'  => $_CATEGORY['SYSTEM']['license'],
));
$tpl->output();

_footer();