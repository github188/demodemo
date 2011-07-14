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
//作者介绍程序

$aid = 0;

define('IN_Read8', true);
require_once str_replace(array('\\', '/'.basename(__FILE__)), array('/', ''), __FILE__).'/include/init.php';

if(!$aid || !ris_int($aid)) {
	back('非法的作者');
}

$author_info = $db->selectOne(array(
	'field' => 'id, pseudonym, pinyin, avatar, column_name, intro, announcement, dateline',
	'from'  => __TAB_MY_AUT__,
	'where' => array(
		'id'     => $aid,
		'active' => 1,
	),
));

if(!$author_info) {
	back('未找到该作家信息');
}

//设定搜索关键词
$seokeyword[]=$author_info['pseudonym'];
$seokeyword[]='authorinfo';
_header($seokeyword);//输出头部


$tpl = new AOKOTpl('authorinfo');
$tpl->assign(html_show($author_info));

$Newbookblock = $db->select(array(
		'field' => 'id, title, subject, content, style, state, dateline',
		'from'  => __TAB_NOVEL__,
		'where' => array('author_id' => $aid),
		'filter'=> 'convert_novel_classes',
));
$tpl->assign('Newbookblock', html_show($Newbookblock, false));
unset($Newbookblock);


$tpl->output();

_footer();
