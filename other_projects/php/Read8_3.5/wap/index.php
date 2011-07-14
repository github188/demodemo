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

$psz = false;
$bid = $cid = $page = 0;
require '../include/init.php';
require_once ROOT.'include/wap.func.php';

if(false !== $psz && ris_int($psz) && ($psz == 0 || ($psz >= 100 || $psz <= 10000))) {
	cookie('psz', strval($psz), 31536000);
}

if(!$bid) {
	wap_index();
}else {
	if(!ris_int($bid))
		wap_exit('非法的书籍编号');

	if(!($book = novel_exists($bid)))
		wap_exit('未找到该书信息');

	if(!$cid) {
		wap_novel($book, $page);
	}else {
		wap_chapter($book, $cid, $page);
	}
}
