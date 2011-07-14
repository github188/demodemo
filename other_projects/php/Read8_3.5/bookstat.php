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
//书籍状态显示/书籍投票/书籍书签程序

$mbid = $mcid = $vbid = '';

require_once './include/init.php';

//设置点击一次等于几
$a = 1;

//添加书签
if($mbid && $mcid) {
	if(!CheckLogin(1)) exit(Lang('system_login_expired'));
	$mbid = intval($mbid);
	$mcid = intval($mcid);
	$msg = 'bookcase_bookcase_update_success';

	if(!$db->selectOne(array(
		'field' => 'id',
		'from'  => __TAB_BOOKCASE__,
		'where' => array(
			'uid'     => $Global['F_uid'],
			'book_id' => $mbid,
		),
	))) {//检查该书是否存在书架上
		$msg = 'bookstat_not_in_bookcase';
	}else {//更新书架
		$db->update(array(
			'chapter_orderid' => $mcid,
		), array(
			'uid'             => $Global['F_uid'],
			'book_id'         => $mbid,
		), __TAB_BOOKCASE__);
	}
	exit(Lang($msg));
}

//投票处理 ok
if($vbid) {
	$vbid = intval($vbid);
	$bookvote = _cookie_decode("bookvote$vbid");
	if(!$bookvote) {
		$db->execUpBySql("UPDATE ".__TAB_BOOKHITS__." SET `vote` = `vote` + 1 WHERE `book_id` = '$vbid'");
		_cookie_encode("bookvote$vbid", 1, 60*600);//每10小时统计一次
		$state = 'success';
	}else $state = 'repeat';
	exit(Lang($state, 'bookstat_vote_'));
}

//点击和投票统计显示
$bid = intval($bid);
$row = $db->selectOne(array(
	'field' => 'click, vote',
	'from'  => __TAB_BOOKHITS__,
	'where' => array('book_id' => $bid),
));
//更新点击数
$bookclick = _cookie_decode("bookclick$bid");
if(!$bookclick || TIMESTAMP - $bookclick > 60) {
	$db->execUpBySql("UPDATE ".__TAB_BOOKHITS__." SET `click` = `click` + $a WHERE `book_id` = '$bid'");
	_cookie_encode("bookclick$bid", TIMESTAMP, 60);//每1分统计一次
	$a *= 2;
}

echo "$('info').innerHTML = '".Lang('click').": ".($row['click']+$a)." &nbsp;".Lang('vote').": ".($row['vote']+1)."';";
