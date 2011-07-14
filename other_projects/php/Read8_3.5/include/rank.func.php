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
	exit('Access Denied (rank.func.php)');
}

/*
	排行榜
*/


include _config('rank');


function book_rank($time, $type, $sc, $story, $limit){
	return array_slice(_book_rank($time, $type, $sc, $story), 0, $limit);
}

function _book_rank($time, $type, $sc, $story, $cacheTime = 86400, $prefix = 'rank_'){
	global $_RANK;

	$f = ROOT.'data/cache/'.$prefix.($story ? 'story' : 'novel').'_'.$time.'_'.$type.'_'.$sc.'.php';

	if(!is_file($f) || $cacheTime && TIMESTAMP - filemtime($f) > $cacheTime)
		_book_rank_update($time, $type, $sc, $story);

	include $f;
	return $_RANK[($story ? 'story' : 'novel').'_'.$time.'_'.$type.'_'.$sc];
}

function _book_rank_update($time, $type, $sc, $story, $prefix = 'rank_'){
	$f = ROOT.'data/cache/'.$prefix.($story ? 'story' : 'novel').'_'.$time.'_'.$type.'_'.$sc.'.php';

	$cacheData = _book_rank_generate($time, $type, $sc, $story);

	_cache_write($f, ($story ? 'story' : 'novel').'_'.$time.'_'.$type.'_'.$sc, $cacheData, $prefix);
}

function _book_rank_generate($time, $type, $sc, $story){
	global $db, $_RANK, $_SYSTEM;

	if(!in_array($time, $_RANK['TIMES']) || !in_array($type, $_RANK['TYPES']))
		return false;

	$sql = array(
		'field' => 'n.title,
					n.id AS bid,
					n.author,
					n.subject,
					n.dateline,
					n.lastupdate,
					ne.click_week,
					ne.click_month,
					ne.click_total,
					ne.vote_week,
					ne.vote_month,
					ne.vote_total,
					ne.rate_week,
					ne.rate_month,
					ne.rate_total,
					ne.favo_week,
					ne.favo_month,
					ne.favo_total,
					ne.commend_week,
					ne.commend_month,
					ne.commend_total,
					ne.digg_week,
					ne.digg_month,
					ne.digg_total,
					ne.bury_week,
					ne.bury_month,
					ne.bury_total,'
					.($story ? 'n.description' : 'n.author_id, n.intro, n.path, n.pinyin'),
		'from'  => $db->table($story ? __TAB_STORY__ : __TAB_NOVEL__, 'n'),
		'join'  => array(
			array('table' => $db->table($story ? __TAB_STORY_EXT__ : __TAB_NOVEL_EXT__, 'ne'), 'on' => 'n.id = ne.book_id'),
		),
		'order' => 'ne.'.$type.'_'.$time.' DESC, n.lastupdate DESC',
		'limit' => $_SYSTEM['RANK'][$type.'_'.$time.'_'.($story ? 'story' : 'novel')],
	);

	if($sc && ris_int($sc))
		$sql['where']['n.subject'] = $sc;

	$sql['filter'] = '_book_rank_callback';

	global $db;

	return  '/* SQL: '.HT.' '.var_export($sql, true).' */'.LF.
			LF.
			'$_RANK[\''.($story ? 'story' : 'novel').'_'.$time.'_'.$type.'_'.$sc.'\'] = '.var_export($db->select($sql), true).';';
}

function _book_rank_callback($book){
	if(isset($book['path'])) {
		$book['InfoURL']    = novel_url($book);
		$book['HTMLURL']    = novel_url($book, 0);
		$book['AuthorURL']  = 'author.php?aid='.$book['author_id'];
		return convert_novel_classes(html_show($book, false));
	}else {
		$book['URL']        = story_url($book);
		return convert_story_classes(html_show($book, false));
	}
}


function commend_rank($comd, $sc, $story, $limit){
	return array_slice(_commend_rank($comd, $sc, $story), 0, $limit);
}

function _commend_rank($comd, $sc, $story, $cacheTime = 86400, $prefix = 'rank_'){
	global $_RANK;

	$f = ROOT.'data/cache/'.$prefix.($story ? 'story' : 'novel').'_'.$comd.'_'.$sc.'.php';

	if(!is_file($f) || $cacheTime && TIMESTAMP - filemtime($f) > $cacheTime)
		_commend_rank_update($comd, $sc, $story);

	include $f;
	return $_RANK[($story ? 'story' : 'novel').'_'.$comd.'_'.$sc];
}

function _commend_rank_update($comd, $sc, $story, $prefix = 'rank_'){
	$f = ROOT.'data/cache/'.$prefix.($story ? 'story' : 'novel').'_'.$comd.'_'.$sc.'.php';

	$cacheData = _commend_rank_generate($comd, $sc, $story);

	_cache_write($f, ($story ? 'story' : 'novel').'_'.$comd.'_'.$sc, $cacheData, $prefix);
}

function _commend_rank_generate($comd, $sc, $story){
	global $db, $_RANK, $_SYSTEM;

	if(!isset($_RANK['COMDS'][$comd]))
		return false;

	$sql = array(
		'field' => 'n.title,
					n.id AS bid,
					n.author,
					n.subject,
					n.dateline,
					n.lastupdate,
					ne.click_week,
					ne.click_month,
					ne.click_total,
					ne.vote_week,
					ne.vote_month,
					ne.vote_total,
					ne.rate_week,
					ne.rate_month,
					ne.rate_total,
					ne.favo_week,
					ne.favo_month,
					ne.favo_total,
					ne.commend_week,
					ne.commend_month,
					ne.commend_total,
					ne.digg_week,
					ne.digg_month,
					ne.digg_total,
					ne.bury_week,
					ne.bury_month,
					ne.bury_total,'
					.($story ? 'n.description' : 'n.author_id, n.intro, n.path, n.pinyin'),
		'from'  => $db->table($story ? __TAB_STORY_COMMEND__ : __TAB_NOVEL_COMMEND__, 'nd'),
		'join'  => array(
			array('table' => $db->table($story ? __TAB_STORY__     : __TAB_NOVEL__,     'n'), 'on' => 'n.id = nd.book_id'),
			array('table' => $db->table($story ? __TAB_STORY_EXT__ : __TAB_NOVEL_EXT__, 'ne'), 'on' => 'nd.book_id = ne.book_id'),
		),
		'order' => 'nd.dateline DESC, n.lastupdate DESC',
		'limit' => $_SYSTEM['RANK'][($story ? 'story' : 'novel').'_'.$comd],
	);

	if($sc && ris_int($sc))
		$sql['where']['n.subject'] = $sc;

	$sql['filter'] = '_commend_rank_callback';

	global $db;

	return  '/* SQL: '.HT.' '.var_export($sql, true).' */'.LF.
			LF.
			'$_RANK[\''.($story ? 'story' : 'novel').'_'.$comd.'_'.$sc.'\'] = '.var_export($db->select($sql), true).';';
}

function _commend_rank_callback($book){
	if(isset($book['path'])) {
		$book['InfoURL']    = novel_url($book);
		$book['HTMLURL']    = novel_url($book, 0);
		$book['AuthorURL']  = 'author.php?aid='.$book['author_id'];
		return convert_novel_classes(html_show($book, false));
	}else {
		$book['URL']        = story_url($book);
		return convert_story_classes(html_show($book, false));
	}
}

