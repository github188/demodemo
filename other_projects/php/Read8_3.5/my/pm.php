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
	未查看的短信 - 用户中心
*/
define('IN_UCP', true);
define('PHP_SELF', basename(__FILE__,'.php'));

$action = $id = "";
$page = 1;

require_once '../include/init.php';
require_once ROOT.'my/category.inc.php';

//$db->debug='ture';

$db->single(__TAB_PM__);

_header(array($_SYSTEM['CATEGORY'][PHP_SELF]));

$tpl = new AOKOTpl('navigation_ucp');
$tpl->assign('categories', $category);
$tpl->output();

if(!defined('PAGE_SIZE')) {
	define('PAGE_SIZE', 20);
}

$tpl = new AOKOTpl('my_'.PHP_SELF);
$tpl->assign(array(
	'no_info'    => $db->select(array(
						'field' => '`id`, `from`, `title`, `content`, `dateline`, `to`',
						'where' => array(
							'to_id'    => $Global['F_uid'],
							'isread'   => 0,
							'isinbox'  => 1,
						),
						'order' => 'dateline DESC',
						'limit' => PAGE_SIZE,
						'page'  => $page,
						'filter'=> 'html_show',
						'counter'=>1,
					)),
	'pagelink' => page($db->counter, PAGE_SIZE),
));
$tpl->output();

_footer();