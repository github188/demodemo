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
//后台统计页面

if(!defined('IN_Read8') || !defined('IN_Read8_ACP') || !isset($PHP_SELF)) {
	exit('Access Denied');
}
back('暂不提供');
if(!acppriv(3)) back('admin_no_privileges_logs');

$category = str_replace('count_','',$pfile);

$tpl = new AOKOTpl('count');
$tpl->assign('category', $category);

switch($category) {
	case 'global':
		$sqls = array(
			'allusers'      => "SELECT COUNT(uid) AS totalnum FROM ".__TAB_USER__,
			'todayusers'    => "SELECT COUNT(uid) AS totalnum FROM ".__TAB_USER__." WHERE dateline>=".$Global['F_tdtime'],
			//'groupusers'    => "SELECT COUNT(uid) AS totalnum, group_id FROM".__TAB_USER__." GROUP BY group_id",
			'chapters'      => "SELECT COUNT(id) AS totalnum FROM ".__TAB_CHAPTER__,
			'todaychapters' => "SELECT COUNT(id) AS totalnum FROM ".__TAB_CHAPTER__." WHERE dateline>=".$Global['F_tdtime'],
			'books'         => "SELECT COUNT(id) AS totalnum FROM ".__TAB_BOOK__,
			'todaybooks'    => "SELECT COUNT(id) AS totalnum FROM ".__TAB_BOOK__." WHERE dateline>=".$Global['F_tdtime'],
			'bank'          => "SELECT SUM(money) AS totalnum FROM ".__TAB_MOD_VIP_AUTOBANK__." WHERE ischeck=1",
			'todaybank'     => "SELECT SUM(money) AS totalnum FROM ".__TAB_MOD_VIP_AUTOBANK__." WHERE ischeck=1 AND dateline>=".$Global['F_tdtime'],
			'pay'           => "SELECT SUM(money) AS totalnum FROM ".__TAB_MOD_VIP_APPLYFEE__." WHERE kind=1 AND checktime>1",
			'todaypay'      => "SELECT SUM(money) AS totalnum FROM ".__TAB_MOD_VIP_APPLYFEE__." WHERE kind=1 AND checktime>=".$Global['F_tdtime'],
			'fee'           => "SELECT SUM(money) AS totalnum FROM ".__TAB_MOD_VIP_APPLYFEE__." WHERE kind=0 AND checktime>1",
			'todayfee'      => "SELECT SUM(money) AS totalnum FROM ".__TAB_MOD_VIP_APPLYFEE__." WHERE kind=0 AND checktime>=".$Global['F_tdtime'],
		);
		foreach($sqls as $key => $sql) {
			$row = $db->selectBySql($sql, 'array_shift');
			$tpl->assign("C_$key", intval($row['totalnum']));
		}
		break;
	case 'book':
		$j = 0;
		foreach($_CACHE['booktypebytid'] as $type_id => $type_name) {
			if($j == 4) $j = 0;
			$i = ceil($type_id / 4) - 1;
			$trlist[$i]['ranklist'][$j]['name'] = $type_name;
			//$result = $db->query("SELECT COUNT(h.click) AS totalnum, b.id, b.title, b.type_id FROM ".__TAB_BOOKHITS__." h LEFT JOIN ".__TAB_BOOK__." b ON h.book_id=b.id WHERE b.type_id='$type_id' GROUP BY b.id ORDER BY totalnum DESC LIMIT 0, 10");
			$rank = 1;
			//$trlist[$i]['ranklist'][$j]['list'] = array();
			$trlist[$i]['ranklist'][$j]['list'] = $db->select(array(
				'field' => 'COUNT(h.click) AS C_num,
							b.id           AS C_bid,
							b.title        AS C_title',
				'from'  => $db->table(__TAB_BOOKHITS__, 'h'),
				'join'  => array('table' => $db->table(__TAB_BOOK__, 'b'), 'on' => 'h.book_id = b.id'),
				'where' => array('b.type_id' => $type_id),
				'group' => 'b.id',
				'order' => 'C_num DESC',
				'limit' => 10,
			));
			foreach($trlist[$i]['ranklist'][$j]['list'] as $key => $row) {
				$trlist[$i]['ranklist'][$j]['list'][$key]['C_rank'] = $rank;
				$rank++;
			}
			$j++;
		}
		$tpl->assign('trlist', $trlist);
		break;
	case 'writer':
		$sqladds = array(
			'day'   => ' WHERE c.dateline>='.$Global['F_tdtime'],
			'week'  => ' WHERE c.dateline>='.($Global['F_tdtime'] - 86400 * 7),
			'month' => ' WHERE c.dateline>='.($Global['F_tdtime'] - 86400 * 30),
			'all' => '',
		);
		$i = 0;
		foreach($sqladds as $key => $sqladd) {
			$ranklist[$i]['name'] = Lang($key, 'admin_count_rank_');
			//$result = $db->query("SELECT COUNT(c.id) AS totalnum, b.username FROM ".__TAB_CHAPTER__." c LEFT JOIN ".__TAB_BOOK__." b ON b.id=c.book_id $sqladd GROUP BY b.username ORDER BY totalnum DESC LIMIT 0, 10");
			$rank = 1;
			//$ranklist[$i]['list'] = array();
			$ranklist[$i]['list'] = $db->selectBySql("SELECT COUNT(c.id) AS C_num, b.username AS C_name FROM ".__TAB_CHAPTER__." c LEFT JOIN ".__TAB_BOOK__." b ON b.id=c.book_id $sqladd GROUP BY b.username ORDER BY C_num DESC LIMIT 0, 10");
			foreach($ranklist[$i]['list'] as $key => $row) {
				$ranklist[$i]['list'][$key]['C_rank'] = $rank;
				$rank++;
			}
			$i++;
		}
		$tpl->assign('ranklist', $ranklist);
		break;
}

$tpl->output();
