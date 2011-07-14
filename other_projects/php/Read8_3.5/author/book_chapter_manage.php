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
/*
	章节管理 - 作家中心
*/

define('IN_AUCP', true);
define('PHP_SELF_EXT', '_chapter_manage');
define('PHP_SELF', basename(__FILE__, PHP_SELF_EXT.'.php'));


$cps = array();
$bid = $action = '';

require_once '../include/init.php';
require_once ROOT.'author/category.inc.php';


$book = get_novel_by_id($bid);
if(!$book) {
	back('无此书信息');
}
if($book['author_id'] != $_AUTHOR['id']) {
	back('您无权管理此书');
}
if($book['state'] > 4) {
	back('此书正在审核');
}


if(!empty($cps) && is_array($cps))
	foreach($cps as $k => $v)
		if(!ris_int($v))
			unset($cps[$k]);


if(empty($cps)) {
	back('请选择要管理的章节');
}

$actions = array(
	'delete' => '删除',
	//'lock'   => '锁定',
	//'unlock' => '解除锁定',
	//'vip'    => '申请 VIP',
	'public' => '解禁 VIP',
);

if(!isset($actions[$action])) {
	back('请选择管理操作');
}



if($action == 'delete') {
	foreach($cps as $cid) {
		novel_chapter_delete($bid, $cid);
	}
	jumpage('book_manage.php?bid='.$bid, '删除成功');
}elseif($action == 'vip') {
	/*foreach($cps as $cid) {
		$db->i
	}*/
}