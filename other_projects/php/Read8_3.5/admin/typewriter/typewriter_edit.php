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
if(!defined('IN_Read8') || !defined('IN_Read8_ACP') || !defined('IN_Read8_ACP_TYPEWRITER') || !isset($PHP_SELF)) {
	exit('Access Denied (typewriter_add.php)');
}





$tpl = new AOKOTpl($pfile);

if(empty($id)) {
	$id = '';
}

$tpl->assign('C_user_id', $id);

if($id) {
	if(!ris_int($id))
		back('非法的用户编号');

	$row = $db->selectOne(array(
		'where' => array(
			'id' => $id,
		),
	));
	if(!$row)
		back('用户不存在');

	if($action == 'delete') {
		$db->delete(array(
			'id' => $id,
		));
		jumpage("?ptype=$ptype&pfile=$pfile", '删除成功');
	}elseif($action == 'update') {
		if(empty($overstep))
			$overstep = 0;

		if(empty($novel_add))
			$novel_add = 0;

		if(empty($novel_edit))
			$novel_edit = 0;

		if(empty($novel_del))
			$novel_del = 0;

		if(empty($chapter_add))
			$chapter_add = 0;

		if(empty($chapter_edit))
			$chapter_edit = 0;

		if(empty($chapter_del))
			$chapter_del = 0;

		$db->update(array(
			'novel_add'    => $novel_add,
			'novel_edit'   => $novel_edit,
			'novel_del'    => $novel_del == 1 ? $novel_del_num : $novel_del,
			'chapter_add'  => $chapter_add,
			'chapter_edit' => $chapter_edit,
			'chapter_del'  => $chapter_del == 1 ? $chapter_del_num : $chapter_del,
			'overstep'     => $overstep,
		), array(
			'id' => $id,
		));
		jumpage("?ptype=$ptype&pfile=$pfile", '编辑成功');
	}

	$tpl->assign($row);
}else {
	if(empty($thename))
		$thename = '';

	if(empty($regf))
		$regf = '';

	if(empty($regt))
		$regt = '';

	$sql = array(
		'field' => 'uw.*, u.username',
		'from'  => $db->table(__TAB_USER_TPWT__, 'uw'),
		'join'  => array('table' => $db->table(__TAB_USER__, 'u'), 'on' => 'uw.uid = u.uid'),
		'order' => 'dateline DESC',
		'limit' => PAGE_SIZE,
		'page'  => $page,
		'filter'=> 'html_show',
		'counter'=>1,
	);

	/*$where = 'WHERE active = 1';
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
	}*/

	$tpl->assign(array(
		'itemlist' => $db->select($sql),
		'pagelink' => page($db->counter, PAGE_SIZE),
		'thename'  => $thename,
		'regf'     => $regf,
		'regt'     => $regt,
	));
}

$tpl->output();

