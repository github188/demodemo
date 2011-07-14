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
if(!defined('IN_Read8') || !defined('IN_Read8_ACP') || !defined('IN_Read8_ACP_USER') || !isset($PHP_SELF)) {
	exit('Access Denied (user_add.php)');
}



if($action == 'update') {
	if(empty($acp_add_user)) {
		$acp_add_user = '';
	}
	if(empty($acp_add_user_pswd)) {
		$acp_add_user_pswd = '';
	}
	if(empty($acp_add_user_pswd2)) {
		$acp_add_user_pswd2 = '';
	}
	if(empty($acp_add_user_mail)) {
		$acp_add_user_mail = '';
	}

	$emsg = array();

	$len = rmb_strlen($acp_add_user);
	if($len < $_SYSTEM['USER']['minlength'] || $len > $_SYSTEM['USER']['maxlength'])
		$emsg['user'] = '用户名长度应介于 '.$_SYSTEM['USER']['minlength'].' - '.$_SYSTEM['USER']['maxlength'].' 字符之间';

	if(preg_match("/\\|%|&| |\'|\"|\/|\*|,|<|>|\r|\t|\n|#/im", $acp_add_user))
		$emsg['user'] = '用户名中含有非法字符';

	if($db->selectOne(array(
		'field' => 'uid',
		'where' => array(
			'username' => $acp_add_user,
		),
	))) {
		$emsg['user'] = '用户已存在';
	}

	if(!preg_match("/^\w[\w\.\-\+]*@\w[\w\-]+(\.\w+)+$/", $acp_add_user_mail))
		$emsg['email'] = '请正确输入邮箱';

	if(!$acp_add_user_pswd)
		$emsg['pwd'] = '请输入密码';

	if($acp_add_user_pswd != $acp_add_user_pswd2)
		$emsg['pwd'] = '两次密码不一致';

	if(!$emsg) {
		$acp_add_user_pswd = md5($acp_add_user_pswd);
		$db->insert(array(
			'username' => $acp_add_user,
			'pwd'      => $acp_add_user_pswd,
			'email'    => $acp_add_user_mail,
			'dateline' => TIMESTAMP,
		));
		//AddLog("添加用户<$reguser>");
		jumpage("?ptype=$ptype&pfile=$pfile", '添加用户成功');
	}
}


$tpl = new AOKOTpl($pfile);

$tpl->assign('CE_email',isset($emsg['email']) ? $emsg['email'] : '');
$tpl->assign('CE_pwd',isset($emsg['pwd']) ? $emsg['pwd'] : '');
$tpl->assign('CE_user',isset($emsg['user']) ? $emsg['user'] : '');
$tpl->assign('C_regemail',empty($regemail) ? '' : $regemail);
$tpl->assign('C_regpwd',empty($regpwd) ? '' : $regpwd);
$tpl->assign('C_reguser',empty($reguser) ? '' : $reguser);

$tpl->output();

