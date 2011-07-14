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
/*
	更新 - 作家中心
*/

define('IN_AUCP', true);
define('PHP_SELF_EXT', '_chapter');
define('PHP_SELF', basename(__FILE__, PHP_SELF_EXT.'.php'));


$bid = '';

require_once '../include/init.php';
require_once ROOT.'author/category.inc.php';
require_once ROOT.'include/editor.func.php';
include _config('upload');

_header(array($_SYSTEM['CATEGORY'][PHP_SELF]));

$tpl = new AOKOTpl('navigation_aucp');
$tpl->assign('categories', $category);
$tpl->output();

$tpl = new AOKOTpl('author_'.PHP_SELF.PHP_SELF_EXT);

$book = get_novel_by_id($bid);
if(!$book) {
	back('无此书信息');
}
if($book['author_id'] != $_AUTHOR['id']) {
	back('您无权管理此书');
}
if($book['state'] > 4) {
	back('此书正在审核');
}
if($book['state'] == 2) {
	back('此书暂停更新');
}
if($book['state'] == 3) {
	back('此书已完结');
}

$tpl->assign(html_show($book, false));

/*$rows = $db->select(array(
	'field' => 'c.priority, c.title, v.title AS volume',
	'from'  => $db->table(__TAB_NOVEL_CHAPTER__, 'c'),
	'join'  => array('table' => $db->table(__TAB_NOVEL_VOLUME__, 'v'), 'on' => 'c.volume_id = v.id AND c.book_id = v.book_id'),
	'where' => array('c.book_id' => $bid),
	'order' => 'c.priority ASC',
));
$i = -1;
$volume = '';
$volumes = array();
foreach($rows as $v) {
	if($volume != $v['volume']) {
		$i++;
		$volume = $v['volume'];
		$volumes[$i] = array(
			'title'    => $volume,
			'chapters' => array(),
		);
	}
	$volumes[$i]['chapters'][] = array(
		'title'    => $v['title'],
		'priority' => $v['priority'],
	);
}*/


$tpl->assign(array(
	'bid'     => $bid,
	'editor'  => html_editor('article'),
	'upload'  => $_SYSTEM['UPLOAD']['open'] ? sizecount($_SYSTEM['UPLOAD']['size']) : 0,
	'formats' => implode(' ', $format),
	'chapters'  => $db->select(array(
		'field' => 'id, title, priority, dateline, words, locked, vip',
		'from'  => __TAB_NOVEL_CHAPTER__,
		'where' => array(
			'book_id' => $bid,
		),
		'order' => 'priority ASC',
	)),
	'vip'=>$_AUTHOR['vip'],
));

$tpl->output();

_footer();