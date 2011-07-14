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
	管理 - 作家中心
*/

define('IN_AUCP', true);
define('PHP_SELF_EXT', '_manage');
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


$tpl->assign(html_show($book, false));

if(!defined('PAGE_SIZE')) {
	define('PAGE_SIZE', 20);
}

$tpl->assign(array(
	'vip' => $_AUTHOR['vip'],
			'chapters'  => html_show($db->select(array(
				'field' => 'book_id, id, title, priority, dateline, words, locked, vip',
				'from'  => __TAB_NOVEL_CHAPTER__,
				'where' => array(
					'book_id' => $bid,
				),
				'order' => 'priority DESC',
				'limit' => PAGE_SIZE,
				'page'  => $page,
				'counter'=>1,
			))),
			'pagelink' => page($db->counter, PAGE_SIZE),
));

$tpl->output();

_footer();