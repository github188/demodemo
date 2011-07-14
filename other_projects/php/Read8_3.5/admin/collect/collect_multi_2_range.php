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
	exit('Access Denied (collect_multi_2_range.php)');
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


if(empty($range_target) || !_collect_mark($range_target)) {
	_collect_error('请选择要使用的规则');
}

if(empty($range_start) || empty($range_end)) {
	_collect_error('请指定书籍编号范围');
}

if(!ris_int($range_start) || !ris_int($range_end)) {
	_collect_error('书籍编号只能为整数');
}

$bids = range($range_start, $range_end);


_collect_js();

ob_end_flush();
ob_implicit_flush();

$tpl = new AOKOTpl($pfile.'_step_'.$step);
$tpl->output();


_collect_output_js('display_info(\'取得书籍编号集\', \'final\');');

$bc = count($bids);
$bids = array_unique($bids);
$bac = count($bids);
_collect_output_js('update_info(\'取得书籍编号集\', \'共找到 '.$bc.' 本书, 其中 '.($bc - $bac).' 本重复,  '.$bac.' 本有效\', \'final\', \'Green\');');


_collect_output_js('display_info(\'写入缓存\', \'wchd\');');


// multi collect cache
$chd  = '/*'.LF.
		HT.'rule:   '.wordwrap(base64_encode(serialize($bids)), 60, LF.HT.HT.HT, true).LF.
		'*/'.LF.
		LF;

$chd .= '$read8bids = '.var_export($bids, true).';'.LF;
$chd .= LF;
$chd .= '$targets = '.var_export(array_fill(0, $bac, $range_target), true).';'.LF;
$chd .= LF;
$chd .= '$jump = '.var_export($jump, true).';'.LF;
$chd .= LF;
$chd .= '$clear = '.var_export($clear, true).';'.LF;
$chd .= LF;
$chd .= '$multi_cc = '.var_export($bac, true).';'.LF;
$chd .= LF;
_collect_data($multi, $range_target, $chd, true);

_collect_output_js('update_info(\'写入缓存\', \'完成\', \'wchd\', \'Green\');');

