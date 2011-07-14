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
	新建 - 短篇文学
*/

define('IN_UCP', true);
define('PHP_SELF_EXT', '_create');
define('PHP_SELF', basename(__FILE__, PHP_SELF_EXT.'.php'));


$title = $author = $subject = $content = $description = $tag = $body = '';

require_once '../include/init.php';
require_once ROOT.'my/category.inc.php';



$title = trim($title);
$author = trim($author);



if(!$title)
	back('请填写作品名称');

if(!$author)
	back('请填写作者');


include _config('story');

if(!$_SYSTEM['STORY']['open'])
	back('此功能未开放');


$bid = $db->insert(array(
	'title'       => $title,
	'author'      => $author,
	'typewriter'  => $Global['F_uid'],
	'subject'     => $subject,
	'content'     => $content,
	'description' => $description,
	'tag'         => $tag,
	'body'        => $body,
	'dateline'    => TIMESTAMP,
	'lastupdate'  => TIMESTAMP,
	'state'       => $_SYSTEM['STORY']['verify'] ? 6 : 3,
), __TAB_STORY__);
$db->insert(array(
	'book_id' => $bid,
), __TAB_STORY_EXT__);


header('Location: story.php');
exit;
