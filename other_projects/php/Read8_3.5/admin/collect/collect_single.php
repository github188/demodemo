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
	exit('Access Denied (collect_single.php)');
}

require_once ROOT.'include/collect.func.php';

if(empty($step))
	$step = 1;

if($step > 5)
	back('非法采集操作！');


$steps = array();
$steps[] = array(
	's' => 1,
	't' => '选择目标站点和书籍',
	'c' => $step == 1 ? 'green' : 'gray',
);
$steps[] = array(
	's' => 2,
	't' => '确认书籍基本信息',
	'c' => $step == 2 ? 'green' : 'gray',
);
$steps[] = array(
	's' => 3,
	't' => '开始采集',
	'c' => $step == 3 ? 'green' : 'gray',
);
$steps[] = array(
	's' => 4,
	't' => '整理数据',
	'c' => $step == 4 ? 'green' : 'gray',
);
$steps[] = array(
	's' => 5,
	't' => '完成',
	'c' => $step == 5 ? 'green' : 'gray',
);

$tpl = new AOKOTpl($pfile.'_step');
$tpl->assign('steps', $steps);
$tpl->output();


require ROOT.'admin/'.$pf.'/'.$pfile.'_'.$step.'.php';
