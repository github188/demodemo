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
if(!defined('IN_Read8') || !defined('IN_Read8_TASK')) {
	exit('Access Denied (task.inc.php)');
}

require_once ROOT.'include/editor.func.php';
require_once ROOT.'include/booked.func.php';


// 定时发布
$drafts = $db->select(array(
	'from'  => __TAB_NOVEL_DRAFT__,
	'where' => 'WHERE active = 1 AND pubtime <= '.TIMESTAMP,
	'order' => 'pubtime ASC',
));

if($drafts) {
	foreach($drafts as $draft) {
		extract(caddslashes($draft));

		$db->update(array(
			'active' => 0,
		), array(
			'id'     => $id,
		), __TAB_NOVEL_DRAFT__);

		$bid = $book_id;
		$book = get_novel_by_id($bid);
		if(!$book)
			continue;

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
			'vip'         => $vip,
			'dateline'    => $pubtime,
			'words'       => rmb_strlen($article, true),
			'hash'        => md5($title),
		), __TAB_NOVEL_CHAPTER__);

		//章节网页生成
		$tpl = new AOKOTpl('../../html/template/content');
		$tpl->assign(array(
			'C_chaptertitle' => stripslashes($title),
			'C_booktitle'    => $book['title'],
			'C_authorname'   => $book['author'],
			'C_bookid'       => $bid,
			'C_lchapterid'   => $priority - 1,
			'C_nchapterid'   => $priority + 1,
			'C_tchapterid'   => $priority,
			'C_content'      => editor_safe_replace(UbbDecode(stripslashes($article))),
		));

		if($vip) {
			rfow(ROOT.novel_url($book, $abschapterid.'_'.md5($title)), $tpl->output(1));

			include _config('vip');
			$shopurl    = $_SYSTEM['SYSTEM']['SITE_ADDR'].'/cart.php?bid='.$bid.'&amp;cid='.$abschapterid;
			$loginurl   = $_SYSTEM['SYSTEM']['SITE_ADDR'].'/login.php?forward='.rawurlencode($shopurl);
			$vippayurl  = $_SYSTEM['SYSTEM']['SITE_ADDR'].'/shop.php';
			$vipcontent = '<div id="vip"><img src="'.$_SYSTEM['SYSTEM']['SITE_ADDR'].'/vip.php?bid='.$bid.'&amp;cid='.$abschapterid.'" usemap="#Face" alt="" /></div><map name="Face" id="Face"><area shape="rect" href="'.$shopurl.'" coords="'.$_SYSTEM['VIP']['area_buy'].'" alt="放入购物车" /><area shape="rect" href="'.$loginurl.'" coords="'.$_SYSTEM['VIP']['area_login'].'" alt="登录" /><area shape="rect" href="'.$vippayurl.'" coords="'.$_SYSTEM['VIP']['area_pay'].'" alt="购买结帐" /></map>';//增加map热点供点击
			$tpl->assign('C_content', $vipcontent);

			rfow(ROOT.novel_url($book, $priority), $tpl->output(1));
		}else {
			rfow(ROOT.novel_url($book, $priority), $tpl->output(1));
		}

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
	}
}



// VIP 章节转换
include _config('vip');
$chapters = $db->select(array(
	'field' => 'id, book_id, priority, hash',
	'from'  => __TAB_NOVEL_CHAPTER__,
	'where' => 'WHERE vip = 1 AND dateline <= '.(TIMESTAMP - 86400 * $_SYSTEM['VIP']['expire']),
));
if($chapters) {
	foreach($chapters as $chapter) {
		extract($chapter);

		$db->update(array(
			'vip' => 0,
		), array(
			'id'  => $id,
		), __TAB_NOVEL_CHAPTER__);

		$book = get_novel_by_id($book_id);
		if(!$book)
			continue;

		$path = ROOT.novel_url($book, 0).'/';

		unlink($path.$priority.'.html');
		r_mv($path.$id.'_'.$hash.'.html', $path.$priority.'.html');
	}
}



// 计算下次执行时间
$draft = $db->selectOne(array(
	'field' => 'MIN(pubtime) AS draft',
	'from'  => __TAB_NOVEL_DRAFT__,
	'where' => array(
		'active' => 1,
	),
));

$pub = $db->selectOne(array(
	'field' => 'MIN(dateline) AS pub',
	'from'  => __TAB_NOVEL_CHAPTER__,
	'where' => array(
		'vip' => 1,
	),
));

$nextrun = TIMESTAMP + 86400;
if($draft && $draft['draft'] && $draft['draft'] < $nextrun) {
	$nextrun = $draft['draft'];
}
if($pub && $pub['pub'] && $pub['pub'] + 86400 * $_SYSTEM['VIP']['expire'] < $nextrun) {
	$nextrun = $pub['pub'] + 86400 * $_SYSTEM['VIP']['expire'];
}

$db->update(array(
	'value'   => $nextrun,
), array(
	'config'  => 'SYSTEM',
	'`index`' => 'task',
), __TAB_SYSTEM__);
_config_update('system');
exit;
