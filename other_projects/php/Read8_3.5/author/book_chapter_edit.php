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
	编辑章节 - 作家中心
*/

define('IN_AUCP', true);
define('PHP_SELF_EXT', '_chapter_edit');
define('PHP_SELF', basename(__FILE__, PHP_SELF_EXT.'.php'));


$bid = $cid = '';

require_once '../include/init.php';
require_once ROOT.'author/category.inc.php';

require_once ROOT.'include/booked.func.php';
require_once ROOT.'include/editor.func.php';


_header(array($_SYSTEM['CATEGORY'][PHP_SELF]));

$tpl = new AOKOTpl('navigation_aucp');
$tpl->assign('categories', $category);
$tpl->output();

//$tpl = new AOKOTpl('author_'.PHP_SELF.PHP_SELF_EXT);
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

$chapter = novel_chapter_exists($bid, $cid);
if(!$chapter)
	back('未找到该章节信息');

$priority = novel_get_chapter_priority_by_id($bid, $cid);


_header(array('编辑章节'));

$tpl = new AOKOTpl('author_'.PHP_SELF.PHP_SELF_EXT);
$tpl->assign('list', 0);
$tpl->assign('book', html_show($book, false));

$tpl->assign(html_show($chapter, false));
$tpl->assign(array(
	'bid'     => $bid,
	'cid'     => $cid,
	'editor'  => html_editor('article', novel_content(ROOT.novel_url($book, $priority.($chapter['vip'] ? '_'.$chapter['hash'] : '')))),
	'upload'  => $_SYSTEM['UPLOAD']['open'] ? sizecount($_SYSTEM['UPLOAD']['size']) : 0,
	'formats' => implode(' ', $format),
	'vip'     => $_AUTHOR['vip'],
));

$tpl->output();

_footer();
