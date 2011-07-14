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
	exit('Access Denied (collect.inc.php)');
}



list($pf, $ps) = explode('_', $pfile, 2);

// check if legal
if(in_array($ps, array('single', 'multi', 'single_rule', 'multi_rule', 'blacklist'))) {
	define('IN_Read8_ACP_COLLECT', true);
	require ROOT.'admin/'.$pf.'/'.$pfile.'.php';
}
