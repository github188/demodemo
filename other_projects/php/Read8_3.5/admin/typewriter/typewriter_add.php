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
if(!defined('IN_Read8') || !defined('IN_Read8_ACP') || !defined('IN_Read8_ACP_TYPEWRITER') || !isset($PHP_SELF)) {
	exit('Access Denied (typewriter_add.php)');
}


if($action == 'update') {
	if(empty($typewriter))
		back('请填写用户名');

	if(!($row = $db->selectOne(array(
		'field' => 'uid',
		'from'  => __TAB_USER__,
		'where' => array(
			'username' => $typewriter,
		),
	))))
		back('未找到该用户');

	if($db->selectOne(array(
		'field' => 'id',
		'where' => array(
			'uid' => $row['uid'],
		),
	)))
		back('该用户已是录入员');

	if($db->selectOne(array(
		'field' => 'id',
		'from'  => __TAB_USER_ACP__,
		'where' => array(
			'uid'       => $row['uid'],
			'isfounder' => 1,
		),
	)))
		back('站长已具有作品管理权限');

	if(empty($overstep))
		$overstep = 0;

	if(empty($novel_add))
		$novel_add = 0;

	if(empty($novel_edit))
		$novel_edit = 0;

	if(empty($novel_del))
		$novel_del = 0;

	if(empty($chapter_add))
		$chapter_add = 0;

	if(empty($chapter_edit))
		$chapter_edit = 0;

	if(empty($chapter_del))
		$chapter_del = 0;

	$db->insert(array(
		'uid'          => $row['uid'],
		'novel_add'    => $novel_add,
		'novel_edit'   => $novel_edit,
		'novel_del'    => $novel_del == 1 ? $novel_del_num : $novel_del,
		'chapter_add'  => $chapter_add,
		'chapter_edit' => $chapter_edit,
		'chapter_del'  => $chapter_del == 1 ? $chapter_del_num : $chapter_del,
		'overstep'     => $overstep,
	));
	jumpage("?ptype=$ptype&pfile=$pfile", '添加成功');
}

$tpl = new AOKOTpl($pfile);
$tpl->output();
