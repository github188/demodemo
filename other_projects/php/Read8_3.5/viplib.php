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

$page = 1;

define('IN_Read8', true);
require_once str_replace(array('\\', '/'.basename(__FILE__)), array('/', ''), __FILE__).'/include/init.php';

_header(array('电子书店'));//输出头部



$tpl = new AOKOTpl('viplib');

$tpl->assign(array(
	'novels'   => $db->select(array(
		'field' => 'nc.title AS chapter_title, nc.id AS cid, nc.priority, n.id AS bid, n.title, n.author, n.author_id, n.subject AS bp_t, n.content, n.path, n.lastupdate',
		'from'  => $db->table(__TAB_NOVEL__, 'n'),
		'join'  => array(
			'table' => $db->table(__TAB_NOVEL_CHAPTER__, 'nc'),
			'on'    => 'n.id = nc.book_id',
		),
		'where' => 'WHERE nc.vip = 1',
		'order' => 'n.lastupdate DESC',
		'limit' => 20,
		'page'  => $page,
		'filter'=> '_novel_callback',
		'counter'=>1,
	)),
	'pagelink' => page($db->counter, 20, 0, 1),
));

$tpl->output();

_footer();
