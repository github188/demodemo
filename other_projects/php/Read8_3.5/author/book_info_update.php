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
	更新 - 作家中心
*/

define('IN_AUCP', true);
define('PHP_SELF_EXT', '_info_update');
define('PHP_SELF', basename(__FILE__, PHP_SELF_EXT.'.php'));


$update = $bid = $title = $author = $subject = $state = $typewriter = $typist = $content = $style = $role_lead = $role_support = $description = $tag = $intro = '';

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


if($update == 1) {
	$columns = array(
		'title',
		'state',
		'typewriter',
		'typist',
		'subject',
		'content',
		'style',
		'role_lead',
		'role_support',
		'description',
		'tag',
		'intro',
	);

	$column = array();
	foreach($columns as $v)
		if(!empty($$v))
			$column[$v] = $$v;

	if($typewriter) {
		if($typewriter_row = $db->selectOne(array(
			'field' => 'uid',
			'from'  => __TAB_USER__,
			'where' => array('username' => $typewriter),
		))) {
			$column['typewriter_id'] = $typewriter_row['uid'];
		}else {
			unset($column['typewriter']);
		}
	}else {
		$column['typewriter']    = '';
		$column['typewriter_id'] = 0;
	}

	$db->update($column, array(
		'author_id' => $_AUTHOR['id'],
		'id'        => $bid,
	), __TAB_NOVEL__);
}elseif($update == 2) {
	if(empty($_FILES) || empty($_FILES['cover']) || empty($_FILES['cover']['size']))
		back('请选择一个文件');

	if($_FILES['cover']['error'])
		back('上传失败');

	if('jpg' != rfilename($_FILES['cover']['name'], true))
		back('允许的格式为 jpg');

	if($_FILES['cover']['size'] > $_SYSTEM['AUTHOR']['size'])
		back('允许的大小为 '.sizecount($_SYSTEM['AUTHOR']['size']));

	rmove_uploaded_file($_FILES['cover']['tmp_name'], ROOT.novel_url($book, 0).'/bookface.jpg');
}


jumpage('book_info.php?bid='.$bid, '编辑成功');

