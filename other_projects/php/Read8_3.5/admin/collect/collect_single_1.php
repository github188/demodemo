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
if(!defined('IN_Read8') || !defined('IN_Read8_ACP') || !defined('IN_Read8_ACP_COLLECT') || !isset($PHP_SELF)) {
	exit('Access Denied (collect_single_1.php)');
}

if(empty($target) || !ris_int($target))
	$target = 0;


$sites = _collect_site();
foreach($sites as $k => $v) {
	$sites[$k]['selected'] = $v['id'] == $target ? ' selected="selected"' : '';
}

$tpl = new AOKOTpl($pfile.'_step_'.$step);
$tpl->assign('sites', $sites);
$tpl->output();

