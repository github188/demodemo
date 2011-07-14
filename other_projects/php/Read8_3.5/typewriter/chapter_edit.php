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
	exit('Access Denied (chapter_edit.php)');
}

require_once ROOT.'include/booked.func.php';
require_once ROOT.'include/editor.func.php';
require_once ROOT.'include/image.func.php';


if($update) {
	if($cid) {
		$title   = trim($title);
		$article = rtrim($article);
		$dir = novel_url($book, 0);

		if($_SYSTEM['UPLOAD']['open'] && $_SYSTEM['UPLOAD']['size'] && !empty($_FILES['newattach'])) {
			$article .= new_attach(upload_convert($_FILES['newattach']), $book, $cid);
		}

		$tpl = new AOKOTpl('../../html/template/content');
		$tpl->assign(array(
			'chapter_title' => stripslashes($title),
			'book_title'    => $book['title'],
			'C_authorname'  => $book['author'],
			'bid'           => $bid,
			'cid'           => $cid,
			'C_lchapterid'  => $priority - 1,
			'C_nchapterid'  => $priority + 1,
			'C_tchapterid'  => $priority,
			'C_content'     => editor_safe_replace(UbbDecode(stripslashes($article))),
		));
		rfow(ROOT.novel_url($book, $priority), $tpl->output(1));

		//书籍更新mysql
		//$sqladd = $end ? ", `state` = 0" : '';
		//$words  = $cid ? 0 : $words;
		/*$db->update(array(
			'lastupdate'  => TIMESTAMP,
			'lastchapter' => $abschapterid,
		), array(
			'id'          => $bid,
		), __TAB_NOVEL__);*/
		//$sql="UPDATE ".__TAB_BOOK__." SET `updatetime` = '".TIMESTAMP."', `newchapterid` = '$abschapterid', `size` = `size` + ".$words.$sqladd." WHERE `id` = '$pbid'";
		//$db->execUpBySql($sql);

		novel_volume_tidy($bid);//章节表的卷重新生成

		novel_rebuild_index($bid);

		jumpage('?bid='.$bid.'&action=chapter_edit', '编辑成功');
	}else {
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
	}
}else {
	if($cid) {
		_header(array('编辑章节'));

		$tpl = new AOKOTpl('typewriter_'.$action);
		$tpl->assign('list', 0);
		$tpl->assign('book', html_show($book, false));

		$tpl->assign(html_show($chapter, false));
		$tpl->assign(array(
			'bid'     => $bid,
			'cid'     => $cid,
			'editor'  => html_editor('article', novel_content(ROOT.novel_url($book, $priority))),
			'upload'  => $_SYSTEM['UPLOAD']['open'] ? sizecount($_SYSTEM['UPLOAD']['size']) : 0,
			'formats' => implode(' ', $format),
		));

		$tpl->output();

		_footer();
	}else {
		_header(array('章节管理'));

		$_assign['typepriv'] = $typepriv;

		$tpl = new AOKOTpl('typewriter_'.$action);
		$tpl->assign('list', 1);
		$tpl->assign(html_show($book, false));

		$tpl->assign(array(
			'chapters'  => html_show($db->select(array(
				'field' => 'book_id, id, title, priority, dateline, words, locked, vip',
				'from'  => __TAB_NOVEL_CHAPTER__,
				'where' => array(
					'book_id' => $bid,
				),
				'order' => 'priority DESC',
				'limit' => PAGE_SIZE,
				'page'  => $page,
				'counter'=>1,
			))),
			'pagelink' => page($db->counter, PAGE_SIZE),
		));

		$tpl->output();

		unset($_assign['typepriv']);

		_footer();
	}
}
