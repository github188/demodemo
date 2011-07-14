<?php


session_start();


$buy = 0;

define('IN_Read8', true);
require_once str_replace(array('\\', '/'.basename(__FILE__)), array('/', ''), __FILE__).'/include/init.php';
require_once ROOT.'include/Cart.class.php';


include _config('vip');

$cart = new Cart;


if($buy) {
	if(!$Global['F_uid']) {
		header('Location: login.php?forward=shop.php');
		exit;
	}
	$row = $db->selectOne(array(
		'field' => 'vmoney',
		'from'  => __TAB_USER__,
		'where' => array(
			'uid' => $Global['F_uid'],
		),
	));
	$first = '';
	foreach($cart->getCart() as $buyID => $v) {
		if($v['price'] > $row['vmoney'])
			break;

		$row['vmoney'] -= $v['price'];
		$db->update(array(
			'vmoney' => $row['vmoney'],
		), array(
			'uid' => $Global['F_uid'],
		), __TAB_USER__);
		if(!$first)
			$first = $buyID;

		list($bid, $cid) = explode('_', $buyID);
		$db->insert(array(
			'book_id' => $bid,
			'chapter_id' => $cid,
			'user_id' => $Global['F_uid'],
			'price' => $v['price'],
			'dateline' => TIMESTAMP,
		), __TAB_SHOP__);
		$cart->rmFromCart($buyID);
	}
	list($bid, $cid) = explode('_', $first);
	jumpage(novel_url(get_novel_by_id($bid), novel_get_chapter_priority_by_id($bid, $cid)), '购买成功');
}

_header(array('收银台'));

$tpl = new AOKOTpl('shop');
$tpl->assign('chapters', array_values($cart->getCart()));
$tpl->output();

_footer();
