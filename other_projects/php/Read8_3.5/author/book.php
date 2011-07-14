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
	更新 - 作家中心
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
	'books' => html_show(my_novel(), false),
));

	/*$tpl->assign(array(
		'books' => $db->select(array(
			'field' => 'id, title, description, subject, content, style, license, state, dateline, lastupdate, words',
			'from'  => __TAB_NOVEL__,
			'where' => array('typewriter_id' => $Global['F_uid']),
			'filter'=> 'convert_novel_classes',
		)),
	));*/

$tpl->output();

_footer();