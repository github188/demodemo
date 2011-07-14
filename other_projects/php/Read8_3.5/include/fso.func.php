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
	exit('Access Denied (fso.func.php)');
}

/*
	文件系统函数库
*/



function author_url($author_id){
	global $Global;

	// TODO: 重写

	return 'author.php?aid='.$author_id;
}

function novel_url($book_info, $coid = false){
	global $_SYSTEM;

	$path = $_SYSTEM['SEO']['htmlpath'];

	if($_SYSTEM['SEO']['novel'] == 1) {
		if(false === $coid) {
			return $_SYSTEM['SYSTEM']['SITE_ADDR'].'/'.$book_info['pinyin'].'/';
		}else {
			return $_SYSTEM['SYSTEM']['SITE_ADDR'].'/'.$book_info['pinyin'].'/'.($coid ? $coid : 'list').'.html';
		}
	}elseif($_SYSTEM['SEO']['novel'] == 2) {
		$host = substr($_SYSTEM['SYSTEM']['SITE_ADDR'], 7);
		if(substr($host, 0, 4) == 'www.')
			$host = substr($host, 4);

		if(false === $coid) {
			return 'http://'.$book_info['pinyin'].'.'.$host.'/';
		}else {
			return 'http://'.$book_info['pinyin'].'.'.$host.'/'.($coid ? $coid : 'list').'.html';
		}
	}else {
		if(false === $coid) {
			return $_SYSTEM['SYSTEM']['SITE_ADDR'].'/novel.php?bid='.$book_info['bid'];
		}else {
			return $path.'/'.$book_info['path'].($coid ? '/'.$coid.'.html' : '');
		}
	}
}

function story_url($book_info){
	global $_SYSTEM;

	if(false && $rewrite) {
		return $_SYSTEM['SYSTEM']['SITE_ADDR'].'/story-'.$book_info['bid'].'.html';
	}else {
		return $_SYSTEM['SYSTEM']['SITE_ADDR'].'/story.php?bid='.$book_info['bid'];
	}
}


function _clear_cache($prefix = ''){
	if(!$prefix) {
		rrmdir(ROOT.'data/cache', true);
	}elseif($prefix == 'template') {
		rrmdir(ROOT.'data/cache/'.$prefix, true);
	}else {
		$fs = rlistfile(ROOT.'data/cache');
		$pl = strlen($prefix);
		foreach($fs as $f)
			if(substr($f, 0, $pl) == $prefix)
				unlink(ROOT.'data/cache/'.$f);
	}
}


function novel_chapter_rename($priority, $abspath, $waysw = true, $step = 1){//waysw : true为插入，false为删除
	if($priority <= 0)
		return false;

	$way = $waysw ? 1 * $step : -1 * $step;

	if($waysw && is_file($abspath.'/'.($priority + 1).'.html'))
		novel_chapter_rename($priority + 1, $abspath, $waysw, $step);

	r_mv($abspath.'/'.$priority.'.html', $abspath.'/'.($priority + $way).'.html');//重命名

	//内容修改
	$search  = array(
		($priority + 1).'.html',// next
		($priority - 1).'.html',// prev
	);
	$replace = array(
		($priority + $way + 1).'.html',
		($priority + $way - 1).'.html',
	);
	$filepath = $abspath.'/'.($priority + $way).'.html';
	rf_replace($search, $replace, $filepath);

	if(!$waysw && is_file($abspath.'/'.($priority + 1).'.html'))
		novel_chapter_rename($priority + 1, $abspath, $waysw, $step);
}


function rfileext($filename){
	$fextpos = strrpos($filename, '.');
	if(false === $fextpos)
		return '';
	return strtolower(substr($filename, $fextpos + 1));
}
function rfilename($filename, $fextonly = false){
	$fext = rfileext($filename);
	if($fextonly)
		return $fext;

	if(!rfilesafe($fext))
		$fext .= '_rd8attach';
	$filename = rdate(TIMESTAMP, 'Ymd').'_'.stringff(substr($GLOBALS['Global']['site_url'], 7), false).'_'.md5($filename.rrand(10));
	return array($filename, $fext);
}
function rfilesafe($fext, $safexts = '/^(php|phtml|php3|php4|jsp|do|asp|aspx|asa|asax|mspx|cgi|fcgi|pl|exe|dll|ocx|js|shtm|shtml|cer|cert|ht.+)$/i'){
	if(is_array($safexts))
		return in_array($fext, $safexts);
	return !preg_match($safexts, $fext);
}


function upload_convert($fileinfo){
	foreach($fileinfo['name'] as $k => $v) {
		$uploaded_files[$k]['name']     = $v;
		$uploaded_files[$k]['type']     = $fileinfo['type'][$k];
		$uploaded_files[$k]['size']     = $fileinfo['size'][$k];
		$uploaded_files[$k]['tmp_name'] = $fileinfo['tmp_name'][$k];
		$uploaded_files[$k]['error']    = $fileinfo['error'][$k];
	}
	return $uploaded_files;
}
function upload_status($status, $reportNoneUploads = false){
	switch($status) {
		case 0:
			return '';
			break;
		case 1:
			return 'upload_error_ini_size';
			break;
		case 2:
			return 'upload_error_form_size';
			break;
		case 3:
			return 'upload_error_partial';
			break;
		case 4:
			if($reportNoneUploads)
				return 'upload_error_no_file';
			return '';
			break;
		case 6:
			return 'upload_error_no_tmp_dir';
			break;
		case 7:
			return 'upload_error_cant_write';
			break;
		default :
			return 'upload_error_unknown';
	}
}
function ris_uploaded_file($filename){
	return is_uploaded_file($filename) || is_uploaded_file(str_replace('\\\\', '\\', $filename));//windows compatible
}
function rmove_uploaded_file($filename, $destination){
	if(move_uploaded_file($filename, $destination))
		return true;//move first

	if(!ris_uploaded_file($filename))
		return false;//check before

	return r_mv($filename, $destination);
}

function r_cp($f, $d){
	if(copy($f, $d) || 0 === filesize($f) && is_file($d) && 0 === filesize($d)) {//copy with zero test on winnt
		rchmod($d);
		return true;
	}else {
		return false;
	}
}
function r_mv($filename, $destination){
	if(is_file($destination))
		unlink($destination);

	if(rename($filename, $destination)) {
		if(is_file($filename))
			rchmod($destination);

		return true;//manual move
	}

	if(!r_cp($filename, $destination)) {//copy with zero test on winnt
		if(!is_readable($filename))
			return false;//final try

		$content = rfor($filename);
		if(false === $content)
			return false;

		return rfow($destination, $content);
	}else {
		unlink($filename);
		return true;
	}
}

function rgzr($f, $s = 0, $l = false){
	$fp = gzopen($f, 'rb');
	if(false === $fp)
		return false;

	$c = '';
	while(!feof($fp))
		$c .= gzread($fp, 1024);

	gzclose($fp);
	return $c;
}

function rforr($f, $emulate = true){
	global $_SYSTEM;

	include _config('fopen');

	if(!$emulate || !$_SYSTEM['FOPEN']['emulate'])
		return rfor($f);

	require_once ROOT.'include/HTTPRequest.class.php';

	$http = new HTTPRequest($f);

	if($_SYSTEM['FOPEN']['curl'])
		$http->agent = 'CURL';
	else
		$http->agent = 'SOCKET';

	if($_SYSTEM['FOPEN']['agent'])
		$http->UserAgent = $_SYSTEM['FOPEN']['agent'];

	if($_SYSTEM['FOPEN']['cookie'])
		$http->cookiefile = false === strpos($_SYSTEM['FOPEN']['cookie'], ROOT) ? ROOT.$_SYSTEM['FOPEN']['cookie'] : $_SYSTEM['FOPEN']['cookie'];

	if($_SYSTEM['FOPEN']['proxy'])
		$http->UseProxy($_SYSTEM['FOPEN']['proxy']);

	$http->timeout = 120;

	if(false === $http->Connect())
		return 'Can\'t connect to server';

	$content = $http->Request();

	//empty($content) && $content = 'An error has occured while reading URL' && $Global['urlreaddebug'] && $content .= ': '.$url;
	return $content;
}

function rfor($f, $s = 0, $l = false){
	if(function_exists('file_get_contents')) {
		return file_get_contents($f);
	}else {
		$fp = fopen($f, 'rb');
		if(false === $fp)
			return false;

		$c = '';
		while(!feof($fp))
			$c .= fread($fp, 1024);

		fclose($fp);
		return $c;
	}
}
function rfow($f, $d, $m = false){
	$length = rfile_put_contents($f, $d, 'w');
	if(false === $length)
		return false;

	return rchmod($f, $m);
}
function rfoa($f, $d, $m = false){
	$length = rfile_put_contents($f, $d, 'a');
	if(false === $length)
		return false;

	return rchmod($f, $m);
}
function rfox($f, $d, $m = false){
	$length = rfile_put_contents($f, $d, 'x');
	if(false === $length)
		return false;

	return rchmod($f, $m);
}
function rf_replace($search, $replace, $filepath){
	rfow($filepath, str_replace($search, $replace, rfor($filepath)));
}

function rfile_put_contents($filename, $data, $mode = 'w'){//mode b always on
	switch($mode) {
		case 'w':
			if(function_exists('file_put_contents'))
				return file_put_contents($filename, $data);
		case 'a':
		case 'x':
			$fp = fopen($filename, $mode.'b');
			if(false === $fp)
				return false;
			$length = fwrite($fp, $data);
			fclose($fp);
			return $length;
	}
}

function rchmod($r, $m = false, $strict = false){
	if(false === $m)
		$m = $GLOBALS['_SYSTEM']['SAFE']['chmod_file'];

	if(!file_exists($r)) {
		trigger_error($r.' NOT exists', E_USER_NOTICE);
		return !$strict;
	}

	if(!ris_int($m) || $m <= 0 || $m > 511) {
		trigger_error('Invalid dec mode '.$m.'.', E_USER_NOTICE);
		return !$strict;
	}

	if(!chmod($r, $m)) {
		trigger_error('Can\'t change '.$r.' to mode 0'.decoct($m).', permission denied.', E_USER_NOTICE);
		return !$strict;
	}

	return true;
}

/*function rdirname($path){
	$paths = explode('/', str_replace(array('\\\\', '\\'), array('\\', '/'), $path));
	$ptnum = count($paths);
	if($ptnum > 1)
		unset($paths[$ptnum - 1]);

	if(!$paths[0] && $ptnum == 2)
		return '/';

	return implode('/', $paths);
}*/

function rlistdir($pathname, $subfolder = 0, $ignore = array()){
	$dirlist = rreaddir($pathname, 2, $ignore);

	if($subfolder <= 0) return $dirlist;

	foreach($dirlist as $dir) {
		$subfolders = rlistdir($pathname.'/'.$dir, $subfolder - 1, $ignore);
		foreach($subfolders as $subdir) {
			$dirlist[] = $dir.'/'.$subdir;
		}
	}
	return $dirlist;
}
function rlistfile($pathname, $subfolder = 0, $ignore = array()){
	$filelist = rreaddir($pathname, 1, $ignore);

	if($subfolder <= 0) return $filelist;

	$dirlist = rlistdir($pathname);
	foreach($dirlist as $dir) {
		$subfolderfiles = rlistfile($pathname.'/'.$dir, $subfolder - 1, $ignore);
		foreach($subfolderfiles as $subfile) {
			$filelist[] = $dir.'/'.$subfile;
		}
	}
	return $filelist;
}
function rmkdir($pathname, $mode = false){
	if(false === $mode)
		$mode = $GLOBALS['_SYSTEM']['SAFE']['chmod_dir'];

	$pathname = rtrim(str_replace('\\', '/', $pathname), '/');
	if(!$pathname || file_exists(ROOT.$pathname))
		return is_dir(ROOT.$pathname);

	$mark = true;
	if(false !== strpos($pathname, '/')) {
		$pathnames = explode('/', $pathname);
		unset($pathnames[count($pathnames) - 1]);
		$mark = $mark & rmkdir(implode('/', $pathnames), $mode);
		//$mark = $mark & rmkdir(implode('/', explode('/', $pathname, -1)), $mode); PHP 5 Only
	}

	$tmark = mkdir(ROOT.$pathname, $mode);
	rchmod(ROOT.$pathname, $mode);
	if(!$tmark)
		trigger_error('Can\'t create folder '.$pathname.', permission denied.', E_USER_WARNING);

	return $mark & $tmark;
}
function rreaddir($pathname, $mode = 1, $ignore = array()){//1 - file 2 - folder
	$ignore = array_merge(array('.', '..'), $ignore);
	$dh = opendir($pathname);
	if(false === $dh)
		return false;

	$dirlist = array();
	while(false !== ($filename = readdir($dh))) {
		if(in_array($filename, $ignore))
			continue;
		if($mode == 1 && !is_file($pathname.'/'.$filename))
			continue;
		if($mode == 2 && !is_dir($pathname.'/'.$filename))
			continue;
		$dirlist[] = $filename;
	}
	closedir($dh);
	return $dirlist;
}
function rrmdir($dirname, $leavedir = false, $igfile = array(), $igdir = array()){
	foreach(rlistfile($dirname, 0, $igfile) as $filename) {
		unlink($dirname.'/'.$filename);
	}
	foreach(rlistdir($dirname, 0, $igdir) as $subdirname) {
		rrmdir($dirname.'/'.$subdirname, $leavedir, $igfile, $igdir);
	}
	if(!$leavedir) rmdir($dirname);
}
