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
	exit('Access Denied (collect_multi_1.php)');
}

if(empty($target) || !ris_int($target))
	$target = 0;



$tpl = new AOKOTpl($pfile.'_step_'.$step);

$multi = _cookie('multi');
if(!$multi) {
	$multi = 'regexp';
}


$typewriter    = _cookie('typewriter');
$typewriter_id = _cookie('typewriter_id');
if(!$typewriter || !$typewriter_id || !$db->selectOne(array(
	'field' => 'uid',
	'from'  => __TAB_USER__,
	'where' => array(
		'uid'      => $typewriter_id,
		'username' => $typewriter,
	),
))) {
	$typewriter    = $Global['F_username'];
	$typewriter_id = $Global['F_uid'];
}


$files = array();
foreach(rlistfile(ROOT.'data/task') as $k => $v) {
	if(preg_match('/^([A-Za-z0-9]+)\.txt$/', $v, $match)) {
		$files[] = array(
			'f' => $match[1],
		);
	}
}


$tpl->assign(array(
	'sites'        => _collect_site(true),
	'texts'        => $files,
	'single_sites' => _collect_site(),

	'multi_regexp_c' => $multi == 'regexp' ? ' checked="checked"' : '',
	'multi_text_c'   => $multi == 'text'   ? ' checked="checked"' : '',
	'multi_range_c'  => $multi == 'range'  ? ' checked="checked"' : '',

	'multi_regexp_d' => $multi != 'regexp' ? ' style="display: none;"' : '',
	'multi_text_d'   => $multi != 'text'   ? ' style="display: none;"' : '',
	'multi_range_d'  => $multi != 'range'  ? ' style="display: none;"' : '',

	'typewriter' => $typewriter,

	'jump'  => _cookie('jump')  ? _cookie('jump')  : 20,
	'clear' => _cookie('clear') ? _cookie('clear') : 0,
));
$tpl->assign('single_sites', _collect_site());
$tpl->output();

