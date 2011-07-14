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
	查看全部 - 用户中心
*/
define('IN_UCP', true);
define('PHP_SELF_EXT', '_check_content');
define('PHP_SELF', basename(__FILE__, PHP_SELF_EXT.'.php'));

$aid = $oid = "";

require_once '../include/init.php';
require_once ROOT.'my/category.inc.php';

//$db->debug='ture';

$db->single(__TAB_PM__);

_header(array($_SYSTEM['CATEGORY'][PHP_SELF]));

$tpl = new AOKOTpl('navigation_ucp');
$tpl->assign('categories', $category);
$tpl->output();


$tpl = new AOKOTpl('my_'.PHP_SELF.PHP_SELF_EXT);

if($aid){
	$tpl->assign($db->selectOne(array(
		'field' => '`id`, `from`, `title`, `content`, `dateline`',
		'where' => array(
			'id'    => $aid,
		),
		'filter' => 'html_show',
	)));

	$tpl->output();

	$db->update(array(
		'isread'=> 1,
	),array('id'=>$aid));

}

if($oid){

		$tpl->assign($db->selectOne(array(
			'field' => '`id`, `to`, `title`, `content`, `dateline`',
			'where' => array(
				'id'    => $oid,
			),
			'filter' => 'html_show',
		)));

	$tpl->output();
}

_footer();