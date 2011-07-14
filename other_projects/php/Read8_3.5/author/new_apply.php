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
	新书申请 - 作家中心
*/

define('IN_AUCP', true);
define('PHP_SELF_EXT', '_apply');
define('PHP_SELF', basename(__FILE__, PHP_SELF_EXT.'.php'));


$title = $subject = $content = $style = $role_lead = $role_support = $license = $description = $tag = $intro = $sample = '';

require_once '../include/init.php';
require_once ROOT.'author/category.inc.php';


if(!$title) {
	back('请填写作品名称');
}

$db->insert(array(
	'title'         => $title,
	'author'        => $_AUTHOR['pseudonym'],
	'author_id'     => $_AUTHOR['id'],
	'subject'       => $subject,
	'content'       => $content,
	'style'         => $style,
	'role_lead'     => $role_lead,
	'role_support'  => $role_support,
	'license'       => $license,
	'description'   => $description,
	'tag'           => $tag,
	'intro'         => $intro,
	'sample'        => $sample,
	'dateline'      => TIMESTAMP,
), __TAB_NOVEL_APPLY__);


jumpage('verify.php', '申请成功');
header('Location: verify.php');
exit;