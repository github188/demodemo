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

$page = 1;
$by = $kw = '';

define('IN_Read8', true);
require_once str_replace(array('\\', '/'.basename(__FILE__)), array('/', ''), __FILE__).'/include/init.php';

_header(array('搜索结果'));//输出头部


if($by != 'author')
	$by = 'title';

if(!$kw)
	back('请输入关键字');



$tpl = new AOKOTpl('search');

$tpl->assign(array(
	'novels'   => $db->select(array(
		'field' => '*, id AS bid, subject AS bp_t',
		'from'  => __TAB_NOVEL__,
		'where' => 'WHERE '.$by.' LIKE \'%'.$kw.'%\'',
		'order' => 'lastupdate DESC',
		'limit' => 20,
		'page'  => $page,
		'filter'=> '_novel_callback',
		'counter'=>1,
	)),
	'pagelink' => page($db->counter, 20, 0, 1),
	'by' => $by,
	'kw' => html_show($kw),
));

$tpl->output();

_footer();
