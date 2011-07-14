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
//后台新书审核

if(!defined('IN_Read8') || !defined('IN_Read8_ACP') || !isset($PHP_SELF)) {
	exit('Access Denied (checkbook.php)');
}

require_once ROOT.'include/booked.func.php';

$db->single(__TAB_NOVEL_APPLY__);

//审核处理
switch($action) {
	case 'deny':
		foreach($bids as $val){
			$db->update(array(
				'verifytime' => TIMESTAMP,
			), array(
				'verifytime' => 0,
				'id'         => $val,
			));
			//日志记录
			$row = $db->selectOne(array(
				'field' => 'title',
				'where' => array(
					'verifytime' => 0,
					'id'         => $val,
				),
			));
			$logcon="新书申请拒绝<".$row['title'].">";
		}
		if(count($denybid)>1) $logcon="新书申请拒绝<共".count($denybid)."本>";

		jumpage("?ptype=$ptype&pfile=$pfile",'admin_verify_book_success_deny');
	case 'apply'://书籍申请通过
		foreach($bids as $val){
			//$sql="SELECT title,author,type_id,actor,intro,attrib,uid,username,dateline,checktime FROM ".__TAB_APPLYBOOK__." WHERE id='$val'";
			//$row=$db->fetch_one($sql);
			$bookinfoarr = $db->selectOne(array(
				'where' => array(
					'verifytime' => 0,
					'id'         => $val,
				),
			));
			//if($bookinfoarr['checktime'] != 0) back('admin_verify_book_verified');

			//更新审核
			$db->update(array(
				'verifytime' => TIMESTAMP,
				'book_id'    => novel_create(caddslashes($bookinfoarr), 'html/template/bookface.jpg'),//新增书籍处理
			), array(
				'verifytime' => 0,
				'id'         => $val,
			));

			//日志记录
			$logcon="新书申请通过<".$bookinfoarr['title'].">";
		}
		if(count($bids)>1) $logcon="新书申请通过<共".count($bids)."本>";

		jumpage("?ptype=$ptype&pfile=$pfile",'admin_verify_book_success_pass');
	case 'showps'://显示新书申请的简介
		if(!is_numeric($showpsid)) back("the ID isnt number");

		$bkif = $db->selectOne(array(
			'where' => array('id' => $showpsid),
		));
		$bkif = html_show($bkif);

		$tpl = new AOKOTpl("checkbook_detail");
		$tpl->assign($bkif);
		$tpl->output();
		exit;
}

//UpdateCache('bookmain');

//显示新书申请列表
//$row = $db->selectOne(array(
//	'field' => 'COUNT(*) AS maxnum',
//));
//$pagelink=Page($row['maxnum'],$Global['tiplist'],0,0);

//$sql="SELECT id, title, author, type_id, actor, intro, attrib, username, dateline, checktime FROM ".__TAB_APPLYBOOK__." ORDER BY checktime,dateline DESC ".$pagelink['limit'];
//$result=$db->query($sql);
$itemlist = $db->select(array(
	'where' => array('verifytime' => 0),
	'order' => 'dateline DESC',
	'filter'=> 'convert_novel_classes',
));

//显示
$tpl = new AOKOTpl("checkbook");
$tpl->assign("itemlist",html_show($itemlist, false));
//$tpl->assign("C_pagelink",$pagelink['output']);
$tpl->output();

