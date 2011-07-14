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
	exit('Access Denied (my.func.php)');
}

/*
	用户中心函数库
*/


function is_author($bid, $aid){
	//
}


function my_favorite_category(){
	global $db;

	return $db->select(array(
		'field' => 'gid, category',
		'from'  => __TAB_MY_FAV_CAT__,
		'where' => array('uid' => $GLOBALS['Global']['F_uid']),
	));
}

function my_favorite_check($gid){
	global $db;

	return $db->selectOne(array(
		'field' => 'gid',
		'from'  => __TAB_MY_FAV_CAT__,
		'where' => array(
			'uid' => $GLOBALS['Global']['F_uid'],
			'gid' => $gid,
		),
	));
}

function my_favorite($category = false){
	global $db;

	$where['f.uid'] = $GLOBALS['Global']['F_uid'];
	if($category)
		$where['f.gid'] = $category;

	return $db->select(array(
		'field' => 'f.fid,
					g.category,
					f.bid,
					b.title AS book_title,
					f.cid,
					c.priority,
					c.title AS chapter_title,
					b.lastupdate,
					f.dateline,
					b.initial AS bp_i,
					b.dateline AS bp_d,
					b.words AS bp_w,
					b.subject AS bp_t,
					b.path',
		'from'  => $db->table(__TAB_MY_FAV__, 'f'),
		'join'  => array(
			array('table' => $db->table(__TAB_MY_FAV_CAT__,    'g'), 'on' => 'g.uid = f.uid AND g.gid = f.gid'),
			array('table' => $db->table(__TAB_NOVEL__,         'b'), 'on' => 'b.id = f.bid'),
			array('table' => $db->table(__TAB_NOVEL_CHAPTER__, 'c'), 'on' => 'c.book_id = f.bid AND c.id = f.cid'),
		),
		'where' => $where,
		'filter'=> 'my_favorite_filter',
	));
}

function my_favorite_filter($f){
	$f['BookInfoURL'] = novel_url($f);
	$f['ChapterURL']  = novel_url($f, $f['priority']);
	$f['update']      = $f['lastupdate'] > $f['dateline'];
	return html_show($f, false);
}

function my_typewriter(){
	global $db;

	return $db->selectOne(array(
		'field' => 'id',
		'from'  => __TAB_NOVEL__,
		'where' => array('typewriter_id' => $GLOBALS['Global']['F_uid']),
	));
}

function my_author($active = true){
	global $db;

	if($active) {
		return $db->selectOne(array(
			'field' => 'id, pseudonym, pinyin, avatar, column_name, intro, announcement, vip, dateline',
			'from'  => __TAB_MY_AUT__,
			'where' => array(
				'uid'    => $GLOBALS['Global']['F_uid'],
				'active' => 1,
			),
		));
	}else {
		return $db->selectOne(array(
			'field' => 'id, pseudonym, pinyin, avatar, column_name, intro, announcement, vip, dateline, active',
			'from'  => __TAB_MY_AUT__,
			'where' => array(
				'uid'    => $GLOBALS['Global']['F_uid'],
			),
		));
	}
}

function my_novel($bid = false){
	global $_AUTHOR;

	if(false === $bid) {
		return get_novel_by_author_id($_AUTHOR['id']);
	}else {
		//return ;
	}
}


function my_author_commend($aid){
	global $db;

	return $db->select(array(
		'field' => 'n.title,
					np.title   AS chapter_title,
					np.priority,
					n.author,
					n.author_id,
					n.lastupdate,
					n.content,
					n.state,
					n.id       AS bid,
					n.initial  AS bp_i,
					n.dateline AS bp_d,
					n.words    AS bp_w,
					n.subject  AS bp_t,
					n.path',
		'from'  => $db->table(__TAB_MY_AUT_CMD__, 'nc'),
		'join'  => array(
			array('table' => $db->table(__TAB_NOVEL__,         'n'), 'on' => 'n.id = nc.book_id'),
			array('table' => $db->table(__TAB_NOVEL_CHAPTER__, 'np'), 'on' => 'n.id = np.book_id AND n.lastchapter = np.id'),
		),
		'where' => array('nc.author_id' => $aid),
		'order' => 'sticky DESC, nc.dateline DESC',
		'filter'=> 'my_author_commend_filter',
	));
}

function my_author_commend_filter($book){
	$book['InfoURL']    = novel_url($book);
	$book['AuthorURL']  = 'author.php?aid='.$book['author_id'];
	$book['subject']    = $book['bp_t'];

	return convert_novel_classes(html_show($book, false));
}