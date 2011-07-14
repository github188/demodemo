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
	exit('Access Denied (db_update.php)');
}


if($action == 'disable') {
	if(!$sqlrun)
		back('已禁止运行 SQL');

	if(!unlink(SITE_SAFE_SQL))
		back('删除安全文件 '.$sqlsff.' 失败，请从ftp手动删除');

	jumpage("?ptype=$ptype&pfile=$pfile", '禁止 SQL 运行成功');
}elseif($action == 'update') {
	if(!$sqlrun)
		back('当前不可运行 SQL 语句，请从ftp创建 '.$sqlsff.' 文件');

	if(!empty($queries))
		$queries = trim($queries);

	if(empty($queries))
		back('请输入要运行的 SQL 语句');

	$sqls = split_sql(stripslashes($queries));
	foreach($sqls as $sql) {
		$db->execUpBySql($sql);
		if($db->errno)
			back('升级过程中发生错误<br />'.$sql.'<br />'.$db->error());
	}

	jumpage("?ptype=$ptype&pfile=$pfile", '运行成功');
}




$tpl = new AOKOTpl($pfile);
$tpl->assign(array(
	'queries' => empty($queries) ? '' : html_show($queries, false),

	'sqlrun' => $sqlrun,
	'sqlsff' => $sqlsff,
));
$tpl->output();

