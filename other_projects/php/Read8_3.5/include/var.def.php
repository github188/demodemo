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
	exit('Access Denied (var.def.php)');
}


if(!isset($GLOBALS['_SYSTEM']['SYSTEM']['TIME_ZONE'])) {
	$GLOBALS['_SYSTEM']['SYSTEM']['TIME_ZONE'] = '+8';
}
if(!isset($GLOBALS['_SYSTEM']['SYSTEM']['TIME_DELAY'])) {
	$GLOBALS['_SYSTEM']['SYSTEM']['TIME_DELAY'] = 0;
}




if(!isset($GLOBALS['_SYSTEM']['SAFE']['chmod_dir'])
|| !ris_int($GLOBALS['_SYSTEM']['SAFE']['chmod_dir'])
|| $GLOBALS['_SYSTEM']['SAFE']['chmod_dir'] > 511
|| $GLOBALS['_SYSTEM']['SAFE']['chmod_dir'] <= 0) {
	$GLOBALS['_SYSTEM']['SAFE']['chmod_dir'] = 511;
}
if(!isset($GLOBALS['_SYSTEM']['SAFE']['chmod_file'])
|| !ris_int($GLOBALS['_SYSTEM']['SAFE']['chmod_file'])
|| $GLOBALS['_SYSTEM']['SAFE']['chmod_file'] > 511
|| $GLOBALS['_SYSTEM']['SAFE']['chmod_file'] <= 0) {
	$GLOBALS['_SYSTEM']['SAFE']['chmod_file'] = 511;
}


$_RANK['DESC']['TIME'] = array(
	'week'  => '周',
	'month' => '月',
	'total' => '总',
);

$_RANK['DESC']['TYPE'] = array(
	'click'   => '点击',
	'vote'    => '投票',
	'rate'    => '评分',
	'favo'    => '收藏',
	'commend' => '推荐',
	'digg'    => '顶',
);

$_RANK['DESC']['COMD'] = array(
	'1' => '封面',
	'2' => '强力',
	'3' => '编辑',
	'4' => 'VIP ',
	'5' => '分类',
);

$_RANK['TIMES'] = array('week', 'month', 'total');
$_RANK['TYPES'] = array('click', 'vote', 'rate', 'favo', 'commend', 'digg', 'bury');
$_RANK['COMDS'] = array(
	1 => 'face',
	2 => 'strong',
	3 => 'editor',
	4 => 'vip',
	5 => 'subject',
);

$_RANK['INDEX']['T'] = array(// 排行榜标题
	'1'  => '强力推荐榜',
	'2'  => '编辑推荐榜',
	'3'  => '总点击榜',
	'4'  => '总推荐榜',
	'5'  => '月点击榜',
	'6'  => '月推荐榜',
	'7'  => '周点击榜',
	'8'  => '周推荐榜',
	'9'  => 'VIP 推荐榜',
	'10' => '驻站作家作品榜',
	'11' => '新进作品榜',
	'12' => '精华评论榜',
	'13' => '创作活力榜',
	'14' => '短篇作品榜',
	'15' => '封面推荐榜',
	'16' => '读者收藏榜',
	'17' => '分类推荐榜',
);
$_RANK['INDEX']['T'] = array(// 排行榜标题
	'1'  => '周点击榜',
	'2'  => '月点击榜',
	'3'  => '总点击榜',
	'4'  => '周投票榜',
	'5'  => '月投票榜',
	'6'  => '总投票榜',
	'7'  => '周评分榜',
	'8'  => '月评分榜',
	'9'  => '总评分榜',
	'10' => '周收藏榜',
	'11' => '月收藏榜',
	'12' => '总收藏榜',
	'13' => '周推荐榜',
	'14' => '月推荐榜',
	'15' => '总推荐榜',
	'16' => '周顶榜',
	'17' => '月顶榜',
	'18' => '总顶榜',
	'19' => '周埋榜',
	'20' => '月埋榜',
	'21' => '总埋榜',

	'15' => '封面推荐榜',
	'1'  => '强力推荐榜',
	'2'  => '编辑推荐榜',
	'9'  => 'VIP 推荐榜',
	'17' => '分类推荐榜',

	'12' => '精华评论榜',
	'10' => '驻站作家作品榜',
	'11' => '新进作品榜',
);