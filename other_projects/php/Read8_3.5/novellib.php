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

$page = 1;
$subject = $content = $initial = $words = $finished = $orderbycreate = '';

define('IN_Read8', true);
require_once str_replace(array('\\', '/'.basename(__FILE__)), array('/', ''), __FILE__).'/include/init.php';

_header(array($orderbycreate ? '最新入库' : '大众书库'));//输出头部


if(!ris_int($subject))
	$subject = 0;

if(!ris_int($content))
	$content = 0;

$initial = strtoupper($initial);
$chr = ord($initial);
if($chr < 48 || $chr > 57 && $chr < 65 || $chr > 90)
	$initial = '';

if(!ris_int($words))
	$words = 0;

$_assign['nsubject'] = $subject;
$_assign['ncontent'] = $content;
$_assign['ninitial'] = $initial;
$_assign['nwords']   = $words;

$tpl = new AOKOTpl('novellib');

	$attributes = array('subject', 'content');
	foreach($attributes as $attribute) {
		$tmp = array();
		foreach($_CATEGORY['NOVEL'][$attribute] as $v) {
			$v['selected'] = $$attribute == $v['id'] ? ' selected="selected"' : '';
			$tmp[] = $v;
		}

		$tpl->assign($attribute.'s', $tmp);
	}

	$tmp = array();
	for($i = 65; $i < 91; $i++) {
		$tmp[] = array(
			'v'        => strtoupper(chr($i)),
			'selected' => $initial == strtoupper(chr($i)) ? ' selected="selected"' : '',
		);
	}
	for($i = 0; $i <= 9; $i++) {
		$tmp[] = array(
			'v' => $i,
			'selected' => (strlen($initial) && $initial == $i) ? ' selected="selected"' : '',
		);
	}

	$tpl->assign('initial', $tmp);

	$tmp = array();
	for($i = 2; $i <= 20; $i++) {
		$tmp[] = array(
			'v' => $i,
			'selected' => $words == $i ? ' selected="selected"' : '',
		);
	}

	$tpl->assign('words', $tmp);

$tpl->assign(array(
	'novels'   => _novel($subject, $content, $finished, $initial, $words, $orderbycreate),
	'pagelink' => page($db->counter, 20, 0, 1),
));

$tpl->output();

_footer();
