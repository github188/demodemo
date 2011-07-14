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
	exit('Access Denied (manage_story.php)');
}

$db->single(__TAB_STORY__);

if(empty($thename)) {
	$thename = '';
}

if($action == 'delete') {
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
		'state' => 3,
		'id'    => $bids,
	));
	jumpage("?ptype=$ptype&pfile=$pfile", '删除成功');
}

$tpl = new AOKOTpl($pfile);


$sql = array(
	'field' => '*, id AS bid',
	'where' => 'WHERE state = 3',
	'order' => '`lastupdate` DESC',
	'limit' => 20,
	'page'  => $page,
	'filter'=> '_story_callback',
	'counter' => true,
);
if(trim($thename)) {
	$sql['where'] .= " AND title LIKE '%".trim($thename)."%' ";
}


$tpl->assign(array(
	'stories'  => $db->select($sql),
	'pagelink' => page($db->counter, 20),
	'C_thename' => $thename,
));

$tpl->output();
