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
	短信发件箱 - 用户中心
*/
define('IN_UCP', true);
define('PHP_SELF_EXT', '_outbox');
define('PHP_SELF', basename(__FILE__, PHP_SELF_EXT.'.php'));

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

$tpl = new AOKOTpl('my_'.PHP_SELF.PHP_SELF_EXT);

$tpl->assign(array(
	'current'   => 1,
));

$tpl->assign(array(
	'pinfo'    => $db->select(array(
						'field' => '`id`, `to`, `title`, `content`, `dateline`',
						'where' => array(
							'from_id'    => $Global['F_uid'],
							'isinbox'    => 0,
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