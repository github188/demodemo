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
	评分
*/

$obno = true;
$ajax = $bid = $score = '';

define('IN_Read8', true);
require_once str_replace(array('\\', '/'.basename(__FILE__)), array('/', ''), __FILE__).'/include/init.php';

$db->single(__TAB_NOVEL_RATE__);

if($ajax) {
	header('Content-Type: application/json; charset='.SYSCHARSET);
	if(!$bid || !ris_int($bid) || !novel_exists($bid))
		exit('alert(\'该书不存在\');');

	if($score > 5 || $score < 1)
		exit('alert(\'只能评 1 - 5 分\');');

	$rate = _cookie_decode('rate'.$bid);

	if(!$rate || TIMESTAMP - $rate > 86400) {
		_cookie_encode('rate'.$bid, TIMESTAMP, 86400);//每天只能一次
		$db->insert(array(
			'book_id' => $bid,
			'rate'    => $score,
		));
		$r = $db->selectOne(array(
			'field' => 'AVG(rate) AS rate',
			'where' => array(
				'book_id' => $bid,
			),
		));
		$db->update(array(
			'rate_total' => $r['rate'],
		), array(
			'book_id'    => $bid,
		), __TAB_NOVEL_EXT__);
		exit('alert(\'评分成功，谢谢您的支持\');');
	}else {
		exit('alert(\'请勿重复评分\');');
	}
}
