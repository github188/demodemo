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
	删除短信 - 用户中心
*/
define('IN_UCP', true);
define('PHP_SELF', basename(__FILE__, '.php'));

$id = $oid = $action = $del = $delo = "";

require_once '../include/init.php';

$db->debug='ture';

$db->single(__TAB_PM__);


if($id){
	if(!ris_int($id))
		back("不存在该信息!");

	if($db->delete(array(
		'id'=>$id,
		'isinbox' =>1,
		)))

		header('Location: pm_inbox.php');
		exit;
}

if($oid){
	if(!ris_int($oid))
		back("不存在该信息!");

	if($db->delete(array(
		'id'=>$oid,
		)))

		header('Location: pm_outbox.php');
		exit;

}

if($action=='删除选中项'){
	if(!$del)
		back("您还未选择要删除的项!");

		if($db->delete(array(
		'id'=>$del,
		'isinbox' =>1,
		)))

	jumpage('pm_inbox.php','信息删除成功!');

}

if($action1=='删除选中项'){
	if(!$delo)
		back("您还未选择要删除的项!");

		if($db->delete(array(
		'id'=>$delo,
		)))

	jumpage('pm_outbox.php','信息删除成功!');

}