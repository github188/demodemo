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
if(!defined('IN_Read8') || !defined('IN_Read8_ACP') || !defined('IN_Read8_ACP_AUT') || !isset($PHP_SELF)) {
	exit('Access Denied (author_add.php)');
}


if($action == 'update') {
	if(empty($author))
		back('请填写用户名');

	if(!($row = $db->selectOne(array(
		'field' => 'uid',
		'from'  => __TAB_USER__,
		'where' => array(
			'username' => $author,
		),
	))))
		back('未找到该用户');

	if($db->selectOne(array(
		'field' => 'id',
		'where' => array(
			'uid' => $row['uid'],
		),
	)))
		back('该用户已是作家');

	if(empty($pseudonym))
		back('请填写笔名');

	if($db->selectOne(array(
		'field' => 'id',
		'where' => array(
			'pseudonym' => $pseudonym,
		),
	)))
		back('该笔名已被使用');

	$db->insert(array(
		'pseudonym'    => $pseudonym,
		'pinyin'       => pinyin_unique(pinyin($pseudonym)),
		'uid'          => $row['uid'],
		'intro'        => '',
		'announcement' => '',
		'sample'       => '',
		'dateline'     => TIMESTAMP,
		'active'       => 1,
	));
	jumpage("?ptype=$ptype&pfile=$pfile", '添加成功');
}

$tpl = new AOKOTpl($pfile);
$tpl->output();
