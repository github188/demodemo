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
//初始化程序

function_exists('ini_set') && ini_set('display_errors', 1);
function_exists('ini_set') && ini_set('log_errors', 1);
error_reporting(1);
define('ROOT', str_replace(array('\\', 'include/init.php'), array('/', ''), __FILE__));
//function_exists('ini_set') && ini_set('error_log', ROOT.'data/log/systemerror.log.php');

if(!defined('IN_Read8')) {
	define('IN_Read8', true);
}

if(!is_file(ROOT.'data/install.close')) {
	header("Content-Type: text/html; charset=utf-8");
	exit('如需安装书网，请运行 <a href="install">./install</a>');
}

//PHP5 统一GMT时区 下面用 rdate([int timestamp[, string format]]) 格式化为书网时间
function_exists('date_default_timezone_set') && date_default_timezone_set('GMT');

$mtime = explode(' ', microtime());
$starttime = $mtime[1] + $mtime[0];


if(!defined('IN_UCP')) {
	define('IN_UCP', false);
}
if(!defined('IN_AUCP')) {
	define('IN_AUCP', false);
}


require ROOT.'include/const.def.php';
require ROOT.'include/var.def.php';

require_once ROOT.'include/date.func.php';
require_once ROOT.'include/fso.func.php';
require_once ROOT.'include/string.func.php';
require_once ROOT.'include/math.func.php';
require_once ROOT.'include/book.func.php';
require_once ROOT.'include/safe.func.php';

require_once ROOT.'include/global.func.php';
require_once ROOT.'include/MySQL.class.php';
require_once ROOT.'include/Model.class.php';
require_once ROOT.'include/category.func.php';
require_once ROOT.'include/Template.class.php';
require_once ROOT.'include/config.func.php';
require_once ROOT.'include/cache.func.php';

$ajax = '';

$action = $sid = $asid = $forward = $header = $noheader = $loginuser = $loginpwd = $hash = $hidden = $bid = $mbid = $vbid = $pbid = $cid = $did = $vid = $pfile = $frame = $admfile = $autonext = $manualsub = $thename = $getad = $timed = $bpid = $pid = $thisinfo = $initial = $stype = $lang = $maxorderid = $end = $midinsert = $newvolume = $tmp = $logit = $content =
$temp = $tidy = $delid = $info = '';

$refreshed = $noheader = '';

if(!isset($page)) {
	$page = 1;
}

set_magic_quotes_runtime(0);
if(!get_magic_quotes_gpc()){
	$_POST = caddslashes($_POST);
	$_GET = caddslashes($_GET);
	$_COOKIE = caddslashes($_COOKIE);
	$_FILES = caddslashes($_FILES);
}
extract($_POST);
extract($_GET);

$_SYSTEM = $Global = $_CACHE = $_RANK = array();
require ROOT.'include/var.def.php';



require_once ROOT.'config.inc.php';
require_once ROOT.'include/table.def.php';
require_once ROOT.'include/version.def.php';

//初始化网站
$_SYSTEM['db_prefix'] = $C_MySQL['_prefix_'];
$db = new AOKOM(new sql_db($C_MySQL['hostname'], $C_MySQL['username'], $C_MySQL['password'], $C_MySQL['database']));
unset($C_MySQL);

require_once ROOT.'include/rank.func.php';

include _config('system');
include _config('safe');
include _config('user');
include _config('display');
//error_reporting($Global['error_reporting_level']);

require ROOT.'include/var.def.php';


$Global['ImplodeChar']="|\tR--";

//运行于调试模式，输出错误给站长
//$Global['runasdebug'] && function_exists('ini_set') && IsGroup($Global['Rootgroupid']) && ini_set('display_errors', 1);


//安全文件设定
define('SITE_SAFE_STATE', ROOT."data/close.state");
define('SITE_SAFE_SQL', ROOT."data/sql.run");
define('SITE_SAFE_TEMPLATE', ROOT."data/template.edit");


//$nocache=1;

if($action != 'gethashimg') {
	/*header("Vary: Accept");
	if(!empty($_SERVER['HTTP_ACCEPT']) && stristr($_SERVER['HTTP_ACCEPT'], "application/xhtml+xml"))
		header("Content-Type: application/xhtml+xml; charset=".SYSCHARSET);
	else
		header("Content-Type: text/html; charset=".SYSCHARSET);*/

	header("Content-Type: text/html; charset=".SYSCHARSET);
}

/*if(!empty($nocache)) {
	header("Expires: 0");
	header("Cache-Control: private, post-check=0, pre-check=0, max-age=0", false);
	header("Pragma: no-cache");
}else {
	session_cache_limiter('public');
	header("Cache-control: public");
	header("Expires: 100");
}*/

if(defined('IN_Read8_ACP') || defined('IN_Read8_TPWT') || !empty($nocache)) {
	header("Expires: 0");
	header('Cache-Control: no-cache, must-revalidate');
	header('Pragma: no-cache');
}

if(empty($obno)) {
	if(/*$ Global['gzip'] && */empty($obnocallback) && function_exists('ob_gzhandler') && !$noheader) {
		ob_start('ob_gzhandler');
	}else {
		ob_start();
	}
}

//约定变量定义
$PHP_SELF = $_SERVER['PHP_SELF'] ? $_SERVER['PHP_SELF'] : $_SERVER['SCRIPT_NAME'];
$Global['F_userip'] = !empty($_SERVER['HTTP_CLIENT_IP']) ? $_SERVER['HTTP_CLIENT_IP'] : (!empty($_SERVER['HTTP_X_FORWARDED_FOR']) ? $_SERVER['HTTP_X_FORWARDED_FOR'] : $_SERVER['REMOTE_ADDR']);
$Global['F_longip'] = ip2longencode($Global['F_userip']);
$Global['F_tdtime'] = rtodaytime();

$Global['style'] = 'basic';

$seokeyword = array();
$cachelost = $sqladd = '';

//require CacheFile('group');
//require CacheFile('booktype');
//require CacheFile('banlist');
if(isset($_CACHE['banlist']['ip']) && ipAccess($_CACHE['banlist']['ip'])) exit('很抱歉，您已被禁止访问本站');


$db->execUpBySql("DELETE FROM ".__TAB_SESS__." WHERE expire < ".TIMESTAMP);


$sid = _cookie('sid');

$session_exists = false;
if($sid) {
	$_a = $db->selectOne(array(
		'from'  => __TAB_SESS__,
		'where' => array(
			'sid' => $sid,
		),
	));
	if($_a) {
		$session_exists = true;
	}
}

$Global['F_uid']      = 0;
$Global['F_username'] = '游客';

if(!$session_exists) {
	if(!$sid) {
		$sid = md5($Global['F_userip'].rrand(20));
	}

	$lnfo = _cookie_decode('lnfo', 1);
	if($lnfo && 2 == count($lnfo)) {
		list($_u, $_p) = $lnfo;
		$_a = CheckPass($_u, $_p);
		if(is_array($_a)) {
			$Global['F_uid']      = $_a['uid'];
			$Global['F_username'] = $_a['username'];
		}
	}

	$db->execUpBySql("REPLACE INTO ".__TAB_SESS__." VALUES('$sid', '$Global[F_userip]', '$Global[F_uid]', '$Global[F_username]', '".(TIMESTAMP+900)."', '')");
}else {
	$Global['F_uid']      = $_a['uid'];
	$Global['F_username'] = $_a['username'];
}

cookie('sid', $sid, 900);
$_SID = $sid;

if($Global['F_uid']<=0) {//游客权限
	$Global['F_uid'] = 0;
	$Global['F_username'] = '游客';
}

//得到访问者语言设定


$Global['lang']='chs';

if(Banlist($Global['F_username'], 'username')) back('您的用户名已被封禁');//用户名已被封禁

if(file_exists(SITE_SAFE_STATE)) back($Global['siteclosereason']);


if(IN_UCP || IN_AUCP) {
	if(!$Global['F_uid']) {
		if($ajax) {
			header('Content-Type: application/json; charset='.SYSCHARSET);
			echo "alert('请先登录');";
		}else {
			_login('/'.(IN_UCP ? 'my' : 'author').'/'.PHP_SELF.'.php');
		}
		exit;
	}
}


include _class('style',   'system');
include _class('license', 'system');
include _class('state',   'system');

include _class('subject', 'novel');
include _class('content', 'novel');

include _class('subject', 'story');
include _class('content', 'story');


include _config('seo');

require_once ROOT.'include/header.inc.php';
