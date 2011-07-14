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
	exit('Access Denied (chapter_add.php)');
}

require_once ROOT.'include/booked.func.php';
require_once ROOT.'include/editor.func.php';
require_once ROOT.'include/image.func.php';

if(!$typepriv['overstep']) {
	if($book['state'] > 4)
		back('此书正在审核');

	if($book['state'] == 2)
		back('此书暂停更新');
}
if($book['state'] == 3)
	back('此书已完结');




if($update) {
	$title   = trim($title);
	$article = rtrim($article);
	$dir = novel_url($book, 0);

	if($insertbefore) {//中间插入处理必须的其他处理
		//更新插入章节后的所有章节的静态页面
		novel_chapter_rename($insertbefore, ROOT.$dir, true);

		//修改数据库
		$sql="UPDATE ".__TAB_NOVEL_CHAPTER__." SET priority = priority + 1 WHERE priority >= '$insertbefore' AND `book_id` = '$bid'";
		$db->execUpBySql($sql);
		$sql="UPDATE ".__TAB_NOVEL_VOLUME__." SET chapter_priority = chapter_priority + 1 WHERE chapter_priority >= '$insertbefore' AND `book_id` = '$bid'";
		$db->execUpBySql($sql);

		$priority = $insertbefore;//新章节的顺序id
	}else {//非插入的章节提交
		$priority = 0;
		if($row = $db->selectOne(array(//最大章节id
			'field' => 'MAX(priority) AS priority',
			'from'  => __TAB_NOVEL_CHAPTER__,
			'where' => array('book_id' => $bid),
		))) {
			$priority = $row['priority'];
		}
		$priority++;//新章节的顺序id
	}

	//增加卷
	if($volumetitle) {
		$sql="INSERT INTO ".__TAB_NOVEL_VOLUME__." VALUES('$bid', NULL, '$volumetitle', '$priority')";
		$db->execInBySql($sql);
	}
	//得到卷id
	$volumid = 0;
	if($row = $db->selectOne(array(
		'field' => 'id AS volumid',
		'from'  => __TAB_NOVEL_VOLUME__,
		'where' => "WHERE `book_id` = '$bid' AND chapter_priority < ".($priority + 1),
		'order' => 'chapter_priority DESC',
	))) {
		$volumid = $row['volumid'];
	}
	//$tmpurl = ($charge && VipPriv() && sn('vip')) ? md5(time().rand(1,100)) : '';

	//提交章节mysql处理
	$abschapterid = $db->insert(array(//章节绝对id
		'book_id'     => $bid,
		'title'       => $title,
		'priority'    => $priority,
		'volume_id'   => $volumid,
		'description' => $description,
		'dateline'    => TIMESTAMP,
		'words'       => rmb_strlen($title, true),
		'hash'        => md5($title),
	), __TAB_NOVEL_CHAPTER__);

	if($_SYSTEM['UPLOAD']['open'] && $_SYSTEM['UPLOAD']['size'] && !empty($_FILES['newattach'])) {
		$article .= new_attach(upload_convert($_FILES['newattach']), $book, $abschapterid);
	}

	//章节网页生成
	$tpl = new AOKOTpl('../../html/template/content');
	$tpl->assign(array(
		'chapter_title' => stripslashes($title),
		'book_title'    => $book['title'],
		'C_authorname'  => $book['author'],
		'bid'           => $bid,
		'cid'           => $abschapterid,
		'C_lchapterid'  => $priority - 1,
		'C_nchapterid'  => $priority + 1,
		'C_tchapterid'  => $priority,
		'C_content'     => editor_safe_replace(UbbDecode(stripslashes($article))),
	));
	rfow(ROOT.novel_url($book, $priority), $tpl->output(1));

	//书籍更新mysql
	//$sqladd = $end ? ", `state` = 0" : '';
	//$words  = $cid ? 0 : $words;
	$db->update(array(
		'lastupdate'  => TIMESTAMP,
		'lastchapter' => $abschapterid,
	), array(
		'id'          => $bid,
	), __TAB_NOVEL__);
	//$sql="UPDATE ".__TAB_BOOK__." SET `updatetime` = '".TIMESTAMP."', `newchapterid` = '$abschapterid', `size` = `size` + ".$words.$sqladd." WHERE `id` = '$pbid'";
	//$db->execUpBySql($sql);

	novel_volume_tidy($bid);//章节表的卷重新生成

	novel_rebuild_index($bid);

	if($gotonext) {
		jumpage('?bid='.$bid.'&action=chapter_add', '添加章节成功');
	}else {
		jumpage('?bid='.$bid.'&action=chapter_edit', '添加章节成功');
	}
}else {
	_header(array('添加章节'));

	$tpl = new AOKOTpl('typewriter_'.$action);

	$tpl->assign(html_show($book, false));

	/*$rows = $db->select(array(
		'field' => 'c.priority, c.title, v.title AS volume',
		'from'  => $db->table(__TAB_NOVEL_CHAPTER__, 'c'),
		'join'  => array('table' => $db->table(__TAB_NOVEL_VOLUME__, 'v'), 'on' => 'c.volume_id = v.id AND c.book_id = v.book_id'),
		'where' => array('c.book_id' => $bid),
		'order' => 'c.priority ASC',
	));
	$i = -1;
	$volume = '';
	$volumes = array();
	foreach($rows as $v) {
		if($volume != $v['volume']) {
			$i++;
			$volume = $v['volume'];
			$volumes[$i] = array(
				'title'    => $volume,
				'chapters' => array(),
			);
		}
		$volumes[$i]['chapters'][] = array(
			'title'    => $v['title'],
			'priority' => $v['priority'],
		);
	}*/

	$tpl->assign(array(
		'bid'     => $bid,
		//'volumes' => $volumes,
		'editor'  => html_editor('article'),
		'upload'  => $_SYSTEM['UPLOAD']['open'] ? sizecount($_SYSTEM['UPLOAD']['size']) : 0,
		'formats' => implode(' ', $format),
		'chapters'  => $db->select(array(
			'field' => 'id, title, priority, dateline, words, locked, vip',
			'from'  => __TAB_NOVEL_CHAPTER__,
			'where' => array(
				'book_id' => $bid,
			),
			'order' => 'priority ASC',
		)),
	));

	$tpl->output();

	_footer();
}
