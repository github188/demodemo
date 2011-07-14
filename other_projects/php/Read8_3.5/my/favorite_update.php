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
	收藏更新 - 用户中心
*/

define('IN_UCP', true);
define('PHP_SELF_EXT', '_category');
define('PHP_SELF', basename(__FILE__, PHP_SELF_EXT.'.php'));


$favs = array();
$favm = $delete = '';

require_once '../include/init.php';
require_once ROOT.'my/category.inc.php';


$db->single(__TAB_MY_FAV__);

$mfc = my_favorite_category();

if(!empty($favs))
	foreach($favs as $k => $v)
		if(!ris_int($v))
			unset($favs[$k]);

if(!empty($favs)) {
	if($delete) {
		$rows = $db->select(array(
			'field' => 'bid',
			'where' => array(
				'uid' => $Global['F_uid'],
				'fid' => $favs,
			),
		));
		$bids = array();
		foreach($rows as $v)
			$bids[] = $v['bid'];

		$db->execUpBySql('UPDATE '.__TAB_NOVEL_EXT__.' SET favo_week = favo_week - 1, favo_month = favo_month - 1, favo_total = favo_total - 1 WHERE book_id IN ('.implode(', ', $bids).')');
		$db->delete(array(
			'uid' => $Global['F_uid'],
			'fid' => $favs,
		));
	}else {
		if($favm) {// 有分类号
			foreach($mfc as $fc) {
				if($favm == $fc['gid']) {// 分类合法
					$db->update(array(
						'gid' => $fc['gid'],
					), array(
						'uid' => $Global['F_uid'],
						'fid' => $favs,
					));
					break;
				}
			}
		}else {// 尚未分类
			$db->update(array(
				'gid' => 0,
			), array(
				'uid' => $Global['F_uid'],
				'fid' => $favs,
			));
		}
	}
}

header('Location: favorite.php');
exit;