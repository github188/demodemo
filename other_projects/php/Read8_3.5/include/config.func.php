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
//ÅäÖÃÉú³Éº¯Êý¿â

if(!defined('IN_Read8')) {
	exit('Access Denied (config.func.php)');
}


function _config($cacheName, $prefix = 'config_'){
	$f = ROOT.'data/cache/'.$prefix.$cacheName.'.php';

	if(!is_file($f))
		_config_update($cacheName, $prefix);

	return $f;
}

function _config_update($cacheName = '', $prefix = 'config_'){
	if($cacheName) {
		$caches = array($cacheName);
	}else {
		$caches = array('system', 'user', 'author', 'story', 'display', 'read', 'comment', 'upload', 'fopen', 'seo', 'safe', 'info', 'rank', 'official', 'paymonth', 'pay', 'vip', 'mobile');
	}

	foreach($caches as $cacheName) {
		$f = ROOT.'data/cache/'.$prefix.$cacheName.'.php';

		$cacheData = _config_generate($cacheName);

		_cache_write($f, $cacheName, $cacheData, $prefix);
	}
}

function _config_generate($cacheName){
	global $db;

	$data = $db->select(array(
		'field' => '`index`, value',
		'from'  => __TAB_SYSTEM__,
		'where' => array('config' => strtoupper($cacheName)),
	));

	$cacheData = array();
	foreach($data as $v)
		$cacheData[$v['index']] = $v['value'];

	switch($cacheName) {
		case 'info':
			$ks = array(
				'about',
				'guide',
				'contact',
				'pay',
                                'paymonth',
				'fee',
				'novel',
				'author',
				'eula',
			);
			$html = $cacheData['html'];
			unset($cacheData['html']);
			foreach(explode(',', $html) as $k => $v) {
				$cacheData['html'][$ks[$k]] = $v;
			}
			break;
	}

	return '$_SYSTEM[\''.strtoupper($cacheName).'\'] = '.var_export($cacheData, true).';';
}
