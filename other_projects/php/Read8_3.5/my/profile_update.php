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
	资料更新 - 用户中心
*/

define('IN_UCP', true);
define('PHP_SELF_EXT', '_update');
define('PHP_SELF', basename(__FILE__, PHP_SELF_EXT.'.php'));


$pswd = $newpswd = $newpswd2 = $email = $born = $comm = $privinfo = '';

require_once '../include/init.php';
require_once ROOT.'my/category.inc.php';


if($newpswd) {
	if($newpswd != $newpswd2) back('两次密码不一致');
	//旧密码是否正确
	if(!$row = $db->selectOne(array(
		'field' => 'uid',
		'from'  => __TAB_USER__,
		'where' => array(
			'uid' => $Global['F_uid'],
			'pwd' => md5(md5($pswd)),
		),
	))) back('原密码错');

	$sqladd = ",pwd = '".md5(md5($regnewpwd))."'";
}else {
	$sqladd = '';
}

if(Banlist($comm)) back('关键字');

//更新用户
$sql='UPDATE '.__TAB_USER__." SET email='$email'".$sqladd." WHERE uid='$Global[F_uid]'";
if(!$db->execUpBySql($sql)) back('profile_edit_failed');

header('Location: profile.php');
exit;
jumpage('my/profile.php','profile_edit_success');



_header(array($_SYSTEM['CATEGORY'][PHP_SELF]));

$tpl = new AOKOTpl('navigation_ucp');
$tpl->assign('categories', $category);
$tpl->output();


back($emsg);

_footer();