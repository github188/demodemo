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
	exit('Access Denied');
}

list($pf, $ps) = explode('_', $pfile);

if(!in_array($ps, array('acp', 'php', 'mysql')))
	back('暂不提供');

$cols = array();
$cols[] = array('col' => '时间');
$cols[] = array('col' => '用户');
$cols[] = array('col' => '用户名');
$cols[] = array('col' => 'IP');

switch($ps) {
	case 'acp':
		$cols[] = array('col' => '栏目');
		$cols[] = array('col' => '功能');
		$cols[] = array('col' => 'GET');
		$cols[] = array('col' => 'POST');
		$cols[] = array('col' => 'COOKIE');
		$cols[] = array('col' => 'FILES');
		break;
	case 'php':
		$cols[] = array('col' => '错误号');
		$cols[] = array('col' => '描述');
		$cols[] = array('col' => '文件');
		$cols[] = array('col' => '行');
		//$cols[] = array('col' => '数据');
		break;
	case 'mysql':
		$cols[] = array('col' => '错误');
		$cols[] = array('col' => 'SQL');
		break;
}

$tpl = new AOKOTpl('log');
$tpl->assign('cols', $cols);

$Lang = GetLang(1);

$logs = explode('<'.'?php exit; ?'.'>', rfor(ROOT.LOG_PATH.'/'.gmdate('Ymd', TIMESTAMP).'_'.$ps.'.php'));
rsort($logs);
foreach($logs as $k => $v) {
	$log = array();
	$i = 0;
	foreach(explode(HT, $v, count($cols)) as $logval) {
		if($i == 0)
			$logval = rdate($logval);

		switch($ps) {
			case 'acp':
				if(4 == $i) {
					$logval = Lang($logval, 'admin_category_');
				}elseif(5 == $i) {
					$logval = Lang($logval, 'admin_menu_');
				}elseif(6 == $i || 7 == $i || 8 == $i || 9 == $i) {
					$logval = _ellipsis(base64_decode($logval), 10);
				}
				break;
			case 'php':
				if(8 == $i)
					continue 2;

				break;
		}
		$log[] = array('val' => $logval);
		$i++;
	}
	$logs[$k] = array('log' => $log);
}

$tpl->assign('logs', $logs);
$tpl->output();

