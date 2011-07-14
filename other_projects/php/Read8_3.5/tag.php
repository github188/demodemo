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
//书籍标签程序

$tag = '';
$page=1;
define('IN_Read8', true);
require_once str_replace(array('\\', '/'.basename(__FILE__)), array('/', ''), __FILE__).'/include/init.php';

_header($seokeyword);//输出头部


$db->single(__TAB_NOVEL__);

$tpl=new AOKOTpl('tag');
$tpl->assign('lists',$db->select(array(
	'field' => 'n.*,nc.priority, nc.title AS chapter_title',
	'from'  => $db->table(__TAB_NOVEL__, 'n'),
	'join'  => array('table' => $db->table(__TAB_NOVEL_CHAPTER__, 'nc'), 'on' => 'n.id = nc.book_id AND n.lastchapter = nc.id'),
	'where' => "where tag like '%$tag%'",
)));

$tpl->assign(array(
	'url'	   => _novel($tag),
	'pagelink' => page($db->counter, 20, 0, 1),
));

$tpl->output();

_footer();

?>