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
if(!defined('IN_Read8') || !defined('IN_UCP')) {
	exit('Access Denied (category.inc.php)');
}

/*
	用户中心导航
*/

require_once ROOT.'include/my.func.php';

$_SYSTEM['CATEGORY'] = array(
	'profile'  => '基本信息',
	'story'    => '我的短篇',
	'favorite' => '我的收藏',
	'credit'   => '积分记录',
	'expend'   => '消费记录',
	'balance'  => '我的余额',
	'pay'      => '在线充值',
	'mobile'   => '手机短信',
	'comment'  => '我的评论',
	'pm'       => '站内短信',
	'help'     => '求助投诉',
	'author'   => '作家申请',
);

include _config('story');
if(!$_SYSTEM['STORY']['open']) {
	unset($_SYSTEM['CATEGORY']['story']);
}
include _config('pay');
include _config('mobile');
if(true || !$_SYSTEM['MOBILE']['username'] || !$_SYSTEM['MOBILE']['password']) {
	unset($_SYSTEM['CATEGORY']['mobile']);
}

$category = array();

foreach($_SYSTEM['CATEGORY'] as $key => $val) {
	$category[] = array(
		'f' => $key,
		't' => $val,
		'c' => PHP_SELF == $key,
	);
}