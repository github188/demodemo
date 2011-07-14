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
if(!defined('IN_Read8')) {
	exit('Access Denied (date.func.php)');
}

/*
	时间日期函数库
*/

if(!defined('TIMESTAMP')) {
	define('TIMESTAMP', time());
}
if(!defined('DATE_FORMAT')) {
	define('DATE_FORMAT', 'D, d M Y H:i:s');
}

function rdate($timestamp = false, $format = 'Y-m-d H:i:s'){
	global $_SYSTEM;

	if(false === $timestamp) $timestamp = TIMESTAMP;
	return gmdate($format, $timestamp + $_SYSTEM['SYSTEM']['TIME_ZONE'] * 3600 + $_SYSTEM['SYSTEM']['TIME_DELAY']);
}
function rtodaytime($timestamp = false){
	global $_SYSTEM;

	$times = explode('-', rdate($timestamp, 'Y-m-d'));
	return gmmktime(0, 0, 0, $times[1], $times[2], $times[0]) - $_SYSTEM['SYSTEM']['TIME_ZONE'] * 3600 - $_SYSTEM['SYSTEM']['TIME_DELAY'];
}

function time_format($timestamp){
	$dist = TIMESTAMP - $timestamp;

	$s = '';

	if($dist >= 86400) {
		$d = floor($dist / 86400);
		$s .= ' '.$d .'天';
		$dist -= $d * 86400;
	}

	if($dist >= 3600) {
		$h = floor($dist / 3600);
		$s .= ' '.$h .'小时';
		$dist -= $h * 3600;
	}

	if($dist >= 60) {
		$m = floor($dist / 60);
		$s .= ' '.$m .'分钟';
		$dist -= $m * 60;
	}

	if($dist > 0) {
		$s .= ' '.$dist .'秒';
	}

	return $s.'前';
}

// NCT - novel comment
//     NEW - 发表
function _justnow($m, $a, $b = 0, $t = ''){
	global $db, $Global;

	$db->insert(array(
		'uid'        => $Global['F_uid'],
		'username'   => $Global['F_username'],
		'book_id'    => $b,
		'book_title' => $t,
		'dateline'   => TIMESTAMP,
		'module'     => $m,
		'action'     => $a,
	), __TAB_JUSTNOW__);

	_cache_update('justnow');
}
function _justnow_format($data){
	$data['text'] = _justnow_display($data['module'], $data['action'], $data['uid'], $data['username'], $data['book_id'], $data['book_title'], $data['dateline']);
	return $data;
}
function _justnow_display($m, $a, $u, $n, $b, $t, $d){// module, action, uid, username, bid, title, timestamp
	return sprintf(_justnow_format_table($m, $a), time_format($d), $n, novel_url(novel_exists($b)), $t);
}
function _justnow_format_table($m, $a){
	$format_table = array(
		'NCT' => array(
			'NEW' => '%s %s 对 <a href="%s">《%s》</a> 发表了评论',
		),
		'CLT' => array(
			'SIN' => '%s %s 完成单本采集 <a href="%s">《%s》</a>',
		),
	);
	return $format_table[$m][$a];
}