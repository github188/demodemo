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
if(!defined('IN_Read8')) {
	exit('Access Denied (const.def.php)');
}


if(!defined('TIMESTAMP')) {
	define('TIMESTAMP', time());
}
if(!defined('DATE_FORMAT')) {
	define('DATE_FORMAT', 'D, d M Y H:i:s');
}




if(!defined('LF')) {
	define('LF', "\n");
}
if(!defined('CR')) {
	define('CR', "\r");
}
if(!defined('HT')) {
	define('HT', "\t");
}

if(!defined('SYSCHARSET')) {
	define('SYSCHARSET', 'utf-8');
}


if(!defined('LOG_PATH')) {
	define('LOG_PATH', 'data/logfiles/'.gmdate('Ym', TIMESTAMP));
}




if(!defined('RANK_FACE')) {
	define('RANK_FACE', 1);
}
if(!defined('RANK_STRONG')) {
	define('RANK_STRONG', 2);
}
if(!defined('RANK_EDITOR')) {
	define('RANK_EDITOR', 3);
}
if(!defined('RANK_VIP')) {
	define('RANK_VIP', 4);
}
if(!defined('RANK_SUBJECT')) {
	define('RANK_SUBJECT', 5);
}
