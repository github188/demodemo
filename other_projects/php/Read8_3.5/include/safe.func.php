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
if(!defined('IN_Read8')) {
	exit('Access Denied (safe.func.php)');
}

/*
	系统安全函数库
*/

require_once ROOT.'include/fso.func.php';

if(!defined('TIMESTAMP')) {
	define('TIMESTAMP', time());
}
if(!defined('LOG_PATH')) {
	define('LOG_PATH', 'data/logfiles/'.gmdate('Ym', TIMESTAMP));
}
if(!is_dir(LOG_PATH)) {
	rmkdir(LOG_PATH);
}
set_error_handler('_log_php');

function _log($type, $message){
	if(!in_array($type, array('system', 'rate', 'comment', 'commend', 'author', 'novel', 'collect', 'acp', 'php', 'mysql'))) {
		_log('system', '未定义日志类型 '.$type.', 消息: '.implode(HT, $message));
		return;
	}

	global $Global;
	$f = ROOT.LOG_PATH.'/'.gmdate('Ymd', TIMESTAMP).'_'.$type.'.php';
	rfoa($f, '<'.'?php exit; ?'.'>'.TIMESTAMP.HT.$Global['F_uid'].HT.$Global['F_username'].HT.$Global['F_userip'].HT.implode(HT, $message).LF);
}

function _log_php($errno, $errstr, $errfile = '', $errline = 0, $errcontext = array()){
	$errnos = array(
		1    => 'E_ERROR',
		2    => 'E_WARNING',
		4    => 'E_PARSE',
		8    => 'E_NOTICE',
		16   => 'E_CORE_ERROR',
		32   => 'E_CORE_WARNING',
		64   => 'E_COMPILE_ERROR',
		128  => 'E_COMPILE_WARNING',
		256  => 'E_USER_ERROR',
		512  => 'E_USER_WARNING',
		1024 => 'E_USER_NOTICE',
		6143 => 'E_ALL',
		2048 => 'E_STRICT',
		4096 => 'E_RECOVERABLE_ERROR',
	);
	$errfile = str_replace(array('\\', ROOT), array('/', ''), $errfile);
	_log('php', array(
		$errnos[$errno],
		$errstr,
		$errfile,
		$errline,
		//base64_encode(serialize($errcontext)),
	));
}


function _secode($bit){// 2 - register  1 - login  0 - comment
	global $_SYSTEM;

	$b = sprintf('%03b', $_SYSTEM['SAFE']['secode']);

	return $_SYSTEM['SAFE']['secode_length'] && $b[$bit];
}
function _secode_check($bit, $secode){
	global $_SID, $db;

	if(!_secode($bit))
		return true;

	if(!$secode)
		return false;

	if($row = $db->selectOne(array(
		'field' => 'secode',
		'from'  => __TAB_SESS__,
		'where' => array(
			'sid' => $_SID,
		),
	))) {
		$db->update(array(
			'secode' => '',
		), array(
			'sid'    => $_SID,
		), __TAB_SESS__);
	}
	if(!$row || !$row['secode'])
		return false;

	return strtolower($row['secode']) == strtolower($secode);
}
function _secode_generate(){
	global $_SYSTEM, $_SID, $db;

	$secode = rrand($_SYSTEM['SAFE']['secode_length'], 1);

	$db->update(array(
		'secode' => $secode,
	), array(
		'sid'    => $_SID,
	), __TAB_SESS__);

	$im = ImageCreate($_SYSTEM['SAFE']['secode_length'] * 9 + 3, 16);

	$bg = ImageColorAllocate($im, 236, 233, 216);
	$font_color=ImageColorAllocate($im, 0, 0, 0);
	$gray = ImageColorAllocate($im, 200, 200, 200);
	ImageFill($im, 0, 0, $gray);
	for($i = 0; $i < 300; $i++) {
		$randcolor = ImageColorallocate($im, rrand(3, 1, 255), rrand(3, 1, 255), rrand(3, 1, 255));
		ImageSetPixel($im, rrand(2, 1), rrand(2, 1, 29), $randcolor);
	}
	ImageString($im, 5, 2, 0, $secode, $font_color);
	header('Content-type: image/png');
	ImagePNG($im);
	ImageDestroy($im);
}