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
//后台书籍更新

if(!defined('IN_Read8') || !defined('IN_Read8_ACP') || !isset($PHP_SELF)) {
	exit('Access Denied (setbookupdate.php)');
}


require_once ROOT.'include/booked.func.php';
function_exists('set_time_limit') && set_time_limit(300);

if($action=='gomap') {//生成站内地图
	//$sql="SELECT id,title,author,type_id,initial FROM ".__TAB_BOOK__." ORDER BY initial,id";
	//$result=$db->query($sql);
	//$booklist=array();
	$booklist = $db->select(array(
		'field' => 'id      AS C_bookid,
					title   AS C_booktitle,
					author  AS C_bookauthor,
					type_id AS C_typeid,
					initial AS C_initial',
		'from'  => __TAB_BOOK__,
		'order' => 'initial, id',
	));
	$tmpinitial = '';
	foreach($booklist as $key => $row) {
		stopmark();
		if($row['C_initial'] != $tmpinitial) {
			$thisinitial = $tmpinitial = $row['C_initial'];
		}else {
			$thisinitial = '';
		}
		$booklist[$key]['C_initial'] = $thisinitial;
	}

	//生成index.xml
	$tpl = new AOKOTpl('../../html/map/xml');
	$tpl->assign('booklist',$booklist);
	rfow(ROOT."html/map/index.xml", "<?".$tpl->output(1));
	//生成index.html
	$tpl = new AOKOTpl('../../html/map/html');
	$tpl->assign('booklist',$booklist);
	rfow(ROOT."html/map/index.html", $tpl->output(1));
	unset($booklist);

	//日志记录
	$logcon="生成站内地图";
	jumpage("?ptype=$ptype&pfile=$pfile",'admin_manage_book_update_success_map');

}

if($action=='retidy') {//书籍数据重新整理
	//显示
	empty($thisnum) &&  $thisnum = $startnum;
	$tpl = new AOKOTpl('setbookupdate');
	$tpl->assign('action',$action);
	$tpl->assign('C_thisnum',$thisnum);
	$tpl->assign('C_startnum',$startnum);
	$tpl->assign('C_stopnum',$stopnum);
	$tpl->output();

	//--即时显示函数
	//ob_end_flush();
	//ob_implicit_flush();
	//echo str_repeat(' ',300);

	for($bid = $startnum; $bid <= $stopnum; $bid++) {
		if($bid < $thisnum) continue;

		if(!$bookinfo = html_show(novel_exists($bid), false))
			continue;

		stopmark();
		echo "<script type=\"text/javascript\">document.getElementById('thewatcherbook').style.display='';document.getElementById('thewatcherbook').innerHTML='正在重整书籍 &nbsp;ID: <span class=\"bname\">$bid</span> &nbsp;书名: <span class=\"bname\">&lt;".$bookinfo['title']."&gt;</span>';</script>";
		if($changefile == 'index') {
			novel_volume_tidy($bid);
			novel_rebuild_index($bid);
		}else {
			//$sql = "SELECT title,orderid FROM ".__TAB_CHAPTER__." WHERE book_id='$bid' ORDER BY orderid";
			//$result = $db->query($sql);
			$thisdir = novel_url($bookinfo, 0);
			$chapters = html_show($db->select(array(
				'field' => 'id, title, priority',
				'from'  => __TAB_NOVEL_CHAPTER__,
				'where' => array('book_id' => $bid),
				'order' => 'priority',
			)), false);
			foreach($chapters as $row) {
				stopmark();
				$thischapter = $thisdir.'/'.$row['priority'].'.html';
				$tpl = new AOKOTpl('../../html/template/content');
				$tpl->assign(array(
					'chapter_title' => $row['title'],
					'book_title'    => $bookinfo['title'],
					'C_authorname'  => $bookinfo['author'],
					'bid'           => $bid,
					'cid'           => $row['id'],
					'C_lchapterid'  => $row['priority']-1,
					'C_nchapterid'  => $row['priority']+1,
					'C_tchapterid'  => $row['priority'],
					'C_content'     => novel_content(ROOT.$thischapter),//文章内容
				));
				rfow(ROOT.$thischapter, $tpl->output(1));
				echo "<script type=\"text/javascript\">document.getElementById('thewatcher').innerHTML='正在重整章节 &nbsp;ID: <span class=\"bname\">$row[priority]</span> &nbsp;章节名: <span class=\"bname\">&lt;".$row['title']."&gt;</span>';</script>";
			}
		}
		//exit("<script type=\"text/javascript\">self.location='admin.php?ptype=$ptype&pfile=$pfile&action=retidy&startnum=$startnum&stopnum=$stopnum&thisnum=$thisnum&changefile=$changefile&obnocallback=1';</script>");
		JsTick("?ptype=$ptype&pfile=$pfile&action=retidy&startnum=$startnum&stopnum=$stopnum&thisnum=".($bid+1)."&changefile=$changefile&obnocallback=1", 1);
		//exit("<script type=\"text/javascript\">self.location='';</script>");
	}

	jumpage("?ptype=$ptype&pfile=$pfile&startnum=$startnum&stopnum=$stopnum",'admin_manage_book_update_success_page');
}


//----------显示-----------

//统计当前书籍数目
$allbooknum = $db->nextid(__TAB_NOVEL__) - 1;

$startnum = (empty($startnum) || $startnum > $allbooknum) ? 1 : $startnum;
$stopnum = (empty($stopnum) || $stopnum > $allbooknum) ? $allbooknum : $stopnum;

//显示
$tpl = new AOKOTpl("setbookupdate");
$tpl->assign('action',$action);
$tpl->assign('C_startnum',$startnum);
$tpl->assign('C_stopnum',$stopnum);
$tpl->assign('C_allbooknum',$allbooknum);
$tpl->output();

