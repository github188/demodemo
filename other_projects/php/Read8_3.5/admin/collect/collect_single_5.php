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
	exit('Access Denied (collect_single_5.php)');
}

if(empty($multi)) {
	$multi = 0;
}

if(empty($target) || !ris_int($target)) {
	if($multi) {
		return;
	}else {
		_collect_error('请选择目标站');
	}
}
if(empty($read8bid) || !ris_int($read8bid)) {
	if($multi) {
		return;
	}else {
		_collect_error('请设置目标书籍');
	}
}

if(!(include _collect_data($target, $read8bid))) {
	if($multi) {
		return;
	}else {
		_collect_error('读取采集缓存失败');
	}
}

if(!$novel_exists) {
	$novel_exists = novel_exists($b['title']);
}
$bid = $novel_exists['bid'];



_collect_js();

$tpl = new AOKOTpl($pfile.'_step_'.$step);
$tpl->output();


_collect_output_js('display_info(\'清除缓存\', \'cache\');');
unlink(_collect_data($target, $read8bid));
_collect_output_js('update_info(\'清除缓存\', \'完成\', \'cache\', \'Green\');');


_collect_output_js('display_info(\'采集完成 <a href="'.novel_url($novel_exists).'" target="_blank" style="color: Red;">立即查看《'.$novel_exists['title'].'》</a>\', \'finished\');');

_justnow('CLT', 'SIN', $bid, caddslashes($novel_exists['title']));

_collect_output_js('tick(5, \'?ptype='.$ptype.'&pfile='.$pfile.'\');');
