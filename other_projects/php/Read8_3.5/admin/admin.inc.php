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
//后台帐号管理

if(!defined('IN_Read8') || !defined('IN_Read8_ACP') || !isset($PHP_SELF)) {
	exit('Access Denied (admin.inc.php)');
}
if(!acppriv()) back('admin_no_privileges_root');

$db->single(__TAB_USER_ACP__);

$selfrow = $db->selectOne(array(
	'where' => array('uid' => $Global['F_uid']),
));

if($action == 'add') {//添加后台用户
	if(!empty($pdeladmin)) {
		foreach($pdeladmin as $val) {
			if($val == $selfrow['id'])
				back('不能删除自己');

			if($selfrow['adduser'] && $val == $selfrow['adduser'])
				back('不能删除授予自己权限的管理员');

			$db->delete(array('id' => $val));
		}
	}
	if(!empty($paddadmin)) {
		if(!empty($paddadmin['adminname']) && !empty($paddadmin['username'])) {
			//选择前台用户
			if(!$row = $db->selectOne(array(
				'field' => 'uid, pwd',
				'from'  => __TAB_USER__,
				'where' => array('username' => $paddadmin['username']),
			))) back('用户不存在');
			//更新后台
			if(!empty($paddadmin['adminpswd'])) {
				$paddadmin['adminpswd'] = md5($paddadmin['adminpswd']);
			}else {
				$paddadmin['adminpswd'] = $row['pwd'];
			}

			$db->insert(array(
				'uid'       => $row['uid'],
				'name'      => $paddadmin['adminname'],
				'pswd'      => $paddadmin['adminpswd'],
				'privilege' => '',
				'adduser'   => $selfrow['uid'],
				'accessip'  => '',
				'history'   => '',
			));
		}
	}
	jumpage("?ptype=$ptype&pfile=$pfile", '管理成功');
}elseif($action == 'update') {//修改用户更新
	if(empty($padmin['id']) || !ris_int($padmin['id']))
		back('非法操作');

	if($selfrow['id'] == $padmin['id'])
		back('不能编辑自己');

	if($selfrow['adduser'] && $padmin['id'] == $selfrow['adduser'])
		back('不能编辑授予自己权限的管理员');

	if(empty($padmin['name']))
		back('用户名不能为空');

	$sql = array();

	$sql['name'] = $padmin['name'];
	if(!empty($padmin['pswd']))
		$sql['pswd'] = md5($padmin['pswd']);

	if(!empty($padmin['founder'])) {
		$sql['isfounder'] = 1;
	}else {
		$sql['isfounder'] = 0;
		$privs = array();
		foreach(acpmenu() as $m => $ks) {
			if(empty($padmin['priv'][$m]))
				continue;

			$privs[] = $m;
			foreach($ks as $k => $fs) {
				if(empty($padmin['priv'][$m][$k]))
					continue;

				$privs[] = $m.'_'.$k;
				foreach($fs as $p => $f) {
					if(!empty($padmin['priv'][$m][$k][$f])) {
						$privs[] = $m.'_'.$k.'_'.$f;
					}
				}
			}
		}
		$sql['privilege'] = implode(',', $privs);
	}

	if(!empty($padmin['ip']))
		$sql['accessip'] = $padmin['ip'];

	$db->update($sql, array(
		'id' => $padmin['id'],
	));

	jumpage("?ptype=$ptype&pfile=$pfile", '编辑成功');
}


$tpl = new AOKOTpl('admin');
$tpl->assign('action', $action);

if($action == 'edit') {
	if(empty($editadminuser) || !ris_int($editadminuser))
		back('用户不存在');

	if($editadminuser == $selfrow['id'])
		back('不能编辑自己');

	if($selfrow['adduser'] && $editadminuser == $selfrow['adduser'])
		back('不能编辑授予自己权限的管理员');

	if(!$user = $db->selectOne(array(
		'where' => array('id' => $editadminuser),
	))) back('用户不存在');
	$priv = explode(',', $user['privilege']);

	$acpprivs = array();
	foreach(acpmenu() as $m => $ks) {
		$ksprivs = array();
		$mpriv = false;
		foreach($ks as $k => $fs) {
			if($k == 'shortcut')
				continue;

			$fsprivs = array();
			$kpriv = false;
			foreach($fs as $p => $f) {
				$fpriv = in_array($m.'_'.$k.'_'.$f, $priv);
				$kpriv = $kpriv || $fpriv;
				$fsprivs[] = array(
					't'       => strip_tags(Lang('admin_menu_'.$f)),
					'm'       => $m,
					'k'       => $k,
					'f'       => $f,
					'checked' => $fpriv ? ' checked="checked"' : '',
				);
			}
			$mpriv = $mpriv || $kpriv;
			$ksprivs[] = array(
				't'       => strip_tags(Lang('admin_menu_key_'.$k)),
				'm'       => $m,
				'k'       => $k,
				'checked' => $kpriv ? ' checked="checked"' : '',
				'fsprivs' => $fsprivs,
			);
		}
		$acpprivs[] = array(
			't'       => strip_tags(Lang('admin_category_'.$m)),
			'm'       => $m,
			'checked' => $mpriv ? ' checked="checked"' : '',
			'ksprivs' => $ksprivs,
		);
	}

	$tpl->assign(array(
		'acpprivs'  => $acpprivs,
		'C_checked' => $user['isfounder'] ? ' checked="checked"' : '',
		'C_name'    => $user['name'],
		'C_ip'      => $user['accessip'],
		'C_id'      => $user['id'],
	));
}else {
	$tpl->assign('admins', $db->select(array(
		'field' => 'au.*, u1.username, u2.name AS adduser',
		'from'  => $db->table(__TAB_USER_ACP__, 'au'),
		'join'  => array(
			array('table' => $db->table(__TAB_USER__, 'u1'), 'on' => 'u1.uid = au.uid'),
			array('table' => $db->table(__TAB_USER_ACP__, 'u2'), 'on' => 'u2.id = au.adduser'),
		),
	)));
}

$tpl->output();
