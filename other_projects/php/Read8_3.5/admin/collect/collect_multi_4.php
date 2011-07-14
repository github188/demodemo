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
	exit('Access Denied (collect_multi_4.php)');
}

if(empty($multi) || !in_array($multi, array('regexp', 'text', 'range'))) {
	_collect_error('请选择采集方式');
}



_collect_js();

$tpl = new AOKOTpl($pfile.'_step_'.$step);
$tpl->output();


_collect_output_js('display_info(\'清除缓存\', \'cache\');');
unlink(_collect_data($multi, ${$multi.'_target'}, '', true));
_collect_output_js('update_info(\'清除缓存\', \'完成\', \'cache\', \'Green\');');


_collect_output_js('tick(5, \'?ptype='.$ptype.'&pfile='.$pfile.'\');');
