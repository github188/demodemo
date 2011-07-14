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
if(!defined('IN_Read8') || !defined('IN_Read8_ACP') || !isset($PHP_SELF)) {
	exit('Access Denied (typewriter.inc.php)');
}



list($pf, $ps) = explode('_', $pfile);

$db->single(__TAB_USER_TPWT__);

if(!defined('PAGE_SIZE')) {
	define('PAGE_SIZE', 20);
}


if(in_array($pf, array('typewriter')) && in_array($ps, array('add', 'edit'))) {
	define('IN_Read8_ACP_'.strtoupper($pf), true);
	require ROOT.'admin/'.$pf.'/'.$pfile.'.php';
}