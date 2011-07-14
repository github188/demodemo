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
	exit('Access Denied (user_edit.php)');
}



if($action == 'update') {
	if(!$puser_id || !ris_int($puser_id))
		back('非法操作');

	$row = $db->selectOne(array(
		'where' => array(
			'uid' => $puser_id,
		),
	));
	if(!$row)
		back('无此用户');

	$sql = array();
	if(!empty($pusername)) {
		if($db->selectOne(array(
			'field' => 'uid',
			'where' => array(
				'username' => $pusername,
			),
		))) {
			back('用户已存在');
		}
		$sql['username'] = $pusername;
	}
	if($pnewpswd || $pnewpswd2) {
		if($pnewpswd != $pnewpswd2)
			back('两次密码不一致');

		$sql['pwd'] = md5($pnewpswd);
	}
	if(!$pemail || !preg_match("/^\w[\w\.\-\+]*@\w[\w\-]+(\.\w+)+$/", $pemail))
		back('非法的邮箱');

	$sql['email'] = $pemail;

	$db->update($sql, array(
		'uid' => $puser_id,
	));

	//logs
	jumpage("?ptype=$ptype&pfile=$pfile", '编辑成功');
}elseif($action == 'delete') {
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


$tpl = new AOKOTpl($pfile);

if(empty($id)) {
	$id = '';
}

$tpl->assign('C_user_id',$id);

if($id) {
	if(!ris_int($id))
		back('非法的用户编号');

	$row = $db->selectOne(array(
		'where' => array(
			'uid' => $id,
		),
	));
	if(!$row)
		back('用户不存在');

	$tpl->assign($row);
}else {
	if(empty($thename))
		$thename = '';

	if(empty($regf))
		$regf = '';

	if(empty($regt))
		$regt = '';

	$sql = array(
		'field' => 'uid, username, email, dateline',
		'order' => 'dateline DESC',
		'limit' => PAGE_SIZE,
		'page'  => $page,
		'filter'=> 'html_show',
		'counter'=>1,
	);

	$where = 'WHERE active = 1';
	$search = array();
	if($thename) {
		$search['thename'] = $thename;
		$where .= ' AND username LIKE \'%'.$thename.'%\'';
	}
	if($regf || $regt) {
		$search['regf'] = $search['regt'] = '';
		$where .= ' AND ';

		if(!$regf) {
			$search['regt'] = $regt;
			$where .= 'dateline <= '.strtotime($regt);
		}elseif(!$regt) {
			$search['regf'] = $regf;
			$where .= 'dateline >= '.strtotime($regf);
		}else {
			$where .= 'dateline >= '.strtotime($regf).' AND dateline <= '.strtotime($regt);
		}
	}

	if($where) {
		$sql['where'] = $where;
	}

	$tpl->assign(array(
		'itemlist' => $db->select($sql),
		'pagelink' => page($db->counter, PAGE_SIZE, 0, 1, $search),
		'thename'  => $thename,
		'regf'     => $regf,
		'regt'     => $regt,
	));
}

$tpl->output();

