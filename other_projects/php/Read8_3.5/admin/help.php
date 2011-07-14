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
if(!defined('IN_Read8') || !defined('IN_Read8_ACP') || !isset($PHP_SELF)) {
	exit('Access Denied (help.php)');
}

// 求助投诉



$db->single(__TAB_HELP__);


if($action == 'reply') {
	if(empty($id) || !ris_int($id))
		back('无此记录');

	$row = $db->selectOne(array(
		'field' => 'id, title, content, dateline',
		'where' => array(
			'id'     => $id,
			'active' => 1,
		),
	));

	if(!$row)
		back('无此记录');

	$tpl = new AOKOTpl('help');
	$tpl->assign($row);
	$tpl->output();
	exit;
}elseif($action == 'update') {
	if(empty($id) || !ris_int($id) || !$db->selectOne(array(
		'field' => 'id',
		'where' => array(
			'id' => $id,
		),
	))) {
		back('记录错误');
	}

	if(empty($reply))
		back('请填写回复内容');

	$db->update(array(
		'reply'     => $reply,
		'replytime' => TIMESTAMP,
	), array(
		'id'        => $id,
	));

	jumpage("?ptype=$ptype&pfile=$pfile", '回复成功');
}elseif($action == 'delete') {
	if(!empty($ivs))
		if(is_array($ivs))
			foreach($ivs as $k => $v)
				if(!$v || !ris_int($v))
					unset($ivs[$k]);

	if(!empty($ivs))
		$db->update(array(
			'active' => 0,
		), array(
			'id'     => $ivs,
		));

	jumpage("?ptype=$ptype&pfile=$pfile", '删除成功');
}



if(!defined('PAGE_SIZE')) {
	define('PAGE_SIZE', 20);
}

$sql = array(
	'field' => 'h.id, h.uid, h.title, h.content, h.dateline, h.reply, h.replytime, u.username',
	'from'  => $db->table(__TAB_HELP__, 'h'),
	'join'  => array('table' => $db->table(__TAB_USER__, 'u'), 'on' => 'h.uid = u.uid'),
	'order' => 'h.dateline DESC',
	'limit' => PAGE_SIZE,
	'page'  => $page,
	'filter'=> 'html_show',
	'counter'=>1,
);

if(empty($display)) {
	$sql['where'] = 'WHERE h.active = 1 AND h.replytime = 0';
}else {
	$sql['where'] = 'WHERE h.active = 1';
}

$tpl = new AOKOTpl($pfile);
$tpl->assign(array(
	'id'       => 0,
	'helps'    => $db->select($sql),
	'pagelink' => page($db->counter, PAGE_SIZE),
));
$tpl->output();
