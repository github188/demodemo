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
//后台作品列表
//作品删除、状态修改


if(!defined('IN_Read8') || !defined('IN_Read8_ACP') || !isset($PHP_SELF)) {
	exit('Access Denied (setworks.php)');
}

require_once ROOT.'include/booked.func.php';

$db->single(__TAB_NOVEL__);

//作品状态修改
if($action=="update" && $bid && $newstate) {
	//更新状态
	//$sql="UPDATE ".__TAB_BOOK__." SET state='$newstate' WHERE id='$bid'";
	//$db->query($sql);
	$db->update(array(
		'state' => $newstate,
	), array(
		'id'    => $bid,
	));

	//更新最后一章的下一页按钮
	//$sql="SELECT b.type_id, b.title, b.author, c.title as chapter_title, c.orderid FROM  ".__TAB_BOOK__." b LEFT JOIN ".__TAB_CHAPTER__." c ON b.newchapterid=c.id WHERE b.id='$bid'";
	//$row = $db->selectBySql($sql, 'array_shift');
	extract($db->selectOne(array(
		'field' => 'b.subject,
					b.title   AS book_title,
					b.author  AS book_author,
					c.orderid AS newchapterid',
		'from'  => $db->table(__TAB_NOVEL__, 'b'),
		'join'  => array('table' => $db->table(__TAB_CHAPTER__, 'c'), 'on' => 'b.newchapterid = c.id'),
		'where' => array('b.id' => $bid),
	)));
	$chapterfile    = ROOT."html/$type_id/$bid/$newchapterid.html";
	$chaptercontent = novel_content($chapterfile);
	$dir            = "html/$type_id/$pbid/";
	//章节网页生成
	$tpl = new AOKOTpl('../../html/template/content');
	$tpl->assign(array(
		'C_chaptertitle' => stripslashes($chaptertitle),
		'C_booktitle'    => $book_title,
		'C_authorname'   => $book_author,
		'C_bookid'       => $bid,
		'C_lchapterid'   => $newchapterid-1,
		'C_nchapterid'   => $newstate==0 ? 0 : $newchapterid+1,
		'C_tchapterid'   => $newchapterid,
		'C_content'      => $chaptercontent,
	));
	rfow($chapterfile, $tpl->output(1));

	//日志记录
	$bookstatarr=array('1'=>'连载中','2'=>'暂停中','0'=>'全书完');
	//$sql="SELECT title,state FROM ".__TAB_BOOK__." WHERE id='$bid'";
	//$row=$db->fetch_one($sql);
	$row = $db->selectOne(array(
		'field' => 'title, state',
		'where' => array('id' => $bid),
	));
	$logcon="书籍状态修改<".$row['title']."由".$bookstatarr[$row['state']]."改为".$bookstatarr[$newstate].">";
	jumpage("?ptype=$ptype&pfile=$pfile",'admin_manage_book_success_change_state');
}

//书籍清除
if($action == 'clear' && is_array($clrbookarr)) {
	if(!acppriv(9)) back('admin_no_privileges_manage_book_del');
	//检查是否超过每日删除上限
	/*$sql="SELECT * FROM ".__TAB_ACTLOG__." WHERE uid='$Global[F_uid]' AND actlog_act like '%删除书籍%' AND dateline>".$Global['F_tdtime'];
	$result=$db->query($sql);
	$delednum=$db->num_rows($result);
	$delednum+=count($delbookarr);
	if($delednum>$Global['delbookfreq'] && !IsGroup($Global['Rootgroupid'])) back("书籍删除超过每日上限".$Global['delbookfreq']);*/

	foreach($clrbookarr as $bid){
		novel_clear($bid);
	}
	jumpage("?ptype=$ptype&pfile=$pfile",'admin_manage_book_success_clear');
}

//书籍删除
if($action=="delete" && is_array($delbookarr)) {
	if(!acppriv(9)) back('admin_no_privileges_manage_book_del');
	//检查是否超过每日删除上限
	//$sql="SELECT count(*) as maxnum FROM ".__TAB_ACTLOG__." WHERE uid='$Global[F_uid]' AND info like '%删除书籍%' AND dateline>".$Global['F_tdtime'];
	//$row=$db->selectBySql($sql, 'array_shift');
	//$delednum = $row['maxnum'] + count($delbookarr);
	//if($delednum>$Global['delbookfreq'] && !IsGroup($Global['Rootgroupid'])) back("书籍删除超过每日上限".$Global['delbookfreq']);

	//日志记录
	if(count($delbookarr)>1) {
		$logcon="批量删除书籍<共".count($delbookarr)."本>";
	}else {
		//$sql="SELECT title,type_id FROM ".__TAB_BOOK__." WHERE id='".$delbookarr[0]."'";
		//$row=$db->fetch_one($sql);
		$row = $db->selectOne(array(
			'field' => 'title',
			'where' => array('id' => $delbookarr[0]),
		));
		$logcon="删除书籍<".$row['title'].">";
	}

	//删除静态文件 删除附件
	foreach($delbookarr as $bid){
		//$sql="SELECT type_id FROM ".__TAB_BOOK__." WHERE id='$bid'";
		//$row=$db->fetch_one($sql);
		$row = $db->selectOne(array(
			'field' => 'type_id',
			'where' => array('id' => $bid),
		));
		rrmdir(ROOT.novel_url(novel_exists($bid), 0));

		//删除附件
		//$sql="SELECT * FROM ". __TAB_ATTACH__." WHERE book_id='$bid'";
		//$result=$db->query($sql);
		$attachs = $db->select(array(
			'field' => 'file',
			'from'  => __TAB_NOVEL_ATTACH__,
			'where' => array('book_id' => $bid),
		));
		foreach($attachs as $row) {
			unlink(ROOT."$Global[attach_dir]/$row[file]");
		}
	}
	//删除数据库
	$sql = array(
		__TAB_NOVEL_EXT__,
		__TAB_NOVEL_CHAPTER__,
		__TAB_NOVEL_VOLUME__,
		__TAB_NOVEL_COMMENT__,
		__TAB_NOVEL_COMMEND__,
		__TAB_NOVEL_ATTACH__,
	);
	foreach($sql as $val) {
		//$db->query("DELETE FROM $val WHERE book_id IN ('".implode("','",$delbookarr)."')");
		$db->delete(array(
			'book_id' => $delbookarr,
		), $val);
	}
	//$db->query("DELETE FROM ".__TAB_BOOK__." WHERE id IN ('".implode("','",$delbookarr)."')");
	$db->delete(array(
		'id' => $delbookarr,
	));

	//UpdateCache('bookmain');
	//日志记录
	jumpage("?ptype=$ptype&pfile=$pfile",'admin_manage_book_success_del');
}



//-------------显示作品------------

$sql = array(
	'order' => '`lastupdate` DESC',
	'limit' => 20,
	'page'  => $page,
	'filter'=> 'convert_novel_classes',
	'counter' => true,
);
if(trim($thename)) {
	$sql['where'] = " WHERE title LIKE '%".trim($thename)."%' ";
}

//$sql="SELECT count(*) as maxnum FROM ".__TAB_BOOK__.$sqladd;
//$row=$db->selectBySql($sql, 'array_shift');

//$sql="SELECT id, title, username, author, intro, attrib, dateline, updatetime, size, state, type_id FROM ".__TAB_BOOK__." $sqladd ORDER BY updatetime DESC ".$pagelink['limit'];
//$result=$db->query($sql);
//$booklist=array();

$booklist = $db->select($sql);

$pagelink = Page($db->counter, 20);
/*foreach($booklist as $key => $row) {
	if($row['C_state']==1) $status='连载中';
	elseif($row['C_state']==2) $status='暂停中';
	else $status='全书完';

	$booklist[$key]['C_attribsub'] = csubstr($row['C_attrib'], 0, 4);
	$booklist[$key]['C_booktype']  = csubstr($_CACHE['booktypebytid'][$row['C_type_id']], 0, 4);
	$booklist[$key]['C_introsub']  = PrintCodes(csubstr($row['C_intro'], 0, 500));
	$booklist[$key]['C_status']    = $status;
}*/

//显示
$tpl = new AOKOTpl("setworks");
$tpl->assign("C_thename",$thename);
$tpl->assign("booklist",html_show($booklist, false));
$tpl->assign("C_pagelink",$pagelink);
$tpl->output();

