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
//书库首页

$nc = $sc = 0;

define('IN_Read8', true);
require_once str_replace(array('\\', '/'.basename(__FILE__)), array('/', ''), __FILE__).'/include/init.php';


if($nc) {
	$_assign['nc'] = $nc;

	$tpl = new AOKOTpl('novel_index');


	$attributes = array('subject', 'content');
	foreach($attributes as $attribute) {
		$tmp = array();
		foreach($_CATEGORY['NOVEL'][$attribute] as $v) {
			$v['selected'] = $nc == $v['id'] ? ' selected="selected"' : '';
			$tmp[] = $v;
		}

		$tpl->assign($attribute.'s', $tmp);
	}

	$tmp = array();
	for($i = 65; $i < 91; $i++) {
		$tmp[] = array(
			'v' => strtoupper(chr($i)),
		);
	}
	for($i = 0; $i <= 9; $i++) {
		$tmp[] = array(
			'v' => $i,
		);
	}

	$tpl->assign('initial', $tmp);

	$tmp = array();
	for($i = 2; $i <= 20; $i++) {
		$tmp[] = array(
			'v' => $i,
		);
	}

	$tpl->assign('words', $tmp);

	$tpl->assign('nc_title',   $_CATEGORY['NOVEL']['subject'][$nc-1]['title']);

	$seokeyword = array($_CATEGORY['NOVEL']['subject'][$nc-1]['title'].'频道');

	foreach($_RANK['TIMES'] as $i) {
		foreach($_RANK['TYPES'] as $j) {
			$tpl->assign('novel_'.$i.'_'.$j, _book_rank($i, $j, 0, false));
			$tpl->assign('story_'.$i.'_'.$j, _book_rank($i, $j, 0, true));
		}
	}
	foreach($_RANK['COMDS'] as $i => $j) {
		$tpl->assign('novel_'.$i, _commend_rank($i, 0, false));
		$tpl->assign('story_'.$i, _commend_rank($i, 0, true));
	}

	$tpl->assign('novels', _novel($nc));
}elseif($sc) {
	$_assign['sc'] = $sc;

	$tpl = new AOKOTpl('story_index');


	$attributes = array('subject', 'content');
	foreach($attributes as $attribute) {
		$tmp = array();
		foreach($_CATEGORY['STORY'][$attribute] as $v) {
			$v['selected'] = $nc == $v['id'] ? ' selected="selected"' : '';
			$tmp[] = $v;
		}

		$tpl->assign($attribute.'s', $tmp);
	}

	$tpl->assign('sc_title',   $_CATEGORY['STORY']['subject'][$sc-1]['title']);

	$seokeyword = array($_CATEGORY['STORY']['subject'][$sc-1]['title'].'频道');

	foreach($_RANK['TIMES'] as $i) {
		foreach($_RANK['TYPES'] as $j) {
			$tpl->assign('novel_'.$i.'_'.$j, _book_rank($i, $j, 0, false));
			$tpl->assign('story_'.$i.'_'.$j, _book_rank($i, $j, 0, true));
		}
	}
	foreach($_RANK['COMDS'] as $i => $j) {
		$tpl->assign('novel_'.$i, _commend_rank($i, 0, false));
		$tpl->assign('story_'.$i, _commend_rank($i, 0, true));
	}

	$tpl->assign('stories', _story($sc));
}else {
	$tpl = new AOKOTpl('index');

	include _cache('ads');
	$tpl->assign($_CACHE['ads']);

	//首页公告
	include _cache('announce');
	$tpl->assign('Announceblock', $_CACHE['announce']);
	unset($_CACHE['announce']);

	foreach($_RANK['TIMES'] as $i) {
		foreach($_RANK['TYPES'] as $j) {
			$tpl->assign('novel_'.$i.'_'.$j, _book_rank($i, $j, 0, false));
			$tpl->assign('story_'.$i.'_'.$j, _book_rank($i, $j, 0, true));
		}
	}
	foreach($_RANK['COMDS'] as $i => $j) {
		$tpl->assign('novel_'.$i, _commend_rank($i, 0, false));
		$tpl->assign('story_'.$i, _commend_rank($i, 0, true));
	}

	$tpl->assign('friendlinks', $db->select(array(
		'field' => 'name, url, logo',
		'from'  => __TAB_LINKS__,
	)));

	include _cache('interview');
	include _cache('justnow');
	include _cache('focus');

	include _cache('cache');
	foreach(array_keys($_CACHE['cache']) as $v) {
		include _cache_custom($v);
		$tpl->assign('custom_'.$v, $_CACHE['custom_'.$v]);
	}

	$tpl->assign($_CACHE['focus']);
	$tpl->assign(array(
		'interview'  => $_CACHE['interview'],
		'justnow'    => $_CACHE['justnow'],
		'lastupdate' => _novel(0, 0, 0),
		'lastfinish' => _novel(0, 0, 1),
	));

	$seokeyword = array();
}


_header($seokeyword);//输出头部

$tpl->output();

_footer();
