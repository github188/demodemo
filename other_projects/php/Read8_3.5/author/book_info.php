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
define('PHP_SELF_EXT', '_info');
define('PHP_SELF', basename(__FILE__, PHP_SELF_EXT.'.php'));


$bid = '';

require_once '../include/init.php';
require_once ROOT.'author/category.inc.php';

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


unset($_CATEGORY['state'][4]);
unset($_CATEGORY['state'][5]);

$attributes = array('subject', 'content');
foreach($attributes as $attribute) {
	$tmp = array();
	foreach($_CATEGORY['NOVEL'][$attribute] as $v) {
		$v['selected'] = $book[$attribute] == $v['id'] ? ' selected="selected"' : '';
		$tmp[] = $v;
	}

	$tpl->assign($attribute.'s', $tmp);
}

$attributes = array('style', 'license', 'state');
foreach($attributes as $attribute) {
	$tmp = array();
	foreach($_CATEGORY['SYSTEM'][$attribute] as $v) {
		$v['selected'] = $book[$attribute] == $v['id'] ? ' selected="selected"' : '';
		$tmp[] = $v;
	}

	$tpl->assign($attribute.'s', $tmp);
}


$tpl->assign(html_show($book, false));

$tpl->assign(array(
	'pseudonym' => $_AUTHOR['pseudonym'],
	'chapters'  => $db->select(array(
		'field' => 'id, title, priority, dateline, words, locked, vip',
		'from'  => __TAB_NOVEL_CHAPTER__,
		'where' => array('book_id' => $bid),
	)),
	'HTMLURL'   => novel_url($book, 0),
));

$tpl->output();

_footer();