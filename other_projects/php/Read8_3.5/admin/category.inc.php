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
//后台类别管理


if(!defined('IN_Read8') || !defined('IN_Read8_ACP') || !isset($PHP_SELF)) {
	exit('Access Denied (category.inc.php)');
}


list($pf, $ps) = explode('_', $pfile);

$db->single($ps == 'novel' ? __TAB_CLASS_NOVEL__ : __TAB_CLASS_STORY__);


if(empty($class)) {
	$class = '';
}
if($action && $class != 'subject' && $class != 'content') {
	back('非法操作');
}

switch($action) {
	case 'delete':
		if(empty($delete_id) || !ris_int($delete_id))
			back('非法操作');

		$db->update(array(
			'active'   => 0,
			'deltime'  => TIMESTAMP,
		), array(
			'id'       => $delete_id,
			'active'   => 1,
			'category' => $class,
		));
		_clear_cache('class');
		jumpage("?ptype=$ptype&pfile=$pfile", '删除成功');
		break;
	case 'add':
		if(empty($title) || !trim($title))
			back('名称不能为空');

		$db->insert(array(
			'category' => $class,
			'title'    => trim($title),
		));
		_clear_cache('class');
		jumpage("?ptype=$ptype&pfile=$pfile", '添加成功');
		break;
	case 'edit':
		if(empty($edit_id) || !ris_int($edit_id))
			back('非法操作');

		if(empty($new_title) || !trim($new_title))
			back('名称不能为空');

		$db->update(array(
			'title'    => trim($new_title),
		), array(
			'id'       => $edit_id,
			'active'   => 1,
			'category' => $class,
		));
		_clear_cache('class');
		jumpage("?ptype=$ptype&pfile=$pfile", '编辑成功');
		break;
	case 'recover':
		if(empty($recover_id) || !ris_int($recover_id))
			back('非法操作');

		$db->update(array(
			'active'   => 1,
		), array(
			'id'       => $recover_id,
			'active'   => 0,
			'category' => $class,
		));
		_clear_cache('class');
		jumpage("?ptype=$ptype&pfile=$pfile", '恢复成功');
		break;
	case 'move':
		if(empty($move_from))
			$move_from = 0;

		if(!ris_int($move_from))
			back('非法操作');

		if(empty($move_to) || !ris_int($move_to))
			back('非法操作');

		if($move_from == $move_to)
			back('目标分类不能相同');

		$db->execUpBySql('UPDATE '.($ps == 'novel' ? __TAB_NOVEL__ : __TAB_STORY__).' SET '.$class.' = '.$move_to.' WHERE '.$class.($move_from ? ' = '.$move_from : ' NOT IN ('.implode(', ', array_keys($_CATEGORY[strtoupper($ps)]['IDX'][$class])).')'));
		_clear_cache('class');
		jumpage("?ptype=$ptype&pfile=$pfile", '移动成功');
		break;
}





//显示
$tpl = new AOKOTpl('category');
$tpl->assign(array(
	'subjects' => $db->select(array(
		'field' => 'id, title, active, deltime',
		'where' => array(
			'category' => 'subject',
		),
	)),
	'contents' => $db->select(array(
		'field' => 'id, title, active, deltime',
		'where' => array(
			'category' => 'content',
		),
	)),
));
$tpl->output();

