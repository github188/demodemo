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
	exit('Access Denied (friend_link.php)');
}


$db->single(__TAB_LINKS__);

empty($linkid) && $linkid = 0;
empty($move) && $move = 0;

if($action=='add' && $sitename) {
	if($linkid) {
		if($sitelogo && substr($sitelogo, 0, 9) != 'data/logo') {
			rfow(ROOT.'data/logo/'.$linkid.'.'.rfileext($sitelogo), rfor('http://'.$sitelogo));
			$sitelogo = 'data/logo/'.$linkid.'.'.rfileext($sitelogo);
		}

		$db->update(array(
			'name' => $sitename,
			'url'  => $siteurl,
			'logo' => $sitelogo,
		), array(
			'id'   => $linkid,
		));
	}else {
		$linkid = $db->insert(array(
			'name' => $sitename,
			'url'  => $siteurl,
		));
		if($sitelogo) {
			rfow(ROOT.'data/logo/'.$linkid.'.'.rfileext($sitelogo), rfor('http://'.$sitelogo));
			$sitelogo = 'data/logo/'.$linkid.'.'.rfileext($sitelogo);

			$db->update(array(
				'logo' => $sitelogo,
			), array(
				'id'   => $linkid,
			));
		}
	}
	jumpage("?ptype=$ptype&pfile=$pfile", '修改成功');
}elseif($action=='delete' && $linkid) {
	$db->execUpBySql("DELETE FROM ".__TAB_LINKS__." WHERE id='$linkid'");
	jumpage("?ptype=$ptype&pfile=$pfile", '删除成功');
}elseif($action=='verify' && $linkid) {
	$db->execUpBySql("UPDATE ".__TAB_LINKS__." SET apply='0' WHERE id='$linkid'");
	jumpage("?ptype=$ptype&pfile=$pfile", '审核成功');
}elseif($action=='move' && $linkid && $move) {
	$rows = $db->select(array(
		'field' => 'id, priority',
		'where' => array(
			'apply' => 0,
		),
		'order' => 'priority ASC',
	));
	foreach($rows as $k => $v) {
		$db->update(array(
			'priority' => $k + 1,
		), array(
			'id'       => $v['id'],
		));
		if($v['id'] == $linkid) {
			$p = $k + 1;
		}
	}
	if($move > 0) {
		$db->update(array(
			'priority' => $p,
		), array(
			'priority' => $p + 1,
		));
		$db->update(array(
			'priority' => $p + 1,
		), array(
			'id'       => $linkid,
		));
	}else {
		$db->update(array(
			'priority' => $p,
		), array(
			'priority' => $p - 1,
		));
		$db->update(array(
			'priority' => $p - 1,
		), array(
			'id'       => $linkid,
		));
	}
	jumpage("?ptype=$ptype&pfile=$pfile", '移动成功');
}


if(!defined('PAGE_SIZE')) {
	define('PAGE_SIZE', 20);
}

//显示
$tpl = new AOKOTpl($pfile);
$tpl->assign(array(
	'links'     => $db->select(array(
						'where' => array(
							'apply' => 0,
						),
						'order' => 'priority ASC',
						//'limit' => PAGE_SIZE,
						//'page'  => $page,
						'filter'=> 'html_show',
						//'counter'=>1,
					)),
	//'pagelink1' => page($db->counter, PAGE_SIZE),
	'apply'     => $db->select(array(
						'where' => array(
							'apply' => 1,
						),
						'order' => 'priority ASC',
						//'limit' => PAGE_SIZE,
						//'page'  => $page,
						'filter'=> 'html_show',
						//'counter'=>1,
					)),
	//'pagelink2' => page($db->counter, PAGE_SIZE),
));
$tpl->output();
