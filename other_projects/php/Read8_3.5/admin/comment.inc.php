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
if(!defined('IN_Read8') || !defined('IN_Read8_ACP') || !isset($PHP_SELF)) {
	exit('Access Denied (comment.inc.php)');
}


list($pf, $ps) = explode('_', $pfile);

$db->single($ps == 'novel' ? __TAB_NOVEL_COMMENT__ : __TAB_STORY_COMMENT__);


if(!empty($cids) && is_array($cids)) {
	foreach($cids as $k => $v) {
		if(ris_int($v) && isset($bids[$k])) {
			$db->update(array(
				'active'  => 0,
			), array(
				'book_id' => $bids[$k],
				'id'      => $v,
			));
			_log('acp', array($Global['F_username'], '书籍管理', '评论管理', '删除书籍 '.$bids[$k].'的评论 '.$v));
		}
	}

	jumpage("?ptype=$ptype&pfile=$pfile", '删除成功');
}


if($ps == 'novel') {
	$sql = array(
		'field' => 'nc.id,
					nc.book_id    AS bid,
					nc.chapter_id AS cid,
					nc.uid,
					nc.username,
					nc.title,
					nc.content,
					nc.ip,
					nc.digest,
					nc.sticky,
					nc.highlight,
					nc.dateline,
					n.title       AS book_title,
					n.path',
		'from'  => $db->table(__TAB_NOVEL_COMMENT__, 'nc'),
		'join'  => array('table' => $db->table(__TAB_NOVEL__, 'n'), 'on' => 'n.id = nc.book_id'),
		'where' => array(
			'nc.active'  => 1,
		),
		'order' => 'nc.dateline DESC',
		'page'  => $page,
		'limit' => 20,
		'filter'=> 'comment_filter',
		'counter'=>1,
	);
}else {
	$sql = array(
		'field' => 'nc.id,
					nc.book_id    AS bid,
					nc.uid,
					nc.username,
					nc.title,
					nc.content,
					nc.ip,
					nc.digest,
					nc.sticky,
					nc.highlight,
					nc.dateline,
					n.title       AS book_title',
		'from'  => $db->table(__TAB_STORY_COMMENT__, 'nc'),
		'join'  => array('table' => $db->table(__TAB_STORY__, 'n'), 'on' => 'n.id = nc.book_id'),
		'where' => array(
			'nc.active'  => 1,
		),
		'order' => 'nc.dateline DESC',
		'page'  => $page,
		'limit' => 20,
		'filter'=> 'comment_filter',
		'counter'=>1,
	);
}



//显示
$tpl = new AOKOTpl('comment');
$tpl->assign('comments', $db->select($sql));
$tpl->assign('pagelink', Page($db->counter, 20));
$tpl->output();

