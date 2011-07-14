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

$bid = 0;
$page = 1;
$update = 0;

define('IN_Read8', true);
define('IN_Read8_TPWT', true);
require_once '../include/init.php';

if(!defined('PAGE_SIZE')) {
	define('PAGE_SIZE', 20);
}

$typepriv = $db->selectOne(array(
	'from'  => __TAB_USER_TPWT__,
	'where' => array(
		'uid' => $Global['F_uid'],
	),
));
if(!$typepriv) {
	$acp = $db->selectOne(array(
		'field' => 'id',
		'from'  => __TAB_USER_ACP__,
		'where' => array(
			'uid'       => $Global['F_uid'],
			'isfounder' => 1,
		),
	));
	if(!$acp) {
		back('您不是录入员');
	}else {
		$typepriv = array(
			'uid'          => $Global['F_uid'],
			'novel_add'    => 1,
			'novel_edit'   => 1,
			'novel_del'    => -1,
			'chapter_add'  => 1,
			'chapter_edit' => 1,
			'chapter_del'  => -1,
			'overstep'     => 1,
		);
	}
}


if(!in_array($action, array('', 'novel_add', 'novel_edit', 'novel_del', 'chapter_add', 'chapter_edit', 'chapter_del')))
	back('非法操作');

if($bid) {
	$book = html_show(get_novel_by_id($bid), false);

	if(!$book)
		back('未找到该书信息');

	if(!$typepriv['overstep'] && $Global['F_uid'] != $book['typewriter_id'])
		back('您没有该书的管理权限');

	if($cid) {
		$chapter = html_show(novel_chapter_exists($bid, $cid), false);
		if(!$chapter)
			back('未找到该章节信息');

		$priority = novel_get_chapter_priority_by_id($bid, $cid);
	}
}


if($action) {
	if(!$typepriv[$action])
		back('您没有足够的权限执行该操作');

	include _config('author');
	include _config('upload');

	$format = array();
	$formats = sprintf('%04b', $_SYSTEM['UPLOAD']['format']);
	if($formats[3]) {
		$format[] = 'png';
	}
	if($formats[2]) {
		$format[] = 'gif';
	}
	if($formats[1]) {
		$format[] = 'jpg';
		$format[] = 'jpeg';
	}
	if($formats[0]) {
		$format[] = 'bmp';
	}

	require_once ROOT.'typewriter/'.$action.'.php';
}else {
	$sql = array(
		'from'  => __TAB_NOVEL__,
		'order' => 'dateline DESC',
		'filter'=> 'convert_novel_classes',
		'limit' => PAGE_SIZE,
		'page'  => $page,
		'counter'=>1,
	);
	if(!$typepriv['overstep']) {
		$sql['where']['typewriter_id'] = $Global['F_uid'];
	}

	_header(array('作品管理'));

	$_assign['typepriv'] = $typepriv;

	$tpl = new AOKOTpl('typewriter');
	$tpl->assign(array(
		'itemlist' => html_show($db->select($sql)),
		'pagelink' => page($db->counter, PAGE_SIZE),
	));
	$tpl->output();

	unset($_assign['typepriv']);

	_footer();
}

