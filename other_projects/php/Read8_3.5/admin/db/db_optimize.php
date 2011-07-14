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
if(!defined('IN_Read8') || !defined('IN_Read8_ACP') || !defined('IN_Read8_ACP_DB') || !isset($PHP_SELF)) {
	exit('Access Denied (db_optimize.php)');
}

if($action == 'optimize') {
	if(empty($tbl_read8) || !is_array($tbl_read8))
		back('请选择要优化的数据表');

	$db->execUpBySql('REPAIR TABLE `'.implode('`, `', $tbl_read8).'`');

	jumpage("?ptype=$ptype&pfile=$pfile", '数据库优化成功');
}



$read8_tables = $db->selectBySql('SHOW TABLE STATUS WHERE Name LIKE \''.$_SYSTEM['db_prefix'].'%\'');
$other_tables = $db->selectBySql('SHOW TABLE STATUS WHERE Name NOT LIKE \''.$_SYSTEM['db_prefix'].'%\'');
foreach($read8_tables as $k => $v) {
	$read8_tables[$k]['Data_length']  = sizecount($v['Data_length'],  true);
	$read8_tables[$k]['Index_length'] = sizecount($v['Index_length'], true);
	$read8_tables[$k]['Data_free']    = sizecount($v['Data_free'],    true);
}
foreach($other_tables as $k => $v) {
	$other_tables[$k]['Data_length']  = sizecount($v['Data_length'],  true);
	$other_tables[$k]['Index_length'] = sizecount($v['Index_length'], true);
	$other_tables[$k]['Data_free']    = sizecount($v['Data_free'],    true);
}

//显示
$tpl = new AOKOTpl($pfile);
$tpl->assign(array(
	'db_prefix' => $_SYSTEM['db_prefix'],

	'read8_tables' => $read8_tables,
	'other_tables' => $other_tables,
));
$tpl->output();
