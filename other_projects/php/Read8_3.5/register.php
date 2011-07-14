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
//注册程序
//ok for rd8

$action = $forward = $eula = '';
$rd8user = $rd8password = $rd8password2 = $rd8mail = $rd8idcard = $rd8protect = $rd8answer = $rd8secode = '';

define('IN_Read8', true);
require_once str_replace(array('\\', '/'.basename(__FILE__)), array('/', ''), __FILE__).'/include/init.php';
require_once ROOT.'include/api.func.php';

$js = '';

if(!$_SYSTEM['USER']['open']) {
	back('目前禁止注册');
}

$db->single(__TAB_USER__);

if('register' == $action) {
	if(!_secode_check(2, $rd8secode))
		back('验证码错误');

	$notNullFields = array(
		'rd8user',
		'rd8password',
		'rd8password2',
		'rd8mail',
		'rd8secode',
	);
	foreach($notNullFields as $v) {
		if(!$$v) {
			$js .= '';
		}
	}

	$len = rmb_strlen($rd8user);
	if($len < $_SYSTEM['USER']['minlength'] || $len > $_SYSTEM['USER']['maxlength'])
		back('用户名长度应介于 '.$_SYSTEM['USER']['minlength'].' - '.$_SYSTEM['USER']['maxlength'].' 字符之间');

	if(preg_match("/\\|%|&| |\'|\"|\/|\*|,|<|>|\r|\t|\n|#/im", $rd8user))
		back('用户名中含有非法字符');

	if(!preg_match("/^\w[\w\.\-\+]*@\w[\w\-]+(\.\w+)+$/", $rd8mail))
		back('请正确输入邮箱');

	//用户存在与否
	if($db->selectOne(array(
		'field' => 'uid',
		'where' => array('username' => $rd8user),
	)))
		back('该用户名已被注册');

	//封禁关键词
	if(Banlist($rd8user, 'name'))
		back('该用户名禁止注册');

	if(!$rd8password)
		back('请填写密码');

	if($rd8password != $rd8password2)
		back('两次输入的密码不一致');

	$uid = $db->insert(array(
		'username' => $rd8user,
		'pwd'      => md5($rd8password),
		'email'    => $rd8mail,
		'dateline' => TIMESTAMP,
		'active'   => !$_SYSTEM['USER']['verify'],
	));
	if(!$uid)
		back('注册数据写入失败');

	//登录部分
	_cookie_encode("lnfo", array($rd8user, md5($rd8password)), 3600);
	_cookie_encode("userinfo");

	$db->execUpBySql("REPLACE INTO ".__TAB_SESS__." VALUES('$_SID', '$Global[F_userip]', '$uid', '$rd8user', '".(TIMESTAMP+900)."', '')");

	if(!$forward) {
		$forward = $_SYSTEM['SYSTEM']['SITE_ADDR'];
	}else {
		$forward = urldecode($forward);
	}

	if($_SYSTEM['SYSTEM']['api'] == 'general') {//api跳转
		include _cache('api');
		$asid = GetApiData($_CACHE['api'], 'login', 'ENCODE', $rd8user, $rd8password, $rd8mail);
		$uri = ApiCache($asid);
		jumpage($uri[1], '注册成功');
	}else {
		jumpage($forward, '注册成功');
	}
}

include _config('info');

_header(array('注册'));

$tpl = new AOKOTpl('register');
$tpl->assign(array(
	'eula'      => !$eula ? show_text($_SYSTEM['INFO']['eula'], $_SYSTEM['INFO']['html']['eula']) : 0,
	'forward'   => $forward,
	'rd8user'   => $rd8user,
	'rd8mail'   => $rd8mail,
	'js'        => $js,
	'minlen'    => $_SYSTEM['USER']['minlength'],
	'maxlen'    => $_SYSTEM['USER']['maxlength'],
	'secode'    => _secode(2),
));
$tpl->output();

_footer();