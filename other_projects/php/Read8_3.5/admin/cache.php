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
//缓存管理

if(!defined('IN_Read8') || !defined('IN_Read8_ACP') || !isset($PHP_SELF)) {
	exit('Access Denied (cache.php)');
}


if($action=='clear') {
	_clear_cache($prefix);
	jumpage("?ptype=$ptype&pfile=$pfile","更新成功");
}


$tpl = new AOKOTpl('cache');
$tpl->output();
