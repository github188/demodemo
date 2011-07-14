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


$bid = $cid = $buyID = 0;

session_start();

define('IN_Read8', true);
require_once str_replace(array('\\', '/'.basename(__FILE__)), array('/', ''), __FILE__).'/include/init.php';
require_once ROOT.'include/Cart.class.php';

include _config('vip');

$cart = new Cart;


if($buyID) {
	$cart->rmFromCart($buyID);
	jumpage('shop.php', '删除成功');
}


$book = get_novel_by_id($bid);
if(!$book) {
	back('未找到该书信息');
}

$chapter = novel_chapter_exists($bid, $cid);
if(!$chapter) {
	back('未找到该章节信息');
}


$cart->addToCart($bid.'_'.$cid, $chapter, round($chapter['words'] / 1000) * $_SYSTEM['VIP']['price']);
$cart->saveChange();


jumpage('shop.php', '该章节已加入购物车');
