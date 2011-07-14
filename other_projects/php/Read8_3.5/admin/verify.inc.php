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
if(!defined('IN_Read8') || !defined('IN_Read8_ACP') || !isset($PHP_SELF)) {
	exit('Access Denied (verify.inc.php)');
}


list($pf, $ps) = explode('_', $pfile);

$db->single($ps == 'novel' ? __TAB_NOVEL__ : __TAB_STORY__);


if($action == 'view') {
	if(!$bid || !ris_int($bid))
		back('错误');

	if(!($book = story_exists($bid)))
		back('未找到该文');

	$tpl = new AOKOTpl($pfile.'_view');
	$tpl->assign(html_show($book));
	$tpl->output();
}elseif($action == 'accept') {
	if(empty($bids) || !is_array($bids))
		back('请选择要审核的文章');

	foreach($bids as $k => $v)
		if(!$v || !ris_int($v))
			unset($bids[$k]);

	if(empty($bids))
		back('请选择要审核的文章');

	$db->update(array(
		'state' => 3,
	), array(
		'state' => 6,
		'id'    => $bids,
	));
	jumpage("?ptype=$ptype&pfile=$pfile", '审核成功');
}elseif($action == 'delete') {
	if(empty($bids) || !is_array($bids))
		back('请选择要删除的文章');

	foreach($bids as $k => $v)
		if(!$v || !ris_int($v))
			unset($bids[$k]);

	if(empty($bids))
		back('请选择要删除的文章');

	$db->update(array(
		'state' => 0,
	), array(
		'state' => 6,
		'id'    => $bids,
	));
	jumpage("?ptype=$ptype&pfile=$pfile", '删除成功');
}else {
	$sql = array(
		'where' => array(
			'state'  => 6,
		),
		'order' => 'dateline DESC',
		'page'  => $page,
		'limit' => 20,
		'filter'=> 'convert_story_classes',
		'counter'=>1,
	);

	$tpl = new AOKOTpl($pfile);
	$tpl->assign('books', html_show($db->select($sql), false));
	$tpl->assign('pagelink', Page($db->counter, 20));
	$tpl->output();
}


