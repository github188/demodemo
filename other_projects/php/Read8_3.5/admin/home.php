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
//后台首页

if(!defined('IN_Read8') || !defined('IN_Read8_ACP') || !isset($PHP_SELF)) {
	exit('Access Denied (home.php)');
}

//相关版本
$phpver = PHP_VERSION;
$phpos = PHP_OS;
$dbversion = $db->ver();
//相关时间
$servertime = gmdate('Y-m-d H:i:s', TIMESTAMP);
$thistime = rdate(TIMESTAMP, 'Y-m-d H:i:s');

$adminonline = $db->select(array(
	'field' => 'u.username AS C_name',
	'from'  => $db->table(__TAB_SESS_ACP__, 'a'),
	'join'  => array('table' => $db->table(__TAB_USER__, 'u'), 'on' => 'a.uid = u.uid'),
));

//检测gd
$jpgspt=0;
if(function_exists('gd_info')) {
	$gdinfo=gd_info();
	if($gdinfo['JPG Support']) $jpgspt= 1;
}
//Mysql容量
$dbsize = 0;
$tables = $db->selectBySql("SHOW TABLE STATUS LIKE '".$_SYSTEM['db_prefix']."%'");
foreach($tables as $table) {
	$dbsize += $table['Data_length'] + $table['Index_length'];
}
$dbsize = $dbsize ? sizecount($dbsize) : '未知';

//官方网站获取信息用的hash
$hash = base64_encode($_SYSTEM['SYSTEM']['SITE_ADDR'].'|||'.$_SYSTEM['SYSTEM']['SITE_NAME'].'|||'.SYS_VERSION.'|||'.SYS_BUILD);

//显示
$tpl = new AOKOTpl('index');
$tpl->assign("C_servertime",$servertime);
$tpl->assign("C_thistime",$thistime);
$tpl->assign("C_timeblock",$_SYSTEM['SYSTEM']['TIME_ZONE']);
$tpl->assign("C_timedelay",$_SYSTEM['SYSTEM']['TIME_DELAY']);
$tpl->assign("admins",$adminonline);
$tpl->assign("C_dbversion",$dbversion);
$tpl->assign("C_phpver",$phpver);
$tpl->assign("C_phpos",$phpos);
$tpl->assign("C_dbsize",$dbsize);
$tpl->assign("C_jpgspt",$jpgspt);
$tpl->assign("C_version",SYS_VERSION.' Build '.SYS_BUILD);
$tpl->assign("C_hash",$hash);
$tpl->output();

?>