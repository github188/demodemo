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
	评论 - 用户中心
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
$tpl->assign('comments', $db->select(array(
	'field' => 'nc.title,
				nc.content,
				nc.book_id    AS bid,
				n.title       AS novel_title,
				n.path,
				nc.chapter_id AS cid,
				cp.title      AS chapter_title,
				nc.dateline',
	'from'  => $db->table(__TAB_NOVEL_COMMENT__, 'nc'),
	'join'  => array(
		array('table' => $db->table(__TAB_NOVEL__,         'n'),  'on' => 'n.id = nc.book_id'),
		array('table' => $db->table(__TAB_NOVEL_CHAPTER__, 'cp'), 'on' => 'cp.id = nc.chapter_id'),
	),
	'where' => array(
		'nc.uid'    => $Global['F_uid'],
		'nc.active' => 1,
	),
	'order' => 'nc.dateline DESC',
	'filter'=> 'comment_filter',
)));
$tpl->output();



_footer();