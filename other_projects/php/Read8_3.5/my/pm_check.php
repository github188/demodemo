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
	短信发送 - 用户中心
*/

define('IN_UCP', true);
define('PHP_SELF_EXT', '_check');
define('PHP_SELF', basename(__FILE__, PHP_SELF_EXT.'.php'));

$action = $pmto = $pmtitle = $pmmsg = '';

require_once '../include/init.php';
require_once ROOT.'my/category.inc.php';


$db->single(__TAB_PM__);

	if(!$pmto)
		back('收件人不能为空');

	if(!$pmtitle)
		back('主题不能为空');

	if(!$pmmsg)
		back('内容不能为空');

	if(!$rows=$db->selectOne(array(
		'field' => 'uid',
		'from'  =>__TAB_USER__,
		'where' =>array('username'=>$pmto),
	)))
		back('收件人错误!');

	if($db->insert(array(
		'from_id' => $Global['F_uid'],
		'to_id'   => $rows['uid'],
		'from'    => $Global['F_username'],
		'to'      => $pmto,
		'title'   => $pmtitle,
		'content' => $pmmsg,
		'dateline'=>TIMESTAMP,
		'isread'  =>0,
		'isinbox' =>0,
	))  &&   $db->insert(array(
		'from_id' => $Global['F_uid'],
		'to_id'   => $rows['uid'],
		'from'    => $Global['F_username'],
		'to'      => $pmto,
		'title'   => $pmtitle,
		'content' => $pmmsg,
		'dateline'=>TIMESTAMP,
		'isread'  =>0,
		'isinbox' =>1,
	)))

	jumpage('pm_outbox.php','信息发送成功!');


