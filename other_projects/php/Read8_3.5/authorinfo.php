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
//作者介绍程序

require_once './include/init.php';
//设定搜索关键词
$seokeyword[]=$target;
$seokeyword[]='authorinfo';
_header($seokeyword);//输出头部

$row = $db->selectOne(array(
	'field' => 'uid, dateline, comm',
	'from'  => __TAB_USER__,
	'where' => array('username' => $target),
));

$tpl = new Template('authorinfo');
$tpl->assign('C_viewuserstatus',ViewUserStatus());
$tpl->assign(array(
	'C_regdate' => $row ? rdate($row['dateline'], 'Y-m-d H:i') : '--',
	'C_author'  => $target,
	'C_comm'    => $row ? PrintCodes($row['comm']) : '--',
));

$Newbookblock = $db->select(array(
	'field' => 'b.id      AS C_bookid,
				b.title   AS C_booktitle,
				b.type_id AS C_typeid,
				c.orderid AS C_orderid,
				c.title   AS C_chaptertitle',
	'from'  => $db->table(__TAB_BOOK__, 'b'),
	'join'  => array('table' => $db->table(__TAB_CHAPTER__, 'c'), 'on' => 'b.newchapterid = c.id'),
	'where' => array('b.author' => $target),
));
$tpl->assign('Newbookblock', $Newbookblock);
unset($Newbookblock);


$tpl->output();

_footer();
