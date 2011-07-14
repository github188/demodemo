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
	exit('Access Denied (collect_multi_2_regexp.php)');
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


if(empty($regexp_target) || !ris_int($regexp_target)) {
	_collect_error('请选择要使用的规则');
}

$multi_rule = _collect_rule($regexp_target, true);

$target = _collect_mark($multi_rule['parent']);
if(!$target) {
	_collect_error('该规则的基础规则不存在');
}

$rule = _collect_rule($target);


$use = array(
	'read8page',
);



_collect_js();

ob_end_flush();
ob_implicit_flush();

$tpl = new AOKOTpl($pfile.'_step_'.$step);
$tpl->output();


if(false === strpos($multi_rule['page_list'], '{read8page}')) {
	$multi_rule['page_start'] = $multi_rule['page_end'] = 0;
}else {
	if($multi_rule['page_end'] < $multi_rule['page_start']) {
		$multi_rule['page_end'] = $multi_rule['page_start'];
	}
}

$read8page = $multi_rule['page_start'];


$bids = array();
$bc = 0;

do {
	_collect_output_js('display_info(\'取得书籍编号集 页码: '.$read8page.'\', '.$read8page.');');

	_collect_output_js('update_info(\'取得书籍编号集 页码: '.$read8page.'\', \'读取书籍列表页面\', '.$read8page.', \'Blue\');');

	$address = _collect_get_address($multi_rule['page_list'], $use);
	$page_list = _collect_get_page($rule, $address);
	if(!$page_list) {
		_collect_output_js('update_info(\'取得书籍编号集 页码: '.$read8page.'\', \'读取书籍列表页面 失败\', '.$read8page.', \'Red\');');
		continue;
	}

	$bids_info = _collect_get_info_by_regexp($page_list, $multi_rule, 'bid');
	if(!isset($bids_info[1])) {
		_collect_output_js('update_info(\'取得书籍编号集 页码: '.$read8page.'\', \'未在本页找到书籍\', '.$read8page.', \'Red\');');
		continue;
	}

	$bids = array_merge($bids, $bids_info[1]);
	$bic = count($bids_info[1]);
	$bc += $bic;
	_collect_output_js('update_info(\'取得书籍编号集 页码: '.$read8page.'\', \'找到 '.$bic.' 本书\', '.$read8page.', \'Green\');');

	$read8page++;
} while($read8page <= $multi_rule['page_end']);

_collect_output_js('display_info(\'取得书籍编号集\', \'final\');');
if(!$bc) {
	_collect_output_js('update_info(\'取得书籍编号集\', \'未找到任何书籍\', \'final\', \'Red\');');
	_collect_error('未能在目标页面找到任何书籍');
}

$bids = array_unique($bids);
$bac = count($bids);
_collect_output_js('update_info(\'取得书籍编号集\', \'共找到 '.$bc.' 本书, 其中 '.($bc - $bac).' 本重复,  '.$bac.' 本有效\', \'final\', \'Green\');');


_collect_output_js('display_info(\'写入缓存\', \'wchd\');');

// multi collect cache
$chd  = '/*'.LF.
		HT.'rule:   '.wordwrap(base64_encode(serialize($multi_rule)), 60, LF.HT.HT.HT, true).LF.
		'*/'.LF.
		LF;

$chd .= '$read8bids = '.var_export($bids, true).';'.LF;
$chd .= LF;
$chd .= '$targets = '.var_export(array_fill(0, $bac, $multi_rule['parent']), true).';'.LF;
$chd .= LF;
$chd .= '$jump = '.var_export($jump, true).';'.LF;
$chd .= LF;
$chd .= '$clear = '.var_export($clear, true).';'.LF;
$chd .= LF;
$chd .= '$multi_cc = '.var_export($bac, true).';'.LF;
$chd .= LF;
_collect_data($multi, $regexp_target, $chd, true);

_collect_output_js('update_info(\'写入缓存\', \'完成\', \'wchd\', \'Green\');');

