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
	推荐 - 作家中心
*/

define('IN_AUCP', true);
define('PHP_SELF_EXT', '_info_update');
define('PHP_SELF', basename(__FILE__, PHP_SELF_EXT.'.php'));


$acs = array();
$actitle = '';

require_once '../include/init.php';
require_once ROOT.'author/category.inc.php';



$db->single(__TAB_MY_AUT_CMD__);


if($actitle) {
	if(!$bid = novel_get_id_by_name($actitle)) {
		back('未找到该书');
	}
	$db->insert(array(
		'author_id' => $_AUTHOR['id'],
		'book_id'   => $bid,
	));
}

if(!empty($acs))
	foreach($acs as $k => $v)
		if(!ris_int($v))
			unset($acs[$k]);

if(!empty($acs)) {
	$db->delete(array(
		'author_id' => $_AUTHOR['id'],
		'book_id'   => $acs,
	));
}

header('Location: commend.php');
exit;

