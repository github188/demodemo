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
//后台友情链接管理

if(!defined('IN_Read8') || !defined('IN_Read8_ACP') || !isset($PHP_SELF)) {
	exit('Access Denied (advertise.php)');
}


$db->single(__TAB_AD__);

$ads = $db->select();

if($action == 'update') {
	foreach($ads as $ad) {
		$var = $ad['pos'];
		if(isset($$var)) {
			$db->update(array(
				'ad'  => $$var,
			), array(
				'pos' => $var,
			));
		}
	}

	_clear_cache('cache');
	_clear_cache('template/');
	jumpage("?ptype=$ptype&pfile=$pfile",'管理成功');
}


$tpl = new AOKOTpl($pfile);

foreach($ads as $ad) {
	$tpl->assign('C_'.$ad['pos'], html_show($ad['ad'], false));
}

$tpl->output();
