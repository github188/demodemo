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
//后台公告设置

if(!defined('IN_Read8') || !defined('IN_Read8_ACP') || !isset($PHP_SELF)) {
	exit('Access Denied (announce.php)');
}


$db->single(__TAB_ANNOUNCE__);

if($action=='update' && $content) {//发表公告
	if(!empty($editid)) {
		$db->update(array(
			'title'    => $title,
			'content'  => $content,
			'dateline' => TIMESTAMP,
		), array(
			'id'       => $editid,
		));
		//$sql="UPDATE ".__TAB_ANNOUNCE__." SET content='$content',dateline='".TIMESTAMP."' WHERE id='$editid'";
		//$db->query($sql);
		$logcon="修改公告<ID:$editid>";
	}else {
		$insertid = $db->insert(array(
			'title'    => $title,
			'content'  => $content,
			'dateline' => TIMESTAMP,
		));
		//$sql='INSERT INTO '.__TAB_ANNOUNCE__." VALUES(NULL, '$content', '".TIMESTAMP."')";
		//$db->query($sql);
		$logcon="发布公告<ID:".$insertid.">";
	}
	_clear_cache('cache');
	//日志记录
	jumpage("?ptype=$ptype&pfile=$pfile",'admin_manage_announce_success_edit');
}
if($action=='del' && $delid) {//删除公告
	$db->delete(array(
		'id' => $delid,
	));
	//$sql='DELETE FROM '.__TAB_ANNOUNCE__." WHERE id='$delid'";
	//$db->query($sql);
	_clear_cache('cache');
	//日志记录
	jumpage("?ptype=$ptype&pfile=$pfile",'admin_manage_announce_success_del');
}


$tpl = new AOKOTpl('announce');


//公告列表
//$sql="SELECT * FROM ".__TAB_ANNOUNCE__." ORDER BY id desc";
//$result=$db->query($sql);
//$itemlist=array();
$tpl->assign("itemlist", html_show($db->select(array(
	'field' => 'id, title, content, dateline',
	'order' => 'id DESC',
))));

//若有编辑公告
if($action=='update' && !empty($editid)) {
	//$sql='SELECT * FROM '.__TAB_ANNOUNCE__." WHERE id='$editid'";
	//$row=$db->fetch_one($sql);
	$tpl->assign(html_show($db->selectOne(array(
		'field' => 'title, content',
		'where' => array('id' => $editid),
	))));
	$ttl = '修改';
}else {
	$tpl->assign(array(
		'title'   => '',
		'content' => '',
	));
	$ttl = '新增';
}

//显示
$tpl->assign("ttl",$ttl);
$tpl->assign("editid", !empty($editid) ? $editid : '');
$tpl->output();

