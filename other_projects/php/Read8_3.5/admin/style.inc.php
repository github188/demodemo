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
	exit('Access Denied (style.inc.php)');
}



list($pf, $ps) = explode('_', $pfile);

// check if legal
if(in_array($ps, array('choose', 'cache'))) {
	define('IN_Read8_ACP_STYLE', true);
	require ROOT.'admin/'.$pf.'/'.$pfile.'.php';
}
