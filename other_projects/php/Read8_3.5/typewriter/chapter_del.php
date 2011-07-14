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
if(!defined('IN_Read8') || !defined('IN_Read8_TPWT') || !isset($PHP_SELF)) {
	exit('Access Denied (chapter_del.php)');
}


if($cid) {
	novel_chapter_delete($bid, $cid);
}elseif(!empty($cps)) {
	foreach($cps as $cid) {
		novel_chapter_delete($bid, $cid);
	}
}


novel_rebuild_index($bid);

jumpage('?bid='.$bid.'&action=chapter_edit', '删除成功');
