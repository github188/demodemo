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
	收藏 - 用户中心
*/

define('IN_UCP', true);
define('PHP_SELF', basename(__FILE__, '.php'));


$ajax = $bid = $cid = $gid = '';

require_once '../include/init.php';
require_once ROOT.'my/category.inc.php';

$db->single(__TAB_MY_FAV__);

if($ajax) {
	header('Content-Type: application/json; charset='.SYSCHARSET);
	if(!$bid || !ris_int($bid) || !novel_exists($bid))
		exit("alert('该书不存在');");

	$fav = $db->selectOne(array(
		'field' => 'fid',
		'where' => array(
			'bid' => $bid,
			'uid' => $Global['F_uid'],
		)
	));
	if($fav) {
		$db->update(array(
			'cid'      => $cid && ris_int($cid) ? $cid : 0,
			'dateline' => TIMESTAMP,
		), array(
			'uid'      => $Global['F_uid'],
			'fid'      => $fav['fid'],
		));
		exit("alert('收藏已更新');");
	}else {
		$db->execUpBySql('UPDATE '.__TAB_NOVEL_EXT__.' SET favo_today = favo_today + 1 WHERE book_id = '.$bid);
		$db->insert(array(
			'uid'      => $Global['F_uid'],
			'bid'      => $bid,
			'cid'      => $cid && ris_int($cid) ? $cid : 0,
			'dateline' => TIMESTAMP,
		));
		exit("alert('收藏已添加');");
	}
	exit;
}

if($gid && !my_favorite_check($gid))
	back('未知书架');


_header(array($_SYSTEM['CATEGORY'][PHP_SELF]));

$tpl = new AOKOTpl('navigation_ucp');
$tpl->assign('categories', $category);
$tpl->output();

$tpl = new AOKOTpl('my_'.PHP_SELF);
$tpl->assign(array(
	'favcats' => my_favorite_category(),
	'favlist' => my_favorite($gid),
));
$tpl->output();

_footer();