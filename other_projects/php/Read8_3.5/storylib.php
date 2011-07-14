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
$subject = $content = '';

define('IN_Read8', true);
require_once str_replace(array('\\', '/'.basename(__FILE__)), array('/', ''), __FILE__).'/include/init.php';

_header(array('短篇文学'));//输出头部


if(!ris_int($subject))
	$subject = 0;

if(!ris_int($content))
	$content = 0;

$_assign['nsubject'] = $subject;
$_assign['ncontent'] = $content;

$tpl = new AOKOTpl('storylib');

	$attributes = array('subject', 'content');
	foreach($attributes as $attribute) {
		$tmp = array();
		foreach($_CATEGORY['STORY'][$attribute] as $v) {
			$v['selected'] = $$attribute == $v['id'] ? ' selected="selected"' : '';
			$tmp[] = $v;
		}

		$tpl->assign($attribute.'s', $tmp);
	}

$tpl->assign(array(
	'novels'   => _story($subject, $content),
	'pagelink' => page($db->counter, 20, 0, 1),
));

$tpl->output();

_footer();
