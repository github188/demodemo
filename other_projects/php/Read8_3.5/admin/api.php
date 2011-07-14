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
	exit('Access Denied (api.php)');
}

$db->single(__TAB_API__);


if($action == 'charset') {
	if(!$id || !ris_int($id))
		back('非法操作');

	$db->execUpBySql('UPDATE '.__TAB_API__.' SET `charset` = `charset` % 3 + 1 WHERE id = '.$id);
	jumpage("?ptype=$ptype&pfile=$pfile", '修改成功');
}elseif($action == 'gate') {
	if(!$id || !ris_int($id))
		back('非法操作');

	$db->execUpBySql('UPDATE '.__TAB_API__.' SET gate = gate % 2 + 1 WHERE id = '.$id);
	jumpage("?ptype=$ptype&pfile=$pfile", '修改成功');
}elseif($action == 'lock') {
	if(!$id || !ris_int($id))
		back('非法操作');

	$db->execUpBySql('UPDATE '.__TAB_API__.' SET active = (active + 1) % 2 WHERE id = '.$id);
	jumpage("?ptype=$ptype&pfile=$pfile", '修改成功');
}elseif($action == 'edit') {
	if(!$id || !ris_int($id))
		back('非法操作');

	$tpl = new AOKOTpl('api_edit');
	$tpl->assign($db->selectOne(array(
		'where' => array(
			'id' => $id,
		),
	)));
	$tpl->output();
	exit;
}elseif($action == 'update') {
	if(!$id || !ris_int($id))
		back('非法操作');

	$db->update(array(
		'address' => $address,
		'code'    => $code,
	), array(
		'id'      => $id,
	));
	jumpage("?ptype=$ptype&pfile=$pfile", '修改成功');
}elseif($action == 'delete') {
	if(!$id || !ris_int($id))
		back('非法操作');

	$db->delete(array(
		'id' => $id,
	));
	jumpage("?ptype=$ptype&pfile=$pfile", '删除成功');
}

if(!empty($update)) {
	if(!empty($api)) {
		if($api != 'general' && $api != 'ucenter')
			$api = 'none';

		$db->update(array(
			'value'   => $api,
		), array(
			'config'  => 'SYSTEM',
			'`index`' => 'api',
		), __TAB_SYSTEM__);
		_config_update('system');
	}

	if(!empty($address)) {
		if(substr($address, 0, 7) != 'http://' && substr($address, 0, 8) != 'https://')
			back('地址需加"http://"');

		if(substr($address, -1) == '/')
			back('末尾不加"/"');

		$db->insert(array(
			'address' => $address,
		));
	}
	jumpage("?ptype=$ptype&pfile=$pfile", '修改成功');
}


$tpl = new AOKOTpl('api');

$tpl->assign(array(
	'api_none_c'    => $_SYSTEM['SYSTEM']['api'] == 'none'    ? ' checked="checked"' : '',
	'api_general_c' => $_SYSTEM['SYSTEM']['api'] == 'general' ? ' checked="checked"' : '',
	'api_ucenter_c' => $_SYSTEM['SYSTEM']['api'] == 'ucenter' ? ' checked="checked"' : '',
	'api_none_d'    => $_SYSTEM['SYSTEM']['api'] != 'none'    ? ' style="display: none;"' : '',
	'api_general_d' => $_SYSTEM['SYSTEM']['api'] != 'general' ? ' style="display: none;"' : '',
	'api_ucenter_d' => $_SYSTEM['SYSTEM']['api'] != 'ucenter' ? ' style="display: none;"' : '',

	'apis' => $db->select(),
));

$tpl->output();