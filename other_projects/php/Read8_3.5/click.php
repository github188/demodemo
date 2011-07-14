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
	点击
*/

$ajax = $bid = '';

define('IN_Read8', true);
require_once str_replace(array('\\', '/'.basename(__FILE__)), array('/', ''), __FILE__).'/include/init.php';

$db->single(__TAB_NOVEL_EXT__);

if($ajax) {
	header('Content-Type: application/json; charset='.SYSCHARSET);
	//if(!$bid || !ris_int($bid) || !novel_exists($bid))
		//exit("alert('该书不存在');");

	$bookclick = _cookie_decode("bookclick$bid");

	if(!$bookclick || TIMESTAMP - $bookclick > 60) {
		_cookie_encode("bookclick$bid", TIMESTAMP, 60);//每1分统计一次
		$db->execUpBySql("UPDATE ".__TAB_NOVEL_EXT__." SET click_today = click_today + 1 WHERE book_id = $bid");
		//exit("alert('投票成功，谢谢您的支持');");
	}
}
