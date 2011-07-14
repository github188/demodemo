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
//后台作家审核

if(!defined('IN_Read8') || !defined('IN_Read8_ACP') || !defined('IN_Read8_ACP_AUT') || !isset($PHP_SELF)) {
	exit('Access Denied (author_verify.php)');
}



//审核处理
if($action=='deny' && $denybid) {
	$db->update(array(
		'active' => -1,
	), array(
		'active' => 0,
		'id'     => $denybid,
	));
	//增加日志记录
	$row = $db->selectOne(array(
		'field' => 'username',
		'where' => array('id' => $denybid),
	));
	//$logcon="作家申请拒绝<".$row['username'].">";
	//AddLog($logcon);

	jumpage("?ptype=$ptype&pfile=$pfile",'admin_verify_writer_success_deny');
}
if($action=='apply' && $cbid) {//作家申请通过
	$db->update(array(
		'active' => 1,
	), array(
		'active' => 0,
		'id'     => $cbid,
	));
	//增加日志记录
	//$logcon="作家申请通过<".$row['username'].">";
	//AddLog($logcon);

	jumpage("?ptype=$ptype&pfile=$pfile",'admin_verify_writer_success_pass');
}





//显示作家申请列表
//$row = $db->selectOne(array(
//	'field' => 'COUNT(*) AS maxnum',
//));
//$pagelink=Page($row['maxnum'],$Global['tiplist'],0,0);

//$sql="SELECT id, username, dateline, checktime, checkkind, ps FROM ".__TAB_APPLYWRITER__." ORDER BY checktime,dateline DESC ".$pagelink['limit'];
//$result=$db->query($sql);
$itemlist = $db->select(array(
	'field' => 'id, pseudonym, dateline',
	'where' => array('active' => 0),
	'order' => 'dateline DESC',
));

//显示
$tpl = new AOKOTpl("checkwriter");
$tpl->assign("itemlist",$itemlist);
//$tpl->assign("C_pagelink",$pagelink['output']);
$tpl->output();

