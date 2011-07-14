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
//打包下载管理

if(!defined('IN_Read8') || !defined('IN_Read8_ACP') || !isset($PHP_SELF)) {
	exit('Access Denied (setzips.php)');
}


//更新操作
$updated = false;
if(!empty($delzips) && is_array($delzips)) {
	foreach($delzips as $val){
		if(substr($val,-4)=='.zip') unlink(ROOT."data/zip/$val");
	}
	//日志记录
	$logcon="打包下载管理<批量删除".count($delzips)."个zip>";
	$updated = true;
}
if(!empty($deltxts) && is_array($deltxts)) {
	foreach($deltxts as $val){
		if(substr($val,-4)=='.txt') unlink(ROOT."data/txt/$val");
	}
	//日志记录
	$logcon="打包下载管理<批量删除".count($delzips)."个txt>";
	$updated = true;
}
$updated && jumpage("?ptype=$ptype&pfile=$pfile", 'admin_manage_book_zip_success_del');


//主体内容显示
$ziplist = $txtlist = array();
$dirlist = rlistfile(ROOT."data/zip");
foreach($dirlist as $val){
	if(substr($val,-4)!='.zip') continue;
	$ziplist[]['C_filename']=$val;
}
$dirlist = rlistfile(ROOT."data/txt");
foreach($dirlist as $val){
	if(substr($val,-4)!='.txt') continue;
	$txtlist[]['C_filename']=$val;
}

$tpl = new AOKOTpl('setzips');
$tpl->assign('ziplist', $ziplist);
$tpl->assign('txtlist', $txtlist);
$tpl->output();

