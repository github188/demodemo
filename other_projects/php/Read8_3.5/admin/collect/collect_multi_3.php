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
	exit('Access Denied (collect_multi_3.php)');
}

if(empty($jump)) {
	$jump  = 20;
}
if(empty($clear)) {
	$clear = 0;
}
if(empty($sp)) {
	$sp = 0;
}

cookie('jump',  $jump,  31536000);
cookie('clear', $clear, 31536000);

if(empty($multi) || !in_array($multi, array('regexp', 'text', 'range'))) {
	_collect_error('请选择采集方式');
}

if(!(include _collect_data($multi, ${$multi.'_target'}, '', true))) {
	_collect_error('读取批量采集缓存失败');
}

_collect_js();

ob_end_flush();
ob_implicit_flush();

$tpl = new AOKOTpl($pfile.'_step_'.$step);
$tpl->output();



if(empty($multi_pos) || !isset($read8bids[$multi_pos - 1])) {
	_collect_output_js('tick(5, \'?ptype='.$ptype.'&pfile='.$pfile.'&step=4&multi='.$multi.'&'.$multi.'_target='.${$multi.'_target'}.'\');');
	return;
}

$target   = _collect_mark($targets[$multi_pos - 1]);
$read8bid = $read8bids[$multi_pos - 1];



_collect_output_js('create_progress(true, \'批量采集状态\');');
_collect_output_js('refresh_progress(true, '.(round(($multi_pos - 1) / $multi_cc * 100)).');');


_collect_output_js('update_info(\'正在采集 '.$targets[$multi_pos - 1].' No.'.$read8bid.'\', \'读取基本信息\', \'multi\', \'Blue\');');




if(!$sp) {
	_collect_output_js('display_info(\'读取基本信息\', \'multi_'.$target.'_'.$read8bid.'_info\');');
	if(!(require ROOT.'admin/'.$pf.'/'.$pf.'_single_2.php')) {
		_collect_output_js('update_info(\'正在采集 '.$targets[$multi_pos - 1].' No.'.$read8bid.'\', \'跳过\', \'multi\', \'Gray\');');
		_collect_output_js('tick(5, \'?ptype='.$ptype.'&pfile='.$pfile.'&step=3&multi='.$multi.'&'.$multi.'_target='.${$multi.'_target'}.'&jump='.$jump.'&clear='.$clear.'&multi_pos='.($multi_pos + 1).'\');');
		return;
	}
}

_collect_output_js('update_info(\'正在采集 '.$targets[$multi_pos - 1].' No.'.$read8bid.'\', \'开始采集\', \'multi\', \'Blue\');');
_collect_output_js('display_info(\'开始采集\', \'multi_'.$target.'_'.$read8bid.'_start\');');

if(!(require ROOT.'admin/'.$pf.'/'.$pf.'_single_3.php')) {
	_collect_output_js('update_info(\'正在采集 '.$targets[$multi_pos - 1].' No.'.$read8bid.'\', \'跳过\', \'multi\', \'Gray\');');
	_collect_output_js('tick(5, \'?ptype='.$ptype.'&pfile='.$pfile.'&step=3&multi='.$multi.'&'.$multi.'_target='.${$multi.'_target'}.'&jump='.$jump.'&clear='.$clear.'&multi_pos='.($multi_pos + 1).'\');');
	return;
}

