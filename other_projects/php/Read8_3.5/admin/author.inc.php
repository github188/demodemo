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
	exit('Access Denied (author.inc.php)');
}



list($pf, $ps) = explode('_', $pfile);

$db->single(__TAB_MY_AUT__);

if(!defined('PAGE_SIZE')) {
	define('PAGE_SIZE', 20);
}

// check if legal
if(in_array($ps, array('add', 'edit', 'verify', 'interview'))) {
	define('IN_Read8_ACP_AUT', true);
	require ROOT.'admin/'.$pf.'/'.$pfile.'.php';
}
