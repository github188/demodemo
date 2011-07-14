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
if(!defined('IN_Read8') || !defined('IN_Read8_ACP') || !defined('IN_Read8_ACP_STYLE') || !isset($PHP_SELF)) {
	exit('Access Denied (style_cache.php)');
}



$db->single(__TAB_CACHE__);

_clear_cache('cache_custom');

if($action == 'insert') {
	if(empty($cache))
		back('缓存标识不能为空');

	if(!preg_match('/^[A-Za-z0-9]+$/', $cache))
		back('缓存标识只能使用字母和数字');

	if($db->selectOne(array(
		'field' => 'id',
		'where' => array(
			'id' => $cache,
		),
	)))
		back('缓存标识不能重复');

	$db->insert(array(
		'id'    => $cache,
		'title' => $title,
		'sql'   => '',
	));
	header('Location: ?ptype='.$ptype.'&pfile='.$pfile.'&action=edit&id='.$cache);
	exit;
}elseif($action == 'edit') {
	$cache = $db->selectOne(array(
		'where' => array(
			'id' => $id,
		),
	));
	if(!$cache)
		back('未知缓存');

	$tpl = new AOKOTpl($pfile.'_edit');
	$tpl->assign($cache);
	$tpl->output();
}elseif($action == 'update') {
	$db->update(array(
		'title' => $title,
		'sql'   => $customsql,
		'twod'  => $twod,
		'group' => $customgroup,
		'with'  => $customwith,
	), array(
		'id'    => $id,
	));
	_cache_update('cache');
	jumpage("?ptype=$ptype&pfile=$pfile", '编辑成功');
}elseif($action == 'lock') {
	$db->execUpBySql('UPDATE '.__TAB_CACHE__.' SET active = (active + 1) % 2 WHERE id = \''.$id.'\'');
	_cache_update('cache');
	jumpage("?ptype=$ptype&pfile=$pfile", '状态修改成功');
}elseif($action == 'delete') {
	$db->delete(array(
		'id' => $id,
	));
	_cache_update('cache');
	jumpage("?ptype=$ptype&pfile=$pfile", '删除成功');
}else {
	$caches = $db->select(array(
		'order' => 'id DESC',
	));


	$tpl = new AOKOTpl($pfile);
	$tpl->assign('caches', $caches);
	$tpl->output();
}


