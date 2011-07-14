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
	更新 - 短篇文学
*/

define('IN_UCP', true);
define('PHP_SELF_EXT', '_update');
define('PHP_SELF', basename(__FILE__, PHP_SELF_EXT.'.php'));


$bid = '';

require_once '../include/init.php';
require_once ROOT.'my/category.inc.php';


$book = story_exists($bid);
if(!$book) {
	back('无此书信息');
}

$title = trim($title);
$author = trim($author);



if(!$title)
	back('请填写作品名称');

if(!$author)
	back('请填写作者');


include _config('story');

if(!$_SYSTEM['STORY']['open'])
	back('此功能未开放');


$db->update(array(
	'title'       => $title,
	'author'      => $author,
	'subject'     => $subject,
	'content'     => $content,
	'description' => $description,
	'tag'         => $tag,
	'body'        => $body,
	'lastupdate'  => TIMESTAMP,
), array(
	'id'          => $bid,
), __TAB_STORY__);


header('Location: story.php');
exit;
