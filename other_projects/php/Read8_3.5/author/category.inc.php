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
if(!defined('IN_Read8') || !defined('IN_AUCP')) {
	exit('Access Denied (category.inc.php)');
}

/*
	作家中心导航
*/

require_once ROOT.'include/my.func.php';

$_AUTHOR = my_author();

if(!$_AUTHOR) {
	header('Location: ../my/author.php');
	exit;
}

$_SYSTEM['CATEGORY'] = array(
	'profile'  => '基本信息',
	'column'   => '专栏设置',
	'new'      => '新建作品',
	'verify'   => '作品审核信息',
	'book'     => '管理作品',
	'commend'  => '作者推文',
	'ban'      => '书评禁言',
	'vip'      => 'VIP 管理',
	'balance'  => '收益记录',
	'withdraw' => '结算记录',
);


if(!isset($_SYSTEM['CATEGORY'][PHP_SELF])) {
	header('Location: profile.php');
	exit;
}


$category = array();

foreach($_SYSTEM['CATEGORY'] as $key => $val) {
	$category[] = array(
		'f' => $key,
		't' => $val,
		'c' => PHP_SELF == $key,
	);
}



include _config('author');
include _config('upload');

$format = array();
$formats = sprintf('%04b', $_SYSTEM['UPLOAD']['format']);
if($formats[3]) {
	$format[] = 'png';
}
if($formats[2]) {
	$format[] = 'gif';
}
if($formats[1]) {
	$format[] = 'jpg';
	$format[] = 'jpeg';
}
if($formats[0]) {
	$format[] = 'bmp';
}
