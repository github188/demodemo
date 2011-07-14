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
if(!defined('IN_Read8') || !defined('IN_Read8_ACP') || !defined('IN_Read8_ACP_AUT') || !isset($PHP_SELF)) {
	exit('Access Denied (author_interview.php)');
}

require_once ROOT.'include/author.func.php';

$db->single(__TAB_AUT_INTERVIEW__);

if(empty($id)) {
	$id = '';
}
if(empty($title)) {
	$title = '';
}
if(empty($content)) {
	$content = '';
}
if(empty($pseudonym)) {
	$pseudonym = '';
}

if($action == 'delete') {
	if(!empty($ivs))
		if(is_array($ivs))
			foreach($ivs as $k => $v)
				if(!$v || !ris_int($v))
					unset($ivs[$k]);

	if(!empty($ivs))
		$db->update(array(
			'active' => 0,
		), array(
			'id'     => $ivs,
		));

	jumpage("?ptype=$ptype&pfile=$pfile", '删除成功');
}elseif($action == 'update') {
	if(!$title)
		back('标题不能为空');

	if($id) {
		if(!author_interview($id))
			back('未找到该访谈记录');

		$db->update(array(
			'title'   => $title,
			'content' => $content,
		), array(
			'id'      => $id,
		));
	}else {
		if(!$pseudonym)
			back('笔名不能为空');

		if(!$author = author_by_pseudonym($pseudonym))
			back('未找到该作家');

		$db->insert(array(
			'author_id' => $author['id'],
			'title'     => $title,
			'content'   => $content,
			'dateline'  => TIMESTAMP,
		));
	}

	_clear_cache('cache');
	jumpage("?ptype=$ptype&pfile=$pfile", '修改成功');
}

if($id) {
	$title = $content = $pseudonym = '';
	if($interview = author_interview($id)) {
		extract(html_show(html_show_decode($interview), false));
	}else {
		$id = '';
	}
}



if(!defined('PAGE_SIZE')) {
	define('PAGE_SIZE', 20);
}

$tpl = new AOKOTpl('author_interview');
$tpl->assign(array(
	'id'         => $id,
	'title'      => $title,
	'content'    => $content,
	'pseudonym'  => $pseudonym,
	'interviews' => $db->select(array(
						'field' => 'ai.id, ai.author_id, ai.title, ai.dateline, a.pseudonym',
						'from'  => $db->table(__TAB_AUT_INTERVIEW__, 'ai'),
						'join'  => array('table' => $db->table(__TAB_MY_AUT__, 'a'), 'on' => 'ai.author_id = a.id'),
						'where' => array(
							'ai.active' => 1,
							'a.active'  => 1,
						),
						'order' => 'ai.dateline DESC',
						'limit' => PAGE_SIZE,
						'page'  => $page,
						'filter'=> 'html_show',
						'counter'=>1,
					)),
	'pagelink'   => page($db->counter, PAGE_SIZE),
));
$tpl->output();
