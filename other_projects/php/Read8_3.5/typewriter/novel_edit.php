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
if(!defined('IN_Read8') || !defined('IN_Read8_TPWT') || !isset($PHP_SELF)) {
	exit('Access Denied (novel_edit.php)');
}



if(!$typepriv['overstep']) {
	if($book['state'] > 4)
		back('此书正在审核');

	unset($_CATEGORY['state'][4]);
	unset($_CATEGORY['state'][5]);
}



if($update == 1) {
	$columns = array(
		'title',
		'author',
		'typewriter',
		'subject',
		'content',
		'style',
		'role_lead',
		'role_support',
		'description',
		'tag',
		'intro',
		'state',
	);

	$column = array();
	foreach($columns as $v)
		if(!empty($$v))
			$column[$v] = $$v;

	if($book['author_id'])
		unset($column['author']);

	if(!$typepriv['overstep']) {
		unset($column['typewriter']);
	}else {
		if($column['typewriter'] != $book['typewriter']) {
			if($typewriter_row = $db->selectOne(array(
				'field' => 'uid',
				'from'  => __TAB_USER__,
				'where' => array('username' => $typewriter),
			))) {
				$column['typewriter_id'] = $typewriter_row['uid'];
			}else {
				unset($column['typewriter']);
			}
		}
	}
	$db->update($column, array(
		'id' => $bid,
	), __TAB_NOVEL__);
	jumpage('?bid='.$bid.'&action=novel_edit', '编辑成功');
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
	jumpage('?bid='.$bid.'&action=novel_edit', '修改封面成功');
}else {
	_header(array('作品编辑'));

	$tpl = new AOKOTpl('typewriter_'.$action);
	$tpl->assign('typepriv', $typepriv);

	$attributes = array('subject', 'content');
	foreach($attributes as $attribute) {
		$tmp = array();
		foreach($_CATEGORY['NOVEL'][$attribute] as $v) {
			$v['selected'] = $book[$attribute] == $v['id'] ? ' selected="selected"' : '';
			$tmp[] = $v;
		}

		$tpl->assign($attribute.'s', $tmp);
	}

	$attributes = array('style', 'license', 'state');
	foreach($attributes as $attribute) {
		$tmp = array();
		foreach($_CATEGORY['SYSTEM'][$attribute] as $v) {
			$v['selected'] = $book[$attribute] == $v['id'] ? ' selected="selected"' : '';
			$tmp[] = $v;
		}

		$tpl->assign($attribute.'s', $tmp);
	}


	$tpl->assign(html_show($book, false));
	$tpl->assign('HTMLURL', novel_url($book, 0));

	$tpl->output();

	_footer();
}
