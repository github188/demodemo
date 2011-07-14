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

$bid = '';

define('IN_Read8', true);
require_once str_replace(array('\\', '/'.basename(__FILE__)), array('/', ''), __FILE__).'/include/init.php';


if(!$bid || !ris_int($bid))
	back('无该书信息');

if(!($book = story_exists($bid)))
	back('无该书信息');

if($book['state'] != 3)
	back('无该书信息');


// get subject name by id
$book = html_show(convert_story_classes($book));


//设定搜索关键词
$seokeyword[] = $book['title'];
$seokeyword[] = $book['author'];
_header($seokeyword);//输出头部


$tpl = new AOKOTpl('story_info');

// rank
foreach($_RANK['TIMES'] as $i) {
	foreach($_RANK['TYPES'] as $j) {
		$tpl->assign('novel_'.$i.'_'.$j, _book_rank($i, $j, 0, false));
		$tpl->assign('story_'.$i.'_'.$j, _book_rank($i, $j, 0, true));
	}
}

// book info
$tpl->assign($book);
// book ext
$tpl->assign(story_ext($book['bid']));


// system configuration
include _config('read');
include _config('comment');

$dugg = _cookie_decode('story_digg'.$bid);
$bury = _cookie_decode('story_bury'.$bid);


$tpl->assign(array(
	'comment_open'     => $_SYSTEM['COMMENT']['open']          ? ($Global['F_uid'] ? 1 : ($_SYSTEM['COMMENT']['guest']               ? 1 : 0)) : 0,
	'comment_chapter'  => $_SYSTEM['COMMENT']['chapter']       ? ($Global['F_uid'] ? 1 : ($_SYSTEM['COMMENT']['guest']               ? 1 : 0)) : 0,
	'comment_min'      => $_SYSTEM['COMMENT']['minlength'],
	'comment_max'      => $_SYSTEM['COMMENT']['maxlength'],

	'comment_secode'   => _secode(0),
	'comments'         => _story_comment($bid),

	'dugg' => $dugg && TIMESTAMP - $dugg <= 86400,
	'bury' => $bury && TIMESTAMP - $bury <= 86400,

	'tags' => parse_tags($book['tag']),

	'jubao' => rawurlencode('举报《'.$book['title'].'》'),
));


$tpl->output();


_footer();