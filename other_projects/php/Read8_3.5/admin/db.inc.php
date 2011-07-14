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
	exit('Access Denied (db.inc.php)');
}



list($pf, $ps) = explode('_', $pfile);



$sqlrun = is_file(SITE_SAFE_SQL);
$sqlsff = substr(SITE_SAFE_SQL, strlen(ROOT));


// check if legal
if(in_array($ps, array('export', 'import', 'update', 'optimize', 'repair'))) {
	define('IN_Read8_ACP_DB', true);
	require ROOT.'admin/'.$pf.'/'.$pfile.'.php';
}
