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
	审核 - 作家中心
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
	'novels' => html_show($db->select(array(
		'field' => 'title, dateline, verifytime, book_id',
		'from'  => __TAB_NOVEL_APPLY__,
		'where' => array('author_id' => $_AUTHOR['id']),
		'order' => 'verifytime DESC, dateline DESC',
	)), false),
));
$tpl->output();

_footer();