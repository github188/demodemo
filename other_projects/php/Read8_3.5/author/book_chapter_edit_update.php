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
define('PHP_SELF_EXT', '_chapter_edit_update');
define('PHP_SELF', basename(__FILE__, PHP_SELF_EXT.'.php'));


$bid = $cid = '';

require_once '../include/init.php';
require_once ROOT.'author/category.inc.php';

require_once ROOT.'include/booked.func.php';
require_once ROOT.'include/editor.func.php';
require_once ROOT.'include/image.func.php';


_header(array($_SYSTEM['CATEGORY'][PHP_SELF]));

$tpl = new AOKOTpl('navigation_aucp');
$tpl->assign('categories', $category);
$tpl->output();

//$tpl = new AOKOTpl('author_'.PHP_SELF.PHP_SELF_EXT);
$book = html_show(get_novel_by_id($bid), false);
if(!$book) {
	back('无此书信息');
}
if($book['author_id'] != $_AUTHOR['id']) {
	back('您无权管理此书');
}
if($book['state'] > 4) {
	back('此书正在审核');
}

$chapter = html_show(novel_chapter_exists($bid, $cid), false);
if(!$chapter)
	back('未找到该章节信息');

$priority = novel_get_chapter_priority_by_id($bid, $cid);



$title   = trim($title);
$article = rtrim($article);
$dir = novel_url($book, 0);

if(!$chapter['vip'] && $_SYSTEM['UPLOAD']['open'] && $_SYSTEM['UPLOAD']['size'] && !empty($_FILES['newattach'])) {
	$article .= new_attach(upload_convert($_FILES['newattach']), $book, $cid);
}

$tpl = new AOKOTpl('../../html/template/content');
$tpl->assign(array(
	'chapter_title' => stripslashes($title),
	'book_title'    => $book['title'],
	'C_authorname'  => $book['author'],
	'bid'           => $bid,
	'cid'           => $cid,
	'C_lchapterid'  => $priority - 1,
	'C_nchapterid'  => $priority + 1,
	'C_tchapterid'  => $priority,
	'C_content'     => editor_safe_replace(UbbDecode(stripslashes($article))),
));

if($chapter['vip']) {
	rfow(ROOT.novel_url($book, $cid.'_'.$chapter['hash']), $tpl->output(1));

	include _config('vip');
	$shopurl    = $_SYSTEM['SYSTEM']['SITE_ADDR'].'/cart.php?bid='.$bid.'&amp;cid='.$cid;
	$loginurl   = $_SYSTEM['SYSTEM']['SITE_ADDR'].'/login.php?forward='.rawurlencode($shopurl);
	$vippayurl  = $_SYSTEM['SYSTEM']['SITE_ADDR'].'/shop.php';
	$vipcontent = '<div id="vip"><img src="'.$_SYSTEM['SYSTEM']['SITE_ADDR'].'/vip.php?bid='.$bid.'&amp;cid='.$cid.'" usemap="#Face" alt="" /></div><map name="Face" id="Face"><area shape="rect" href="'.$shopurl.'" coords="'.$_SYSTEM['VIP']['area_buy'].'" alt="放入购物车" /><area shape="rect" href="'.$loginurl.'" coords="'.$_SYSTEM['VIP']['area_login'].'" alt="登录" /><area shape="rect" href="'.$vippayurl.'" coords="'.$_SYSTEM['VIP']['area_pay'].'" alt="购买结帐" /></map>';//增加map热点供点击
	$tpl->assign('C_content', $vipcontent);

	rfow(ROOT.novel_url($book, $priority), $tpl->output(1));
}else {
	rfow(ROOT.novel_url($book, $priority), $tpl->output(1));
}

//rfow(ROOT.novel_url($book, $priority), $tpl->output(1));

//书籍更新mysql
//$sqladd = $end ? ", `state` = 0" : '';
//$words  = $cid ? 0 : $words;
/*$db->update(array(
	'lastupdate'  => TIMESTAMP,
	'lastchapter' => $abschapterid,
), array(
	'id'          => $bid,
), __TAB_NOVEL__);*/
//$sql="UPDATE ".__TAB_BOOK__." SET `updatetime` = '".TIMESTAMP."', `newchapterid` = '$abschapterid', `size` = `size` + ".$words.$sqladd." WHERE `id` = '$pbid'";
//$db->execUpBySql($sql);

novel_volume_tidy($bid);//章节表的卷重新生成

novel_rebuild_index($bid);

jumpage('book_manage.php?bid='.$bid, '编辑成功');
