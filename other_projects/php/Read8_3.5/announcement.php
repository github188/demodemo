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
	announcement.php
	Mar. 3rd. 2008
*/


define('IN_Read8', true);
require_once str_replace(array('\\', '/'.basename(__FILE__)), array('/', ''), __FILE__).'/include/init.php';


$db->single(__TAB_ANNOUNCE__);


_header(array('所有公告'));

$tpl = new AOKOTpl('announcement');
$tpl->assign('Announcements', $db->select(array(
	'field' => 'title,
				content,
				dateline',
	'order' => 'dateline DESC',
)));
$tpl->output();

_footer();