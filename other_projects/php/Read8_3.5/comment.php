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
//书评程序
//ok for 3.3

$bid = $cid = 0;
$title = $comment = $secode = '';

define('IN_Read8', true);
require_once str_replace(array('\\', '/'.basename(__FILE__)), array('/', ''), __FILE__).'/include/init.php';

include _config('comment');

if(!$_SYSTEM['COMMENT']['open'])
	back('评论功能尚未开启，请联系管理员');

if(!$Global['F_uid'] && !$_SYSTEM['COMMENT']['guest'])
	back('请先登录');

if(!_secode_check(0, $secode))
	back('验证码错误');

$length = rmb_strlen($comment);
if($length < $_SYSTEM['COMMENT']['minlength'])
	back('评论太短，最少 '.$_SYSTEM['COMMENT']['minlength'].' 字');

if($length > $_SYSTEM['COMMENT']['maxlength'])
	back('评论太长，最多 '.$_SYSTEM['COMMENT']['maxlength'].' 字');



$db->single(__TAB_NOVEL_COMMENT__);


if(!$bid || !ris_int($bid))
	back('无该书信息');

if(!($book = novel_exists($bid)))
	back('无该书信息');


if($cid) {
	if(!$_SYSTEM['COMMENT']['chapter'])
		back('当前不允许对章节发表评论');

	if(!ris_int($cid))
		back('无该章节信息');

	if(!($chapter = novel_chapter_exists($bid, $cid)))
		back('无该章节信息');
}

_log('comment', array($Global['F_username'], '发表', base64_encode($title), base64_encode($comment)));
_justnow('NCT', 'NEW', $bid, caddslashes($book['title']));

$db->insert(array(
	'book_id'    => $bid,
	'chapter_id' => $cid,
	'uid'        => $Global['F_uid'],
	'username'   => $Global['F_username'],
	'title'      => $title,
	'content'    => $comment,
	'ip'         => $Global['F_longip'],
	'dateline'   => TIMESTAMP,
	'active'     => 1,
));

jumpage(novel_url($book, $cid ? $cid : false), '评论成功');
