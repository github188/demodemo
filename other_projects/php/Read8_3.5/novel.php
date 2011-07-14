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
//书籍信息程序

$bid = '';

define('IN_Read8', true);
require_once str_replace(array('\\', '/'.basename(__FILE__)), array('/', ''), __FILE__).'/include/init.php';


if(!$bid || !ris_int($bid))
	back('无该书信息');

if(!($book = novel_exists($bid)))
	back('无该书信息');

if($book['state'] > 3)
	back('本书已被锁定');


// get subject name by id
$book = html_show(convert_novel_classes($book));

//设定搜索关键词
$seokeyword[] = $book['title'];
$seokeyword[] = $book['author'];
_header($seokeyword);//输出头部


$tpl = new AOKOTpl('novel_info');

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
$tpl->assign(novel_ext($book['bid']));

// latest chapter
$tpl->assign($db->selectOne(array(
	'field' => 'nc.priority, nc.title AS chapter_title',
	'from'  => $db->table(__TAB_NOVEL__, 'n'),
	'join'  => array('table' => $db->table(__TAB_NOVEL_CHAPTER__, 'nc'), 'on' => 'n.id = nc.book_id AND n.lastchapter = nc.id'),
	'where' => array('n.id' => $book['bid']),
)));

// system configuration
include _config('read');
include _config('comment');

$dugg = _cookie_decode('novel_digg'.$bid);
$bury = _cookie_decode('novel_bury'.$bid);


$tpl->assign(array(
	'AuthorURL' => author_url($book['author_id']),
	'HTMLURL'   => novel_url($book, 0),

	'fulltext'         => $_SYSTEM['READ']['fulltext']         ? ($Global['F_uid'] ? 1 : ($_SYSTEM['READ']['fulltext_guest']         ? 1 : 0)) : 0,
	'download_package' => $_SYSTEM['READ']['download_package'] ? ($Global['F_uid'] ? 1 : ($_SYSTEM['READ']['download_package_guest'] ? 1 : 0)) : 0,
	'download_text'    => $_SYSTEM['READ']['download_text']    ? ($Global['F_uid'] ? 1 : ($_SYSTEM['READ']['download_text_guest']    ? 1 : 0)) : 0,

	'comment_open'     => $_SYSTEM['COMMENT']['open']          ? ($Global['F_uid'] ? 1 : ($_SYSTEM['COMMENT']['guest']               ? 1 : 0)) : 0,
	'comment_chapter'  => $_SYSTEM['COMMENT']['chapter']       ? ($Global['F_uid'] ? 1 : ($_SYSTEM['COMMENT']['guest']               ? 1 : 0)) : 0,
	'comment_min'      => $_SYSTEM['COMMENT']['minlength'],
	'comment_max'      => $_SYSTEM['COMMENT']['maxlength'],

	'comment_secode'   => _secode(0),
	'comments'         => _novel_comment($bid),

	'dugg' => $dugg && TIMESTAMP - $dugg <= 86400,
	'bury' => $bury && TIMESTAMP - $bury <= 86400,

	'tags' => parse_tags($book['tag']),

	'jubao' => rawurlencode('举报《'.$book['title'].'》'),
));



$tpl->output();

_footer();

