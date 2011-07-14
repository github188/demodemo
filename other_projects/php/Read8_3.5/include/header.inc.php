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
//页头程序

if(!defined('IN_Read8')) {
	exit('Access Denied (header.inc.php)');
}

//设定在所有模板中自动被替换的变量集

global $Global, $_SYSTEM;

$_assign = array(
	'F_uid' => $Global['F_uid'],
	'F_username' => $Global['F_username'],
	'G_sitename' => $_SYSTEM['SYSTEM']['SITE_NAME'],
	'G_siteurl' => $_SYSTEM['SYSTEM']['SITE_ADDR'],
	'G_style' => $_SYSTEM['DISPLAY']['template'],
	'G_tpldir' => $_SYSTEM['DISPLAY']['template'],
	'G_PHPSELF' => $_SERVER['HTTP_HOST'],
	'G_encode' => SYSCHARSET,
	'TIMESTAMP' => TIMESTAMP,
);

//页头输出
function _header($seokeyword, $type=0){
	global $_SYSTEM, $_CATEGORY;

	//二次输出检查
	if(defined('HEADER_SENDED')) {
		return;
	}else {
		define('HEADER_SENDED', true);
	}

	$seokeyword = preg_replace("/(\w+)/e","Lang('\\1', 'seo_')", $seokeyword);
	$seokeyword[] = $_SYSTEM['SYSTEM']['SITE_NAME'];
	$tpl = new AOKOTpl('header');
	//$tpl->assign('CAD_header',getad('header'));
	//$tpl->assign('Booktypeblock', $GLOBALS['_CACHE']['booktype']);
	$tpl->assign(array(
		'G_bbsurl'          => $_SYSTEM['SYSTEM']['BBS_URL'],
		'title'             => implode(' - ', $seokeyword).(empty($_SYSTEM['SEO']['title']) ? '' : ' | '.$_SYSTEM['SEO']['title']),
		'keywords'          => implode(', ', $seokeyword).(empty($_SYSTEM['SEO']['keywords']) ? '' : ', '.$_SYSTEM['SEO']['keywords']),
		'description'       => empty($_SYSTEM['SEO']['description']) ? '' : $_SYSTEM['SEO']['description'],
		'novel_cats'        => array_slice($_CATEGORY['NOVEL']['subject'], 0, 14),
		'story_cats'        => array_slice($_CATEGORY['STORY']['subject'], 0, 6),
	));
	if($type==0) $tpl->output();
	else return $tpl->output(1);
}

//页尾输出
function _footer(){
	global $_SYSTEM, $db, $starttime;

	$tpl = new AOKOTpl('footer');
	$mtime = explode(' ', microtime());
	$totaltime = number_format(($mtime[1] + $mtime[0] - $starttime), 6);

	$copyright        = str_replace(array(
		'{SITE_NAME}',
		'{SITE_ADDR}',
		'{BBS_URL}',
		'{NOW_YEAR}',
	), array(
		$_SYSTEM['SYSTEM']['SITE_NAME'],
		$_SYSTEM['SYSTEM']['SITE_ADDR'],
		$_SYSTEM['SYSTEM']['BBS_URL'],
		rdate(TIMESTAMP, 'Y'),
	), $_SYSTEM['SYSTEM']['copyright']);

	$copyright_system = str_replace(array(
		'{read8cn}',
		'{read8en}',
		'{read8ver}',
	), array(
		'<a href="http://www.read8.net" rel="external" id="by">读吧书网</a>',
		'<a href="http://www.read8.net" rel="external" id="by">Read8.Net</a>',
		SYS_VERSION,
	), $_SYSTEM['SYSTEM']['copyright_system']);

	$tpl->assign(array(
		'G_bbsurl'     => $_SYSTEM['SYSTEM']['BBS_URL'],
		'G_miibeian'   => $_SYSTEM['SYSTEM']['miibeian'],
		'G_statcode'   => $_SYSTEM['DISPLAY']['statcode'],
		'G_SpeedTrace' => $_SYSTEM['DISPLAY']['speedtrace'],
		'C_totaltime'  => $totaltime,
		'C_dbcount'    => $db->count(),

		'copyright'        => $copyright,
		'copyright_system' => sn() ? '' : $copyright_system,
	));
	$tpl->output();

	//$Gccheck = ROOT.'template/'.$Global['style'].'/footer.html';
	//if(!is_file($Gccheck)) $Gccheck = ROOT.'template/basic/footer.html';
	//$Gccheck = rfor($Gccheck);
	//if(!strstr($Gccheck,'{G_Copyright}') && !sn()) echo($G_copyright);

	ob_end_flush();
}

