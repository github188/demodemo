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
	exit('Access Denied (db_export.php)');
}

if($action == 'export') {
	if(empty($tbl_read8) || !is_array($tbl_read8))
		back('请选择要备份的数据表');

	if(empty($file) || !preg_match('/^[A-Za-z0-9_]+$/', $file))
		back('文件名只能用数字，英文字母和下划线');

	if(empty($tables) || !in_array($tables, array('full', 'novel', 'story', 'user', 'config', 'custom')))
		$tables = 'custom';

	if(empty($compress) || !in_array($compress, array('none', 'zip', 'gzip', 'bzip')))
		$compress = 'none';

	switch($compress) {
		case 'zip':
			if(function_exists('gzcompress')) {
				$file_ext = 'zip';
			}else {
				$compress = 'none';
				$file_ext = 'sql';
			}
			break;
		case 'gzip':
			if(function_exists('gzencode')) {
				$file_ext = 'tar.gz';
			}else {
				$compress = 'none';
				$file_ext = 'sql';
			}
			break;
		case 'bzip':
			if(function_exists('bzcompress')) {
				$file_ext = 'tar.bz2';
			}else {
				$compress = 'none';
				$file_ext = 'sql';
			}
			break;
		default :
			$compress = 'none';
			$file_ext = 'sql';
			break;
	}

	$f = ROOT.'data/backup/'.$file.'.sql';

	cookie('tables',   $tables,   31536000);
	cookie('compress', $compress, 31536000);

	$data = '# Read8.Net '.SYS_VERSION.' '.SYS_BUILD.' database '.$tables.' dump'.LF.
			'# Identify:    '.base64_encode($tables.','.SYS_VERSION.','.TIMESTAMP).LF.
			'# MySQL:       '.$db->ver().LF.
			'# PHP:         '.PHP_VERSION.LF.
			'# Export Date: '.rdate(TIMESTAMP).LF.
			'#'.LF.
			'# http://www.read8.net'.LF.
			'# Please visit our website for the latest information'.LF.
			'# ----------------------------------------------------------'.LF.
			LF.
			LF.
			LF;

	rfoa($f, $data);

	$length = rmb_strlen($data);
	foreach($tbl_read8 as $tbl) {
		$tbl_info = $db->selectBySql('SHOW CREATE TABLE '.$tbl);
		$sql =  LF.
				'-- --------------------------------------------------------'.LF.
				LF.
				'--'.LF.
				'-- `'.$tbl.'` 的结构'.LF.
				'--'.LF.
				LF.
				'DROP TABLE IF EXISTS `'.$tbl.'`;'.LF.
				$tbl_info[0]['Create Table'].';'.LF.
				LF.
				'--'.LF.
				'-- `'.$tbl.'` 的数据'.LF.
				'--'.LF.
				LF;
		$length += rmb_strlen($sql);
		rfoa($f, $sql);

		$db->debug = true;

		$page = 1;
		while(1) {
			set_time_limit(60);

			$data = $db->selectBySql($db->select(array(
				'from'  => $tbl,
				'limit' => 100,
				'page'  => $page,
				'counter'=>true,
			)));

			foreach($data as $v) {
				$sql = str_replace(array(CR, LF), array('\r', '\n'), $db->insert(caddslashes($v), $tbl)).';'.LF;
				$length += rmb_strlen($sql);
				rfoa($f, $sql);
			}

			$page++;
			if(ceil($db->counter / 100) < $page)
				break;
		}
	}

	if($compress != 'none') {
		require_once ROOT.'include/Archive.class.php';

		$archive_class = $compress.'_file';
		$archive = new $archive_class($file.'.'.$file_ext);
		$archive->set_options(array(
			'basedir' => ROOT.'data/backup',
		));
		$archive->add_files($file.'.sql');
		$archive->create_archive();
		unlink($f);
	}

	jumpage("?ptype=$ptype&pfile=$pfile", '数据库备份成功');
}


$export_tables = _cookie('tables');
if(!$export_tables) {
	$export_tables = 'full';
}
$compress = _cookie('compress');
switch($compress) {
	case 'zip':
		if(function_exists('gzcompress')) {
			$file_ext = 'zip';
		}else {
			$compress = 'none';
			$file_ext = 'sql';
		}
		break;
	case 'gzip':
		if(function_exists('gzencode')) {
			$file_ext = 'tar.gz';
		}else {
			$compress = 'none';
			$file_ext = 'sql';
		}
		break;
	case 'bzip':
		if(function_exists('bzcompress')) {
			$file_ext = 'tar.bz2';
		}else {
			$compress = 'none';
			$file_ext = 'sql';
		}
		break;
	default :
		$compress = 'none';
		$file_ext = 'sql';
		break;
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
	'export_tables' => $export_tables,
	'tables_full'   => $export_tables == 'full'   ? ' checked="checked"' : '',
	'tables_novel'  => $export_tables == 'novel'  ? ' checked="checked"' : '',
	'tables_story'  => $export_tables == 'story'  ? ' checked="checked"' : '',
	'tables_user'   => $export_tables == 'user'   ? ' checked="checked"' : '',
	'tables_config' => $export_tables == 'config' ? ' checked="checked"' : '',
	'tables_custom' => $export_tables == 'custom' ? ' checked="checked"' : '',

	'compress_none' => $compress == 'none' ? ' checked="checked"' : '',
	'compress_zip'  => function_exists('gzcompress') ? ($compress == 'zip'  ? ' checked="checked"' : '') : ' disabled="disabled"',
	'compress_gzip' => function_exists('gzencode')   ? ($compress == 'gzip' ? ' checked="checked"' : '') : ' disabled="disabled"',
	'compress_bzip' => function_exists('bzcompress') ? ($compress == 'bzip' ? ' checked="checked"' : '') : ' disabled="disabled"',

	'db_prefix' => $_SYSTEM['db_prefix'],

	'C_file'   => rdate(TIMESTAMP, 'ymd').'_'.rrand(8, 7),
	'file_ext' => $file_ext,

	'read8_tables' => $read8_tables,
	'other_tables' => $other_tables,
));
$tpl->output();
