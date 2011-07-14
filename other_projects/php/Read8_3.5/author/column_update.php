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
	专栏设置 - 作家中心
*/

define('IN_AUCP', true);
define('PHP_SELF_EXT', '_update');
define('PHP_SELF', basename(__FILE__, PHP_SELF_EXT.'.php'));


$column_name = $intro = $announcement = $avatar = '';

require_once '../include/init.php';
require_once ROOT.'author/category.inc.php';




$db->update(array(
	'column_name'  => $column_name,
	'intro'        => $intro,
	'announcement' => $announcement,
	'avatar'       => $avatar,
), array(
	'id'           => $_AUTHOR['id'],
), __TAB_MY_AUT__);



header('Location: column.php');
exit;

_header(array($_SYSTEM['CATEGORY'][PHP_SELF]));

$tpl = new AOKOTpl('navigation_aucp');
$tpl->assign('categories', $category);
$tpl->output();


$tpl = new AOKOTpl('author_'.PHP_SELF);
$tpl->assign($_AUTHOR);
$tpl->output();

_footer();