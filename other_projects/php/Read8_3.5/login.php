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
//登录程序

$loginuser = $loginpwd = $hash = '';
$forward = '';

define('IN_Read8', true);
require_once str_replace(array('\\', '/'.basename(__FILE__)), array('/', ''), __FILE__).'/include/init.php';
require_once ROOT.'include/api.func.php';

if(!$forward) {
	if(!empty($_SERVER['HTTP_REFERER'])) {
		$forward = $_SERVER['HTTP_REFERER'];
	}else {
		$forward = $_SYSTEM['SYSTEM']['SITE_ADDR'];
	}
}else {
	$forward = urldecode($forward);
}

/*
if($action=='viewstatus') {
	$tpl = new AOKOTpl('login');
	$tpl->assign(array(
		'action' => $action,
		'forward' => $forward,
		'C_loghashenable' => $Global['hashto']['login'],
	));
	$tpl->output();
	exit;
}
*/

//api跳转
if(!empty($asid) && is_numeric($step)) {
	$uri = ApiCache($asid);
	if(isset($uri[$step])) {
		header("Location: ".$uri[$step]);
	}else {
		unlink(ROOT."data/cache/api_$asid.php");
		header("Location: ".$uri['forward']);
	}
	exit;
}

//登录
if($action == 'login') {
	if(!_secode_check(1, $hash))
		back('hash_code_error');

	if(empty($loginuser) || empty($loginpwd))
		back('login_username_password_lack');

	$userarr = CheckPass($loginuser, md5($loginpwd));

	if(!is_array($userarr)) {
		//增加日志记录
		if(strlen($loginpwd)>4) $logpwd=substr($loginpwd,0,2)."...".substr($loginpwd,-2);
		else $logpwd=substr($loginpwd,0,1)."...".substr($loginpwd,-1);
		//AddLog($logpwd, 'password', $loginuser);
		back($userarr);
	}

	_cookie_encode('userinfo');
	_cookie_encode('lnfo', array($loginuser, md5($loginpwd)), $cookie_time);

	$db->execUpBySql("REPLACE INTO ".__TAB_SESS__." VALUES('$_SID', '$Global[F_userip]', '$userarr[uid]', '$loginuser', '".(TIMESTAMP+900)."', '')");

	if($_SYSTEM['SYSTEM']['api'] == 'general') {//api跳转
		include _cache('api');
		$asid = GetApiData($_CACHE['api'], 'login', 'ENCODE', $userarr['username'], $loginpwd, $userarr['email'], $cookie_time);
		$uri = ApiCache($asid);
		jumpage($uri[1], '登录成功');
	}else {
		jumpage($forward, '登录成功');
	}
}

//注销
if($action=="logoff") {
	_cookie_encode("userinfo");
	_cookie_encode("lnfo");
	$db->delete(array(
		'sid' => $sid,
	), __TAB_SESS__);

	if($_SYSTEM['SYSTEM']['api'] == 'general') {//api跳转
		include _cache('api');
		$asid = GetApiData($_CACHE['api'], $action);
		$uri = ApiCache($asid);
		jumpage($uri[1], '退出成功');
	}else {
		jumpage($forward, '退出成功');
	}
}

//显示登录
$seokeyword[]='login';
_header($seokeyword);//输出头部

$tpl = new AOKOTpl('login');
$tpl->assign(array(
	'action'  => $action,
	'forward' => $forward,
	'C_loghashenable' => _secode(1),
));

$tpl->output();

_footer();
