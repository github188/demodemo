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
//»º´æº¯Êý¿â


if(!defined('IN_Read8')) {
	exit('Access Denied (cache.func.php)');
}


function _cache_write($cacheFile, $cacheName, $cacheData = '', $prefix = ''){
	return rfow($cacheFile, '<'.'?php'.LF.
							'/**'.LF.
							'*'.LF.
							'*  Copyright:  (c) 2004 - 2008 [AOKOSOFT] Read8'.LF.
							'*  Web Site:   http://www.aokosoft.com http://www.read8.net'.LF.
							'*  Powered by: [AOKOSOFT] Read8 Group'.LF.
							'*  Support:    http://bbs.read8.net'.LF.
							'*  License:    http://www.read8.net/license'.LF.
							'*'.LF.
							'*/'.LF.
							'if(!defined(\'IN_Read8\')) {'.LF.
							HT.'exit(\'Access Denied ('.$prefix.$cacheName.'.php)\');'.LF.
							'}'.LF.
							LF.
							$cacheData);
}


function _cache($cacheName, $cacheTime = 86400, $prefix = 'cache_'){
	$f = ROOT.'data/cache/'.$prefix.$cacheName.'.php';

	if(!is_file($f) || $cacheTime && TIMESTAMP - filemtime($f) > $cacheTime) {
		_cache_update($cacheName, '', $prefix);
	}

	return $f;
}

function _cache_update($cacheName = '', $cacheData = '', $prefix = 'cache_'){
	if($cacheName) {
		$caches = array($cacheName);
	}else {
		$caches = array('announce', 'cache', 'focus', 'justnow', 'rss', 'ads', 'api');
	}

	foreach($caches as $cacheName) {
		$f = ROOT.'data/cache/'.$prefix.$cacheName.'.php';

		if(!$cacheData)
			$cacheData = _cache_generate($cacheName);

		_cache_write($f, $cacheName, $cacheData, $prefix);
	}
}

function _cache_generate($cacheName){
	global $db;

	switch($cacheName) {
		case 'announce':
			global $_SYSTEM;

			return '$_CACHE[\''.$cacheName.'\'] = '.var_export($db->select(array(
				'field' => 'id, title, content, dateline',
				'from'  => __TAB_ANNOUNCE__,
				'order' => 'id DESC',
				'limit' => $_SYSTEM['DISPLAY']['announce'],
			)), true).';';
			break;
		case 'cache':
			$rows = $db->select(array(
				'from'  => __TAB_CACHE__,
				'where' => array(
					'active' => 1,
				),
			));
			$cache = array();
			foreach($rows as $row) {
				$cache[$row['id']] = $row;
			}
			return '$_CACHE[\''.$cacheName.'\'] = '.var_export($cache, true).';';
			break;
		/*case 'indextab':
			global $_CATEGORY;

			$index = array();
			foreach($_CATEGORY['NOVEL']['IDX']['subject'] as $k => $v) {
				$index[] = array(
					'subject' => $v,
					'fc'      => commend_rank(RANK_FACE, $k, false, 2),
				);
			}
			return '$_CACHE[\''.$cacheName.'\'] = '.var_export(html_show($index), true).';';
			break;
		case 'indexcab':
			global $_CATEGORY;

			$index = array();
			foreach($_CATEGORY['NOVEL']['IDX']['subject'] as $k => $v) {
				$index[] = array(
					'subject' => $v,
					'fc'      => commend_rank(RANK_FACE, $k, false, 4),
				);
			}
			return '$_CACHE[\''.$cacheName.'\'] = '.var_export(html_show($index), true).';';
			break;
		case 'indexhab':
			global $_CATEGORY;

			$index = array();
			foreach($_CATEGORY['STORY']['IDX']['subject'] as $k => $v) {
				$index[] = array(
					'subject' => $v,
					'fc'      => commend_rank(RANK_FACE, $k, true, 3),
				);
			}
			return '$_CACHE[\''.$cacheName.'\'] = '.var_export(html_show($index), true).';';
			break;*/
		case 'focus':
			$rows = $db->select(array(
				'field' => 'url, description, link',
				'from'  => __TAB_FOCUS__,
				'where' => array(
					'active' => 1,
				),
				'order' => 'dateline DESC',
				'limit' => 5,
			));
			$pics = $links = $texts = array();
			foreach($rows as $row) {
				$pics[]  = $row['url'];
				$links[] = $row['link'];
				$texts[] = $row['description'];
			}
			$focus['pics']  = implode('|', $pics);
			$focus['links'] = implode('|', $links);
			$focus['texts'] = implode('|', $texts);
			return '$_CACHE[\''.$cacheName.'\'] = '.var_export($focus, true).';';
			break;
		case 'interview':
			require_once ROOT.'include/author.func.php';
			$interview = author_interview();
			$interview['content'] = rmb_substr(html_show_crlf_decode($interview['content']), 0, 200);
			return '$_CACHE[\''.$cacheName.'\'] = '.var_export($interview, true).';';
			break;
		case 'justnow':
			$data = $db->select(array(
				'field' => 'uid, username, book_id, book_title, dateline, module, action',
				'from'  => __TAB_JUSTNOW__,
				'order' => 'dateline DESC',
				'limit' => 5,
				'filter'=> '_justnow_format',
			));
			return '$_CACHE[\''.$cacheName.'\'] = '.var_export($data, true).';';
			break;
		case 'rss':
			return '$_CACHE[\''.$cacheName.'\'] = '.var_export(html_show($db->select(array(
				'field' => 'id AS bid, title, author, intro, dateline, lastupdate',
				'from'  => __TAB_NOVEL__,
				'order' => 'dateline DESC',
				'limit' => 20,
			))), true).';';
			break;
		case 'ads':
			$ads = $db->select(array(
				'from'  => __TAB_AD__,
			));
			$advertise = array();
			foreach($ads as $ad) {
				$advertise['CAD_'.$ad['pos']] = $ad['ad'];
			}
			return '$_CACHE[\''.$cacheName.'\'] = '.var_export($advertise, true).';';
			break;
		case 'api':
			$apis = $db->select(array(
				'field' => 'address, code, charset, gate + 0 AS gate',
				'from'  => __TAB_API__,
				'where' => array(
					'active' => 1,
				),
			));
			foreach($apis as $k => $api) {
				$apis[$k]['gate'] = $api['gate'] == 1 ? 'Dz' : 'Pw';
			}
			return '$_CACHE[\''.$cacheName.'\'] = '.var_export($apis, true).';';
			break;
	}

	return '';
}


function _cache_custom($cacheName, $cacheTime = 86400, $prefix = 'cache_custom_'){
	$f = ROOT.'data/cache/'.$prefix.$cacheName.'.php';

	if(!is_file($f) || $cacheTime && TIMESTAMP - filemtime($f) > $cacheTime) {
		_cache_custom_update($cacheName, '', $prefix);
	}

	return $f;
}

function _cache_custom_update($cacheName = '', $cacheData = '', $prefix = 'cache_custom_'){
	if($cacheName) {
		$caches = array($cacheName);
	}else {
		include _cache('cache');
		$caches = array_keys($_CACHE['cache']);
	}

	foreach($caches as $cacheName) {
		$f = ROOT.'data/cache/'.$prefix.$cacheName.'.php';

		if(!$cacheData)
			$cacheData = _cache_custom_generate($cacheName);

		_cache_write($f, $cacheName, $cacheData, $prefix);
	}
}

function _cache_custom_generate($cacheName){
	global $db, $_SYSTEM;

	include _cache('cache');
	if(!isset($_CACHE['cache'][$cacheName]))
		return '';

	$sql = str_replace('{db_prefix}', $_SYSTEM['db_prefix'], $_CACHE['cache'][$cacheName]['sql']);
	if(false !== stripos($sql, 'insert') || false !== stripos($sql, 'update') || false !== stripos($sql, 'delete') || false !== stripos($sql, 'create') || false !== stripos($sql, 'drop') || false !== stripos($sql, 'truncate')) {
		$sql = '';
	}
	$data = $db->selectBySql($sql, 'convert_custom_classes');
	if($_CACHE['cache'][$cacheName]['twod']) {
		$cacheData = array();
		$with  = explode(',', $_CACHE['cache'][$cacheName]['with']);
		$group = $_CACHE['cache'][$cacheName]['group'];
		foreach($data as $row) {
			foreach($with as $v) {
				if(!$v)
					continue;

				$cacheData[$row[$group]][$v] = $row[$v];
			}
			if(!isset($cacheData[$row[$group]]['custom__data']))
				$cacheData[$row[$group]]['custom__data'] = array();

			$cacheData[$row[$group]]['custom__data'][] = $row;
		}
		$data = array();
		foreach($cacheData as $val) {
			$data[] = $val;
		}
	}

	return '$_CACHE[\'custom_'.$cacheName.'\'] = '.var_export($data, true).';';
}
