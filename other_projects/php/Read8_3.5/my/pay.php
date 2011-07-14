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
	充值 - 用户中心
*/

define('IN_UCP', true);
define('PHP_SELF', basename(__FILE__, '.php'));


$price = 0;

require_once '../include/init.php';
require_once ROOT.'my/category.inc.php';

_header(array($_SYSTEM['CATEGORY'][PHP_SELF]));

$tpl = new AOKOTpl('navigation_ucp');
$tpl->assign('categories', $category);
$tpl->output();

$prs = array(
	10,
	20,
	50,
	100,
);
if($price && in_array($price, $prs)) {
	require_once ROOT.'include/pay.func.php';

	$subject = $_SYSTEM['SYSTEM']['SITE_NAME'].'充值 ('.$price.'元)';
	$vmoney = $price * $_SYSTEM['PAY']['rate'];
	$description = $price.'元充'.$vmoney.$_SYSTEM['PAY']['unit'].' '.$_SYSTEM['SYSTEM']['SITE_NAME'].'('.$_SYSTEM['SYSTEM']['SITE_ADDR'].')';
	$pid = $db->insert(array(
		'uid'      => $Global['F_uid'],
		'price'    => $price,
		'vmoney'   => $vmoney,
		'dateline' => TIMESTAMP,
	), __TAB_PAY__);
	$tradeno = 'RD8'.gmdate('YmdHis', TIMESTAMP).$pid;
	header('Location: '.payGateAli($subject, $tradeno, $price, $description, $_PAY['ALI']['URL'], true));
	exit;
}

$p = array();
foreach($prs as $v) {
	$p[] = array(
		'k' => $v,
		'v' => $v * $_SYSTEM['PAY']['rate'],
		't' => $_SYSTEM['PAY']['unit'],
	);
}

$tpl = new AOKOTpl('my_'.PHP_SELF);
$tpl->assign('pays', $p);
$tpl->assign('records', $db->select(array(
	'from'  => __TAB_PAY__,
	'where' => array(
		'uid' => $Global['F_uid'],
	),
)));
$tpl->output();

_footer();