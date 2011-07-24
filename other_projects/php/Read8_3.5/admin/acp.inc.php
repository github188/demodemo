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
//后台初始化程序

if(!defined('IN_Read8')) {
	exit('Access Denied (acp.inc.php)');
}


empty($frame) && $frame = '';
empty($ptype) && $ptype = '';
empty($pfile) && $pfile = '';

$_tmp_pswd = $_tmp_new_pswd = $_tmp_repeat_pswd = '';
if(!empty($_POST['admin_pswd'])) {
	$_tmp_pswd = $_POST['admin_pswd'];
	$_POST['admin_pswd'] = '*****';
}
if(!empty($_POST['admin_new_pswd'])) {
	$_tmp_new_pswd = $_POST['admin_new_pswd'];
	$_POST['admin_new_pswd'] = '*****';
}
if(!empty($_POST['admin_repeat_pswd'])) {
	$_tmp_repeat_pswd = $_POST['admin_repeat_pswd'];
	$_POST['admin_repeat_pswd'] = '*****';
}
_log('acp', array(
	$ptype,
	$pfile,
	base64_encode(serialize($_GET)),
	base64_encode(serialize($_POST)),
	base64_encode(serialize($_COOKIE)),
	base64_encode(serialize($_FILES)),
));
if(!empty($_tmp_pswd)) {
	$_POST['admin_pswd'] = $_tmp_pswd;
}
if(!empty($_tmp_new_pswd)) {
	$_POST['admin_new_pswd'] = $_tmp_new_pswd;
}
if(!empty($_tmp_repeat_pswd)) {
	$_POST['admin_repeat_pswd'] = $_tmp_repeat_pswd;
}
unset($_tmp_pswd, $_tmp_new_pswd, $_tmp_repeat_pswd);

//设置快捷cookie保存
if(!$shortcut = _cookie_decode('quickpfile', true)) $shortcut = array();
if($ptype && $pfile) {
	array_unshift($shortcut, $ptype.'||'.$pfile);
	array_unshift($shortcut, 'default||home');
	$shortcut = array_unique($shortcut);
	while(count($shortcut) > 10) {
		array_pop($shortcut);
	}
	_cookie_encode('quickpfile', $shortcut, 3153600);
}

//设置后台程序的模板地址
//$Global['oldstyle'] = $Global['style'];
//$Global['style'] = '../admin/template';
$_OLDSTYLE = $_SYSTEM['DISPLAY']['template'];
$_SYSTEM['DISPLAY']['template'] = '../admin/template';
//$_Stylecfg['../admin/template']['image'] = 'admin/images';
//$_Stylecfg['../admin/template']['template'] = '../admin/template';

$_assign['pfile'] = $pfile;
$_assign['ptype'] = $ptype;
$_assign['IMGDIR'] = $_OLDSTYLE.'/images';
//$Global['main_assign']['G_style'] = $Global['style'];
//$Global['main_assign']['G_oldstyle'] = $Global['oldstyle'];

$db->execUpBySql("DELETE FROM ".__TAB_SESS_ACP__." WHERE expire<".TIMESTAMP."");

$user = $admin = $adminsess = $isfounder = '';
$adminpriv = $adminhistory = array();

if(!$user = $db->selectOne(array(
	'field' => 'uid',
	'from'  => __TAB_USER__,
	'where' => array('username' => $Global['F_username']),
))) {
	$_SYSTEM['DISPLAY']['template'] = $_OLDSTYLE;
	back('您无权访问后台', 7);
}

if(!$admin = $db->selectOne(array(
	'from'  => __TAB_USER_ACP__,
	'where' => array('uid' => $user['uid']),
))) {
	$_SYSTEM['DISPLAY']['template'] = $_OLDSTYLE;
	back('您无权访问后台');
}

if($admin['isfounder']) $isfounder = true;
$needchangepswd = !acppriv() && $admin['changepswdtime'] <= TIMESTAMP;
if($needchangepswd) $adminhistory = explode(',', $admin['history']);

$isLogin = false;

$Global['ACP_userid'] = $admin['id'];

if($adminsess = $db->selectOne(array(
	'from'  => __TAB_SESS_ACP__,
	'where' => array(
		'id'   => $admin['id'],
		'name' => $admin['name'],
		'pswd' => $admin['pswd'],
	),
))) {
	$isLogin = true;
	$expiretime = 0;
	if(!$_SYSTEM['SAFE']['acp_expire']) $_SYSTEM['SAFE']['acp_expire'] = 15;
	if($action != 'logoff') $expiretime = TIMESTAMP + $_SYSTEM['SAFE']['acp_expire'] * 60;
	else {
		//增加日志记录
		//AddLog('退出登录', 'access');
	}
	$db->execUpBySql("UPDATE ".__TAB_SESS_ACP__." SET expire='$expiretime'");
	if($action == 'logoff') {
		header('Location: '.$_SYSTEM['SYSTEM']['SITE_ADDR']);
		exit;
	}
	if($adminsess['isfounder']) $isfounder = true;
	else $adminpriv = explode(',', $adminsess['privilege']);
}

if(!$isLogin) {//用户登录
	$admin_name = empty($_POST['admin_name']) ? '' : caddslashes($_POST['admin_name']);
	$admin_pswd = empty($_POST['admin_pswd']) ? '' : md5($_POST['admin_pswd']);
	if($admin_name && $admin_pswd) {//用户后台登录
		$admin = '';
		if($admin = $db->selectOne(array(
			'from'  => __TAB_USER_ACP__,
			'where' => array('name' => $admin_name),
		))) {
			//IP和旧密码检查
			if($admin['accessip'] && !ipAccess($admin['accessip'], false)) {//IP禁止
				back('admin_login_ip_error');
			}elseif($admin['pswd'] != $admin_pswd) {//旧密码错误
				//增加日志记录
				if(strlen($_POST['admin_pswd'])>4) $logpwd=substr($_POST['admin_pswd'],0,2)."...".substr($_POST['admin_pswd'],-2);
				else $logpwd=substr($_POST['admin_pswd'],0,1)."...".substr($_POST['admin_pswd'],-1);

				$db->execUpBySql("UPDATE ".__TAB_USER_ACP__." SET error=error+1 WHERE id='$admin[id]'");
				back('admin_login_pswd_error');
			}
			//更新用户后台密码资料
			if(!$_SYSTEM['SAFE']['acp_expire']) $_SYSTEM['SAFE']['acp_expire'] = 15;
			$db->execUpBySql("UPDATE ".__TAB_USER_ACP__." SET lastlogin='".TIMESTAMP."', lastloginip='$Global[F_userip]' WHERE id='$admin[id]'");
			$db->execUpBySql("REPLACE INTO ".__TAB_SESS_ACP__." VALUES('$admin[id]', '$admin[uid]', '$admin[name]', '$admin[pswd]', '$admin[privilege]', '$Global[F_userip]', '$admin[isfounder]', '".(TIMESTAMP + $_SYSTEM['SAFE']['acp_expire'] * 60)."')");

			acpheader();
			jumpage($_SYSTEM['SYSTEM']['SITE_ADDR'].'/admin/'.(empty($_POST['requestring']) ? '' : '?'.$_POST['requestring']), 'admin_login_success');
		}else {
			back('admin_login_failed');
		}
	}else {//显示登录页面
		$tpl = new AOKOTpl('frame_login');
		$tpl->assign('U_adminname', '');
		$tpl->assign('C_querystring',empty($_SERVER['QUERY_STRING']) ? '' : $_SERVER['QUERY_STRING']);
		$tpl->output();
		acpfooter();
		exit;
	}

}elseif($needchangepswd || $action == 'changepswd') {//密码过期||主动修改
	$admin_name = empty($_POST['admin_name']) ? '' : caddslashes($_POST['admin_name']);
	$admin_new_pswd = empty($_POST['admin_new_pswd']) ? '' : md5($_POST['admin_new_pswd']);
	$admin_repeat_pswd = empty($_POST['admin_repeat_pswd']) ? '' : md5($_POST['admin_repeat_pswd']);

	if($admin_name && $admin_new_pswd) {//更换提交
		$admin = '';
		if(!$admin = $db->selectOne(array(
			'from'  => __TAB_USER_ACP__,
			'where' => array('name' => $admin_name),
		))) back('admin_user_not_granted');
		//两次密码不同
		if($admin_new_pswd != $admin_repeat_pswd) back('admin_change_pswd_error_repeat');
		//密码长度不对
		if(strlen($_POST['admin_new_pswd']) < $_SYSTEM['SAFE']['acp_pswd_minlength']) back('admin_change_pswd_error_length');
		$adminhistory[] = $admin['pswd'];
		while(count($adminhistory) > $_SYSTEM['SAFE']['acp_pswd_history']) array_pop($adminhistory);
		if(in_array($admin_new_pswd, $adminhistory)) back('admin_change_pswd_error_history');
		$db->execUpBySql("UPDATE ".__TAB_USER_ACP__." SET pswd='$admin_new_pswd', history='".implode(',', $adminhistory)."', changepswdtime='".(TIMESTAMP+$_SYSTEM['SAFE']['acp_pswd_expire']*86400)."' WHERE name='$admin_name'");
		$db->execUpBySql("UPDATE ".__TAB_SESS_ACP__." SET pswd='$admin_new_pswd' WHERE name='$admin_name'");

		acpheader();
		jumpage($_SYSTEM['SYSTEM']['SITE_ADDR'].'/admin/'.(empty($_POST['requestring']) ? '' : '?'.$_POST['requestring']), 'admin_change_pswd_success');

	}else {//显示需要更换页面
		$tpl = new AOKOTpl('frame_login');
		$tpl->assign('U_adminname', $adminsess['name']);
		$tpl->assign('C_forcechange', $action != 'changepswd');
		$tpl->assign('C_querystring',empty($_SERVER['QUERY_STRING']) ? '' : $_SERVER['QUERY_STRING']);
		$tpl->output();
		acpfooter();
		exit;
	}
}

define('IN_Read8_ACP', true);


//菜单生成
require_once ROOT.'admin/menu.cfg.php';
$acpcategorys = acpcategorys();
$acpmenu = acpmenu();
$acpdefaultmenu = $acporidefault = acpdefaultmenu();
/*foreach($_Modulecfg as $key => $cfg) {
	if(!$cfg['open'] || $cfg['method'] != 3 || empty($cfg['acpmenu']['list'])) continue;
	if($cfg['acpmenu']['pos'] == 'C' && !empty($cfg['acpmenu']['name'])) {
		if($cfg['acpmenu']['at'] == 'after') {
			$acpcategorys = acpc_insert($acpcategorys, $cfg['acpmenu']['name'], $cfg['acpmenu']['count']);
			$acpmenu[$cfg['acpmenu']['name']][$cfg['acpmenu']['name']] = $cfg['acpmenu']['list'];
		}elseif($cfg['acpmenu']['at'] == 'in') {
			$acpmenu[$cfg['acpmenu']['count']] = array_merge(array($cfg['acpmenu']['name'] => $cfg['acpmenu']['list']), $acpmenu[$cfg['acpmenu']['count']]);
		}
	}elseif($cfg['acpmenu']['pos'] == 'M') {
		if($cfg['acpmenu']['at'] == 'after' && !empty($cfg['acpmenu']['name'])) {
			foreach($acporicategorys as $val) {
				if(!array_key_exists($cfg['acpmenu']['count'], $acporimenu[$val])) continue;
				$acpmenu[$val] = acpm_insert($acpmenu[$val], $cfg['acpmenu']['name'], $cfg['acpmenu']['count'], $cfg['acpmenu']['list']);
			}
		}elseif($cfg['acpmenu']['at'] == 'in') {
			foreach($acporicategorys as $val) {
				if(!array_key_exists($cfg['acpmenu']['count'], $acporimenu[$val])) continue;
				foreach($cfg['acpmenu']['list'] as $list) $acpmenu[$val][$cfg['acpmenu']['count']][] = $list;
			}
		}
	}
}*/

// $m 大菜单 $k 内容块 $f 功能
if(!$isfounder) {
	foreach($acpcategorys as $mp => $m) {
		if(!acppriv($m) && $m != 'default') {// 没有大类权限
			unset($acpcategorys[$mp]);
			unset($acpmenu[$m]);
			continue;
		}
		$first = $m != 'default';// 第一个有权项
		foreach($acpmenu[$m] as $k => $fs) {
			if(acppriv($m, $k) || $k == 'shortcut') {// 有功能块权限
				foreach($fs as $p => $f) {// 遍历功能
					if(acppriv($m, $k, $f)) {// 有权限
						if($f == $acporidefault[$m] || $first) {// 预设默认或者第一个
							$acpdefaultmenu[$m] = $f;// 设置默认
							$first = false;
						}
					}else {
						unset($acpmenu[$m][$k][$p]);
					}
				}
			}else {
				unset($acpmenu[$m][$k]);
			}
		}
	}
}


$Lang = GetLang(1);


if(!$frame && !$pfile && !$ptype) {//默认显示总框架
	$tpl = new AOKOTpl('frame');
	$tpl->output();
	exit;
}elseif($frame == 'head') {//顶部条
	$tpl = new AOKOTpl('frame_head');

	$catblock = array();
	$i = 1;
	krsort($acpcategorys);
	foreach($acpcategorys as $category) {
		$catblock[] = array(
			'C_i' => $i++,
			'C_ptype' => $category,
			'C_title' => Lang($category, 'admin_category_'),
			//'C_menuon' => $ptype == $category ? ' id="menuon"' : '',
			//'G_siteurl' => $Global['siteurl'],
		);
	}
	$tpl->assign('C_catblock',$catblock);
	$tpl->output();
	exit;
}elseif($frame == 'left') {//左侧条
	$lists = array();
	$i = $jspfile = 0;
	foreach($acpmenu[$ptype] as $title => $items) {
		$funclist = array();
		if($title == 'shortcut') {
			foreach($shortcut as $val) {
				$val = explode('||', $val);
				$funclist[] = array(
					'C_i' => ++$i,
					'C_name' => Lang($val[1], 'admin_menu_'),
					'C_pfile' => $val[1],
					'C_ptype' => $val[0],
					'C_pfileon' => $val[1] == 'home' ? ' id="pfileon"' : '',
				);
			}
			$jspfile = 1;
		}/*elseif($title == 'modules') {
			foreach($_Modulecfg as $key => $cfg) {
				if(!$cfg['open']) continue;
				if(is_file(ROOT."modules/$key/admin.php")) {
					$funclist[] = array(
						'C_i' => ++$i,
						'C_name' => $cfg['name'],
						'C_pfile' => $key,
						'C_ptype' => $ptype,
						'C_pfileon' => '',
					);
				}
			}
		}*/
		foreach($items as $val) {
			$funclist[] = array(
				'C_i' => ++$i,
				'C_name' => Lang($val, 'admin_menu_'),
				'C_pfile' => $val,
				'C_ptype' => $ptype,
				'C_pfileon' => $acpdefaultmenu[$ptype] == $val ? ' id="pfileon"' : '',
			);
			$jspfile = $jspfile ? $jspfile : ($acpdefaultmenu[$ptype] == $val ? $i : 0);
		}
		$lists[] = array(
			'C_i' => $i++,
			'C_name' => Lang($title, 'admin_menu_key_'),
			'funclist' => $funclist,
			'C_collapsed' => isset($lists[$pfile]) ? ' id="ptypeon"' : '',
		);
	}
	$jspfile = $jspfile ? $jspfile : 1;
	$tpl = new AOKOTpl('frame_left');
	$tpl->assign('lists', $lists);
	$tpl->assign('jspfile', $jspfile);
	$tpl->output();
	exit;
}



acpheader();

if(!$ptype || !in_array($ptype, $acpcategorys)) {
	back('无权访问');
}else {
	if(!$pfile || $pfile == 'default')
		$pfile = $acpdefaultmenu[$ptype];

	$inarray = false;
	foreach($acpmenu[$ptype] as $key => $val) {
		if(in_array($pfile, $val)) {
			$inarray = true;
			break;
		}
	}
	if(!$inarray && $pfile != 'home')
		back('无权访问');

	$admfile = acpfile($pfile);

	//包含子文件
	if($admfile) {
		$admfilepath = ROOT.'admin/'.$admfile.'.inc.php';
	}else {
		$admfilepath = ROOT.'admin/'.$pfile.'.php';
	}
	if(!is_file($admfilepath))
		back('暂不提供');

	require $admfilepath;
}

acpfooter();


function acpfile($pfile){
	switch($pfile) {
		case 'official_ask':
		case 'official_bug':
		case 'official_site':
		case 'official_manual':
		case 'official_info':
		case 'official_join':
		case 'download_rule':
			$admfile = 'official';
			break;
		case 'category_novel':
		case 'category_story':
			$admfile = 'category';
			break;
		case 'comment_novel':
		case 'comment_story':
			$admfile = 'comment';
			break;
		case 'verify_novel':
		case 'verify_story':
			$admfile = 'verify';
			break;
		case 'count_global':
		case 'count_book':
		case 'count_writer':
			$admfile = 'count';
			break;
		case 'log_acp':
		case 'log_php':
		case 'log_mysql':
			$admfile = 'log';
			break;
		case 'config_system':
		case 'config_user':
		case 'config_author':
		case 'config_story':
		case 'config_display':
		case 'config_read':
		case 'config_comment':

		case 'config_upload':
		case 'config_fopen':
		case 'config_seo':
		case 'config_safe':
		case 'config_info':
		case 'config_rank':
		case 'config_official':

		case 'config_pay':
		case 'config_paymonth':
		case 'config_vip':
		case 'config_mobile':
			$admfile = 'system';
			break;
		case 'user_add':
		case 'user_edit':
		case 'user_ban':
		case 'user_verify':
			$admfile = 'user';
			break;
		case 'typewriter_add':
		case 'typewriter_edit':
			$admfile = 'typewriter';
			break;
		case 'admin_manage':
			$admfile = 'admin';
			break;
		case 'db_export':
		case 'db_import':
		case 'db_update':
		case 'db_optimize':
			$admfile = 'db';
			break;
		case 'bookbackup':
		case 'bookrecovery':
			$admfile = 'bookbase';
			break;
		case 'collect_single':
		case 'collect_multi':
		case 'collect_single_rule':
		case 'collect_multi_rule':
		case 'collect_blacklist':
			$admfile = 'collect';
			break;
		case 'novel_commend_face':
		case 'novel_commend_subject':
		case 'novel_commend_strong':
		case 'novel_commend_editor':
		case 'novel_commend_vip':
		case 'story_commend_face':
		case 'story_commend_subject':
		case 'story_commend_strong':
		case 'story_commend_editor':
		case 'story_commend_vip':
			$admfile = 'commend';
			break;
		case 'style_choose':
		case 'style_cache':
			$admfile = 'style';
			break;
		case 'author_add':
		case 'author_edit':
		case 'author_verify':
		case 'author_interview':
			$admfile = 'author';
			break;
		default :
			$admfile = '';
	}
	return $admfile;
}


function acpheader(){
	global $_SYSTEM;
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=<?php echo SYSCHARSET; ?>" />
<title><?php echo $_SYSTEM['SYSTEM']['SITE_NAME']; ?> - 后台管理</title>
<base href="<?php echo $_SYSTEM['SYSTEM']['SITE_ADDR']; ?>/admin/" />
<link href="template/acp.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="../include/js/common.js"></script>
<noscript>
<style type="text/css">
	#adminframe, .leftmenu, #adminright {
		display: none;
	}
</style>
<div style="color: Red;">您需要开启浏览器的JavaScript支持才能正常使用后台</div>
</noscript>
</head>
<body onload="javascript:externallinks();">
<div id="adminright">
<?php
}

function acpfooter(){
?>
	<div id="footer">
		<hr class="admin" />
		<div id="copyright">
<?php
	echo str_replace(array(
		'{read8cn}',
		'{read8en}',
		'{read8ver}',
	), array(
		'<a href="http://www.read8.net" rel="external" id="by">读吧书网</a>',
		'<a href="http://www.read8.net" rel="external" id="by">Read8.Net</a>',
		SYS_VERSION,
	), $GLOBALS['_SYSTEM']['SYSTEM']['copyright_system']);
?>
		</div>
	</div>
</div>
</body>
</html>
<?php
}


//后台权限检测 ok
function acppriv(){
	$args = func_get_args();

	if(!$args) {
		return $GLOBALS['isfounder'];
	}else {
		return $GLOBALS['isfounder'] || in_array(implode('_', $args), $GLOBALS['adminpriv']);
	}
}

