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


define('IN_Read8', true);
require_once str_replace(array('\\', '/'.basename(__FILE__)), array('/', ''), __FILE__).'/include/init.php';
require_once ROOT.'include/image.func.php';

include _config('vip');


header('Content-Type: image/png');

$book = get_novel_by_id($bid);
if(!$book) {
	readfile($_SYSTEM['VIP']['buy_picture']);
	exit;
}

$chapter = novel_chapter_exists($bid, $cid);
if(!$chapter) {
	readfile($_SYSTEM['VIP']['buy_picture']);
	exit;
}


$buy = $db->selectOne(array(
	'field' => 'id',
	'from'  => __TAB_SHOP__,
	'where' => array(
		'book_id' => $bid,
		'chapter_id' => $cid,
		'user_id' => $Global['F_uid'],
	),
));

if(!$buy) {
	$acp = $db->selectOne(array(
		'field' => 'id',
		'from'  => __TAB_USER_ACP__,
		'where' => array(
			'uid' => $Global['F_uid'],
			'isfounder'=>1,
		),
	));
	if(!$acp) {
		$author = $db->selectOne(array(
			'field' => 'id',
			'from'  => __TAB_MY_AUT__,
			'where' => array(
				'uid' => $Global['F_uid'],
			),
		));
		if($author['id'] != $book['author_id']) {
			readfile($_SYSTEM['VIP']['buy_picture']);
			exit;
		}
	}
}


ImageVIP(novel_content(ROOT.novel_url($book, $chapter['priority'].'_'.$chapter['hash'])));


