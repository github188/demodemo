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
//�鼮״̬��ʾ/�鼮ͶƱ/�鼮��ǩ����

$mbid = $mcid = $vbid = '';

require_once './include/init.php';

//���õ��һ�ε��ڼ�
$a = 1;

//�����ǩ
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
	))) {//�������Ƿ���������
		$msg = 'bookstat_not_in_bookcase';
	}else {//�������
		$db->update(array(
			'chapter_orderid' => $mcid,
		), array(
			'uid'             => $Global['F_uid'],
			'book_id'         => $mbid,
		), __TAB_BOOKCASE__);
	}
	exit(Lang($msg));
}

//ͶƱ���� ok
if($vbid) {
	$vbid = intval($vbid);
	$bookvote = _cookie_decode("bookvote$vbid");
	if(!$bookvote) {
		$db->execUpBySql("UPDATE ".__TAB_BOOKHITS__." SET `vote` = `vote` + 1 WHERE `book_id` = '$vbid'");
		_cookie_encode("bookvote$vbid", 1, 60*600);//ÿ10Сʱͳ��һ��
		$state = 'success';
	}else $state = 'repeat';
	exit(Lang($state, 'bookstat_vote_'));
}

//�����ͶƱͳ����ʾ
$bid = intval($bid);
$row = $db->selectOne(array(
	'field' => 'click, vote',
	'from'  => __TAB_BOOKHITS__,
	'where' => array('book_id' => $bid),
));
//���µ����
$bookclick = _cookie_decode("bookclick$bid");
if(!$bookclick || TIMESTAMP - $bookclick > 60) {
	$db->execUpBySql("UPDATE ".__TAB_BOOKHITS__." SET `click` = `click` + $a WHERE `book_id` = '$bid'");
	_cookie_encode("bookclick$bid", TIMESTAMP, 60);//ÿ1��ͳ��һ��
	$a *= 2;
}

echo "$('info').innerHTML = '".Lang('click').": ".($row['click']+$a)." &nbsp;".Lang('vote').": ".($row['vote']+1)."';";
