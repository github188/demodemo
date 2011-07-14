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
	exit('Access Denied (collect_single_4.php)');
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


_collect_output_js('display_info(\'整理数据\', \'tidy\');');

novel_volume_tidy($bid);
_collect_output_js('update_info(\'整理数据\', \'生成目录\', \'tidy\', \'Blue\');');

novel_rebuild_index($bid);
_collect_output_js('update_info(\'整理数据\', \'完成\', \'tidy\', \'Green\');');


_collect_output_js('tick(5, \'?ptype='.$ptype.'&pfile='.$pfile.'&step=5&target='.$target.'&read8bid='.$read8bid.'&multi='.$multi.'\');');
