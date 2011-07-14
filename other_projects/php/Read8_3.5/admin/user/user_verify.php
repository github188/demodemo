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
if(!defined('IN_Read8') || !defined('IN_Read8_ACP') || !defined('IN_Read8_ACP_USER') || !isset($PHP_SELF)) {
	exit('Access Denied (user_verify.php)');
}



if($action == 'delete') {
	if(!empty($ids))
		if(is_array($ids))
			foreach($ids as $k => $v)
				if(!$v || !ris_int($v))
					unset($ids[$k]);

	if(!empty($ids)) {
		$db->update(array(
			'active' => -99,
		), array(
			'uid'    => $ids,
		));
	}

	// logs
	jumpage("?ptype=$ptype&pfile=$pfile", '删除成功');
}

if(!empty($id)) {
	if(!ris_int($id))
		back('非法操作');

	$u = $db->selectOne(array(
		'field' => 'active',
		'where' => array(
			'uid' => $id,
		),
	));
	if(!$u)
		back('未知用户');

	if($u['active'])
		back('错误的状态');

	$db->update(array(
		'active' => 1,
	), array(
		'uid'    => $id,
	));

	//logs
	jumpage("?ptype=$ptype&pfile=$pfile", '操作成功');
}


$tpl = new AOKOTpl($pfile);

$sql = array(
	'field' => 'uid, username, email, dateline',
	'where' => array(
		'active' => 0,
	),
	'order' => 'dateline DESC',
	'limit' => PAGE_SIZE,
	'page'  => $page,
	'filter'=> 'html_show',
	'counter'=>1,
);

$tpl->assign(array(
	'itemlist' => $db->select($sql),
	'pagelink' => page($db->counter, PAGE_SIZE),
));


$tpl->output();

