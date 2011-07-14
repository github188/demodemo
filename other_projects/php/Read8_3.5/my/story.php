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
/*
	短篇文学
*/

define('IN_UCP', true);
define('PHP_SELF', basename(__FILE__, '.php'));


require_once '../include/init.php';
require_once ROOT.'my/category.inc.php';

_header(array($_SYSTEM['CATEGORY'][PHP_SELF]));

$tpl = new AOKOTpl('navigation_ucp');
$tpl->assign('categories', $category);
$tpl->output();

$tpl = new AOKOTpl('my_'.PHP_SELF);
$tpl->assign(array(
	'stories' => html_show($db->select(array(
		'field' => 'id, title, author, description, subject, content, state, dateline, lastupdate',
		'from'  => __TAB_STORY__,
		'where' => array(
			'typewriter' => $Global['F_uid'],
			'state'      => array(3, 6)
		),
		'filter'=> 'convert_story_classes',
	)), false),
));
$tpl->output();

_footer();