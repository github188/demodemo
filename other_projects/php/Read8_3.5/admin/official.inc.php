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
	exit('Access Denied (official.inc.php)');
}

//官方互动的交互和文档部分



list($pf, $ps) = explode('_', $pfile);

if($pf == 'official') {
	$cs = array('ask', 'bug', 'site', 'manual', 'info', 'join');
}elseif($pf == 'download') {
	$cs = array('rule', 'style', 'module');
}else {
	back('admin_file_error');
}

if(!in_array($ps, $cs))
	back('admin_file_error');

include _config('official');

echo '<iframe height="800px" width="100%" frameborder="0" src="http://my.read8.net/client.php?info='.rawurlencode(base64_encode($_SYSTEM['SYSTEM']['SITE_ADDR'].'|||'.$_SYSTEM['SYSTEM']['SITE_NAME'].'|||'.SYS_VERSION.'|||'.SYS_BUILD.'|||'.$ps.'|||'.$_SYSTEM['OFFICIAL']['username'].'|||'.$_SYSTEM['OFFICIAL']['password'].'|||'.TIMESTAMP)).'"></iframe>';
