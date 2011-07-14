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
	exit('Access Denied (db_import.php)');
}


if($action == 'disable') {
	if(!$sqlrun)
		back('已禁止运行 SQL');

	if(!unlink(SITE_SAFE_SQL))
		back('删除安全文件 '.$sqlsff.' 失败，请从ftp手动删除');

	jumpage("?ptype=$ptype&pfile=$pfile", '禁止 SQL 运行成功');
}elseif($action == 'import') {
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
}elseif($action == 'delete') {
	if(empty($unlinkfile) || !is_array($unlinkfile))
		back('请选择要删除的文件');

	foreach($unlinkfile as $filename) {
		if(rfileext($filename) != 'sql')
			continue;

		if(!preg_match('/^[A-Za-z0-9_]+$/', substr($filename, 0, -4)))
			continue;

		unlink(ROOT.'data/backup/'.$filename);
	}

	jumpage("?ptype=$ptype&pfile=$pfile", '删除成功');
}


$files = array();
foreach(rlistfile(ROOT.'data/backup') as $v) {
	if(rfileext($v) == 'sql') {
		$i = explode(',', base64_decode(rsubstr(rfor(ROOT.'data/backup/'.$v), '# Identify:    ', LF)));
		$files[] = array(
			'filename' => $v,
			'filesize' => sizecount(filesize(ROOT.'data/backup/'.$v)),
			'version'  => $i[1],
			'dateline' => $i[2],
			'tables'   => $i[0],
			'sqlrun'   => $sqlrun,
		);
	}
}


$tpl = new AOKOTpl($pfile);
$tpl->assign(array(
	'files' => $files,

	'sqlrun' => $sqlrun,
	'sqlsff' => $sqlsff,
));
$tpl->output();

