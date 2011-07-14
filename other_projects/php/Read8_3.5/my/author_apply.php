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
	作家申请 - 用户中心
*/

define('IN_UCP', true);
define('PHP_SELF_EXT', '_apply');
define('PHP_SELF', basename(__FILE__, PHP_SELF_EXT.'.php'));


$pseudonym = '';

require_once '../include/init.php';
require_once ROOT.'my/category.inc.php';

include _config('author');

$author = my_author();


if($author) {
	if($author['active'] > 0) {
		header('Location: ../author/profile.php');
	}else {
		header('Location: author.php');
	}
	exit;
}


if(!$pseudonym) {
	back('笔名不能为空');
}

$length = rmb_strlen($pseudonym);
if(!$length || $length > 12) {
	back('笔名最长六个汉字或十二个英文');
}

if(preg_match("/\\|%|&| |\'|\"|\/|\*|,|<|>|\r|\t|\n|#/im", $pseudonym)) {
	back('笔名中含有非法字符');
}


$db->insert(array(
	'pseudonym'    => $pseudonym,
	'pinyin'       => pinyin($pseudonym),
	'uid'          => $Global['F_uid'],
	'intro'        => '',
	'announcement' => '',
	'dateline'     => TIMESTAMP,
	'active'       => !$_SYSTEM['AUTHOR']['verify'],
), __TAB_MY_AUT__);


header('Location: author.php');
exit;