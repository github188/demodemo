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
if(!defined('IN_UACP_NVCPMGR')) {
	exit('Access Denied (book_chapter_manage_delete.php)');
}

/*
	�½�ɾ�� - �½ڹ��� - ��������
*/

$abspath = ROOT.novel_url($bookinfo, 0);

foreach($cps as $v)
	novel_chapter_delete($bid, $v, $abspath);


novel_rebuild_index($bid);

header('Location: book_info.php?bid='.$bid);
exit;