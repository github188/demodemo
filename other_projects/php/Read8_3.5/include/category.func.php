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
	exit('Access Denied (category.func.php)');
}

/*
	·ÖÀàº¯Êý¿â
*/


function _class($cacheName, $pos, $prefix = 'class_'){
	$f = ROOT.'data/cache/'.$prefix.$pos.'_'.$cacheName.'.php';

	if(!is_file($f))
		_class_update($pos, $cacheName, $prefix);

	return $f;
}

function _class_update($pos, $cacheName = '', $prefix = 'class_'){
	if($cacheName) {
		$caches = array($cacheName);
	}else {
		if('system' == $pos) {
			$caches = array('style', 'license', 'state');
		}elseif('novel' == $pos || 'story' == $pos) {
			$caches = array('subject', 'content');
		}else {
			$caches = array();
		}
	}

	foreach($caches as $cacheName) {
		$f = ROOT.'data/cache/'.$prefix.$pos.'_'.$cacheName.'.php';

		$cacheData = _class_generate($pos, $cacheName);

		_cache_write($f, $cacheName, $cacheData, $prefix.$pos.'_');
	}
}

function _class_generate($pos, $cacheName){
	global $db;

	if('system' == $pos) {
		$caches = array('style', 'license', 'state');
		$sql = array(
			'field' => 'id, title, description',
			'from'  => __TAB_CLASS_SYSTEM__,
			'where' => array(
				'category' => $cacheName,
			),
		);
	}elseif('novel' == $pos || 'story' == $pos) {
		$caches = array('subject', 'content');
		$sql = array(
			'field' => 'id, title, priority, dateline, active, deltime',
			'from'  => 'novel' == $pos ? __TAB_CLASS_NOVEL__ : __TAB_CLASS_STORY__,
			'where' => array(
				'category' => $cacheName,
				'active'   => 1,
			),
		);
	}else {
		$caches = array();
	}

	if(!in_array($cacheName, $caches))
		return '';

	$data = $db->select($sql);

	return  '$_CATEGORY[\''.strtoupper($pos).'\'][\''.$cacheName.'\'] = '.var_export($data, true).';'.LF.
			LF.
			'$_CATEGORY[\''.strtoupper($pos).'\'][\'IDX\'][\''.$cacheName.'\'] = '.var_export(_class_idx($data), true).';'.LF.
			LF.
			'$_CATEGORY[\''.strtoupper($pos).'\'][\'INDEX\'][\''.$cacheName.'\'] = '.var_export(_class_idx($data, true), true).';'.LF;
}

function _class_idx($data, $fulldata = false){
	foreach($data as $k => $v)
		$n[$v['id']] = $fulldata ? $v : $v['title'];

	return $n;
}


function convert_classes($book_info, $pos){
	global $_CATEGORY;

	if(!in_array($pos, array('system', 'novel', 'story')))
		return $book_info;

	foreach($_CATEGORY[strtoupper($pos)]['IDX'] as $k => $v)
		if(isset($book_info[$k]) && isset($v[$book_info[$k]]))
			$book_info[$k] = $v[$book_info[$k]];

	return $book_info;
}

function convert_system_classes($book_info){
	return convert_classes($book_info, 'system');
}

function convert_novel_classes($book_info){
	return convert_classes(convert_classes($book_info, 'system'), 'novel');
}

function convert_story_classes($book_info){
	return convert_classes(convert_classes($book_info, 'system'), 'story');
}

function convert_custom_classes($book_info){
	if(isset($book_info['bid'])) {
		$book_info['InfoURL'] = novel_url($book_info);
		$book_info['HTMLURL'] = novel_url($book_info, 0);
	}
	if(isset($book_info['path'])) {
		return convert_novel_classes($book_info);
	}else {
		$book_info['InfoURL'] = story_url($book_info);
		return convert_story_classes($book_info);
	}
}

