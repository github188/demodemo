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
	收藏分类更新 - 用户中心
*/

define('IN_UCP', true);
define('PHP_SELF_EXT', '_category');
define('PHP_SELF', basename(__FILE__, PHP_SELF_EXT.'.php'));


$favcd = $favc = array();
$newfavc = '';

require_once '../include/init.php';
require_once ROOT.'my/category.inc.php';


$db->single(__TAB_MY_FAV_CAT__);

$mfc = my_favorite_category();

if(!empty($favcd)) {
	foreach($favcd as $k => $v) {
		if(ris_int($v)) {
			// 删除分类则不需更新名称
			unset($favc[$k]);
		}else {
			// 去掉非法分类
			unset($favcd[$k]);
		}
	}
}


if(!empty($favcd)) {
	$db->update(array(
		'gid' => 0,
	), array(
		'uid' => $Global['F_uid'],
		'gid' => $favcd,
	), __TAB_MY_FAV__);
	$db->delete(array(
		'uid' => $Global['F_uid'],
		'gid' => $favcd,
	));
}

if(!empty($favc))
	foreach($mfc as $fc)
		if(isset($favc[$fc['gid']]) && $fc['category'] != $favc[$fc['gid']])
			$db->update(array(
				'category' => $favc[$fc['gid']],
			), array(
				'uid'      => $Global['F_uid'],
				'gid'      => $fc['gid'],
			));


if($newfavc)
	$db->insert(array(
		'uid'      => $Global['F_uid'],
		'category' => $newfavc,
	));

header('Location: favorite.php');
exit;