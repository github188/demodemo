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
if(!defined('IN_Read8')) {
	exit('Access Denied (collect.func.php)');
}

/*
	采集函数库
*/

require_once ROOT.'include/string.func.php';


function _collect_error($msg, $halt = true){
	$tpl = new AOKOTpl('collect_error');
	$tpl->assign('msg', $msg);
	$tpl->output();

	if($halt) {
		acpfooter();
		exit;
	}
}

function _collect_get_address($address, $use){
	foreach($use as $v) {
		global $$v;

		if(!isset($$v))
			$$v = '';

		$address = str_replace('{'.$v.'}', $$v, $address);
	}
	return $address;
}

function _collect_get_page($rule, $address, $image = false){
	return _collect_convert($rule, rforr($address), $image);
}

function _collect_replace($rule, $content){
	foreach($rule['replacements'] as $v) {
		$v['f'] = str_replace(array('\n', '\r', '\t'), array(LF, CR, HT), $v['f']);
		$v['t'] = str_replace(array('\n', '\r', '\t'), array(LF, CR, HT), $v['t']);
		$content = $v['func']($v['f'], $v['t'], $content);
	}
	return $content;
}

function _collect_tag_split($content, $rule, $info){
	$tag = $rule[$info.'_start'];
	if(false === strpos($content, $tag)) {
		return array($content);
	}else {
		$splits = explode($tag, $content);
		foreach($splits as $k => $v)
			$k && $splits[$k] = $tag.$v;

		return $splits;
	}
}

function _collect_get_info_by_tag($content, $rule, $info){
	return rsubstr($content, $rule[$info.'_start'], $rule[$info.'_end']);
}

function _collect_get_info_by_regexp($content, $rule, $info){
	if(preg_match_all($rule[$info.'_regexp'], $content, $matches)) {
		return $matches;
	}else {
		return false;
	}
}

function _collect_get_general_regexp($content, $rule, $level, $limit = 5){
	global $use;

	if($rule['general_regexp_'.$level])
		preg_match($rule['general_regexp_'.$level], $content, $match);

	for($i = 1; $i <= $limit; $i++) {
		$use[] = 'read8g'.$level.'v'.$i;
		$gv['read8g'.$level.'v'.$i] = isset($match[$i]) ? $match[$i] : '';
	}
	return $gv;
}


function _collect_convert($rule, $content, $decompressOnly = false){
	if($rule['compression']) {
		switch($rule['compression']) {
			case 1:
				$compression = 0;
				break;
			case 2:
				$compression = 2;
				break;
			case 3:
				$compression = 3;
				break;
		}
	}else {
		$mime = substr($content, 0, 4);
		if("\x1F\x8B\x08\x00" == $mime) {
			$compression = 2;
		}else {
			$compression = 0;
		}
	}

	if($compression == 2) {
		$f = ROOT.'data/cache/collect_gzdata_'.$rule['id'].'_'.rrand(20);
		rfow($f, $content);
		$content = rgzr($f);
		unlink($f);
	}

	if($decompressOnly)
		return $content;

	if($rule['charsets']) {
		switch($rule['charsets']) {
			case 1:
				$charsets = 'utf-8';
				break;
			case 2:
				$charsets = 'gbk';
				break;
			case 3:
				$charsets = 'big5';
				break;
		}
	}else {
		if(preg_match('/<meta.+charset.+(gb2312|gbk|utf-8|big5)/i', $content, $match)) {
			$charsets = $match[1];
			if($charsets == 'gb2312')
				$charsets = 'gbk';
		}else {
			$charsets = 'gbk';
		}
	}

	if(strtolower($charsets) == SYSCHARSET) {
		return $content;
	}else {
		return convert($content, $charsets, SYSCHARSET);
	}
}

function _collect_data($target, $read8bid, $data = '', $multi = false){
	$f = 'data/cache/collect_data_'.($multi ? 'multi' : 'single').'_'.$target.'_'.$read8bid.'.php';
	if($data) {
		_cache_write(ROOT.$f, $target.'_'.$read8bid, $data, 'collect_data_');
	}else {
		return ROOT.$f;
	}
}

function _collect_rule($id, $multi = false){
	$f = ROOT.'data/cache/collect_rule_'.($multi ? 'multi' : 'single').'.php';

	if(!is_file($f)) {
		_cache_write($f, $multi ? 'multi' : 'single', _collect_rule_generate($multi), 'collect_rule_');
	}

	include $f;

	return $_COLLECT['RULE'][$multi ? 'MULTI' : 'SINGLE'][$id];
}

function _collect_rule_generate($multi = false){
	global $db;

	$rules = $db->select(array(
		'from'  => $multi ? __TAB_COLLECT_MULTI_RULE__ : __TAB_COLLECT_SINGLE_RULE__,
	));

	$data = array();
	foreach($rules as $rule) {
		if(!$multi) {
			$rule['subject_pos']  = unserialize(base64_decode($rule['subject_pos']));
			$rule['content_pos']  = unserialize(base64_decode($rule['content_pos']));
			$rule['replacements'] = unserialize(base64_decode($rule['replacements']));
		}
		$data[$rule['id']] = $rule;
	}

	return '$_COLLECT[\'RULE\'][\''.($multi ? 'MULTI' : 'SINGLE').'\'] = '.var_export($data, true).';';
}

function _collect_site($multi = false){
	$f = ROOT.'data/cache/collect_site_'.($multi ? 'multi' : 'single').'.php';

	if(!is_file($f)) {
		_cache_write($f, $multi ? 'multi' : 'single', _collect_site_generate($multi), 'collect_site_');
	}

	include $f;

	return $_COLLECT['SITE'][$multi ? 'MULTI' : 'SINGLE'];
}

function _collect_site_generate($multi = false){
	global $db;

	return  '$_COLLECT[\'SITE\'][\''.($multi ? 'MULTI' : 'SINGLE').'\'] = '.var_export($db->select(array(
				'field' => 'id, title, lastupdate, '.($multi ? 'parent' : 'mark, address'),
				'from'  => $multi ? __TAB_COLLECT_MULTI_RULE__ : __TAB_COLLECT_SINGLE_RULE__,
			)), true).';';
}

function _collect_mark($mark, $multi = false){
	$f = ROOT.'data/cache/collect_mark_'.($multi ? 'multi' : 'single').'.php';

	if(!is_file($f)) {
		_cache_write($f, $multi ? 'multi' : 'single', _collect_mark_generate($multi), 'collect_mark_');
	}

	include $f;

	if($multi) {
		return array_keys($_COLLECT['MARK']['MULTI'], $mark);
	}else {
		return array_search($mark, $_COLLECT['MARK']['SINGLE']);
	}
}

function _collect_mark_generate($multi = false){
	global $db;

	$rows = $db->select(array(
		'field' => 'id, '.($multi ? 'parent' : 'mark'),
		'from'  => $multi ? __TAB_COLLECT_MULTI_RULE__ : __TAB_COLLECT_SINGLE_RULE__,
	));
	foreach($rows as $row) {
		$data[$row['id']] = $row[$multi ? 'parent' : 'mark'];
	}

	return  '$_COLLECT[\'MARK\'][\''.($multi ? 'MULTI' : 'SINGLE').'\'] = '.var_export($data, true).';';
}


function _collect_rule_export($rule, $multi = false){
	global $_SYSTEM;

	if($multi) {
		$multi = '1';
	}else {
		$multi = '0';
	}
	$data = '# Read8.Net '.SYS_VERSION.' '.SYS_BUILD.' '.($multi ? 'multi' : 'single').' collect rule'.LF.
			'# Identify:    '.base64_encode($multi.','.SYS_VERSION).LF.
			'# From:        '.$_SYSTEM['SYSTEM']['SITE_ADDR'].LF.
			'# Target:      '.($multi ? $rule['parent'] : $rule['address']).LF.
			'# Last Update: '.rdate($rule['lastupdate']).LF.
			'# Export Date: '.rdate(TIMESTAMP).LF.
			'#'.LF.
			'# http://www.read8.net'.LF.
			'# Please visit our website for the latest information'.LF.
			'# ----------------------------------------------------------'.LF.
			LF.
			LF.
			LF.
			wordwrap(base64_encode(serialize($rule)), 80, LF, true);

	ob_end_clean();
	header('Cache-Control: no-cache, must-revalidate');
	header('Pragma: no-cache');
	header('Content-Encoding: none');
	header('Content-Length: '.strlen($data));
	header('Content-Disposition: attachment; filename='.($multi ? 'multi' : 'single').'_collect_rule_'.($multi ? $rule['parent'] : $rule['mark']).'_'.rdate(TIMESTAMP, 'Y_m_d_H_i_s').'.txt');
	header('Content-Type: '.(strpos($_SERVER['HTTP_USER_AGENT'], 'MSIE') ? 'application/octetstream' : 'application/octet-stream'));
	echo $data;
	exit;
}

function _collect_rule_import($data, $multi = false){
	list($rem, $import) = explode(LF.LF.LF.LF, str_replace(CR, '', $data));

	$rems = explode(LF, trim($rem));
	list($i_multi, $i_ver) = explode(',', base64_decode(substr($rems[1], 15)));

	if($i_multi != $multi)
		back('规则类型错误');

	if($i_ver != SYS_VERSION)
		back('该规则数据不适用于当前版本');

	$rule = unserialize(base64_decode($import));
	if(!is_array($rule))
		back('数据类型错误');

	return caddslashes($rule);
}


function _collect_js(){
	$tpl = new AOKOTpl('collect_js');
	$tpl->output();
}

function _collect_output_js($code){
	echo '<script type="text/javascript">'.$code.'</script>';
}

function _collect_display_info($info){
	//
}





function _collect_blacklist($blacklist = false){
	$f = ROOT.'data/blacklist.php';
	if(!is_file($f)) {
		rfow($f, '');
	}

	if(false === $blacklist) {
		$blacklist = array();
		include $f;
		return $blacklist;
	}else {
		return rfow($f, '<'.'?php'.LF.LF.'$blacklist = '.var_export($blacklist, true).';');
	}
}

