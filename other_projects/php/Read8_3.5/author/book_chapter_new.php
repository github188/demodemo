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
define('PHP_SELF_EXT', '_chapter_new');
define('PHP_SELF', basename(__FILE__, PHP_SELF_EXT.'.php'));


$bid = $title = $description = $draft = $vip = $insertbefore = $volumetitle = $gotonext = '';
$timer = array(
	'd' => '',
	'h' => '',
	'm' => '',
);

require_once '../include/init.php';
require_once ROOT.'author/category.inc.php';
require_once ROOT.'include/editor.func.php';
require_once ROOT.'include/booked.func.php';
require_once ROOT.'include/image.func.php';

_header(array($_SYSTEM['CATEGORY'][PHP_SELF]));

$tpl = new AOKOTpl('navigation_aucp');
$tpl->assign('categories', $category);
$tpl->output();

$tpl = new AOKOTpl('author_'.PHP_SELF.PHP_SELF_EXT);


$book = html_show(get_novel_by_id($bid), false);
if(!$book) {
	back('无此书信息');
}
if($book['author_id'] != $_AUTHOR['id']) {
	back('您无权管理此书');
}
if($book['state'] > 4) {
	back('此书正在审核');
}
if($book['state'] == 2) {
	back('此书暂停更新');
}
if($book['state'] == 3) {
	back('此书已完结');
}

if(!$_AUTHOR['vip']) {
	$vip = 0;
}

//提交内容($cid是章节唯一id/$orderid为章节序号id)


//检查过滤
//if(Banlist($chaptertitle) || Banlist($chaptercontent)) back('chaptered_content_banned_string');

$title   = trim($title);
$article = rtrim($article);
//$words = intval(strlen($chaptercontent)/2);

$timedelay = $timer['d'] * 86400 + $timer['h'] * 3600 + $timer['m'] * 60;


if($timedelay) {
	$db->insert(array(
		'book_id'      => $bid,
		'title'        => $title,
		'description'  => $description,
		'insertbefore' => $insertbefore,
		'volumetitle'  => $volumetitle,
		'vip'          => $vip,
		'article'      => $article,
		'dateline'     => TIMESTAMP,
		'pubtime'      => TIMESTAMP + $timedelay,
	), __TAB_NOVEL_DRAFT__);

	$draft = $db->selectOne(array(
		'field' => 'MIN(pubtime) AS draft',
		'from'  => __TAB_NOVEL_DRAFT__,
		'where' => array(
			'active' => 1,
		),
	));
	$db->update(array(
		'value'   => $draft['draft'],
	), array(
		'config'  => 'SYSTEM',
		'`index`' => 'task',
	), __TAB_SYSTEM__);
	_config_update('system');
}else {
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
		'dateline'    => TIMESTAMP,
		'words'       => rmb_strlen($article, true),
		'hash'        => md5($title),
	), __TAB_NOVEL_CHAPTER__);
		//$sql="INSERT INTO ".__TAB_CHAPTER__." VALUES(NULL, '$chaptertitle', '$pbid', '$newchapterid', '".TIMESTAMP."', '0', '".($tmpurl ? 1 : 0)."', '$words', '$tmpurl', '$chapterviewcontent', '$volumid', '".md5($chaptertitle)."')";
	//}
	if(!$vip && $_SYSTEM['UPLOAD']['open'] && $_SYSTEM['UPLOAD']['size'] && !empty($_FILES['newattach'])) {
		$article .= new_attach(upload_convert($_FILES['newattach']), $book, $abschapterid);
	}

	//---------附件管理---------------

	//新的附件
	//UpdateAttach($_FILES['attachnew'],($temp || $timed));
	//UpdateAttach($_FILES['attachnew'], false);

	/*if($temp || $timed) {
		$attachments = $attachments ? base64_encode(serialize($attachments)) : '';
		$timed && $pubtime = TIMESTAMP + $timer * 86400;
		$did = $did ? $did : NULL;
		$sql = "REPLACE INTO ".__TAB_DRAFTBOX__." VALUES('$did', '$chaptertitle', '$pbid', '$cid', '$chaptercontent', '$pubtime', ".TIMESTAMP.", '$charge', '$end', '$newvolume', '$midinsert', '$words', '$attachments', '$chapterviewcontent')";
		$db->execUpBySql($sql);
	}else {*/
		//添加章节文件
		/*if($tmpurl) {//收费章节
		//图片生成
		MakeVipImage($chaptercontent, ROOT."data/vip/$tmpurl.jpg");

		//章节临时网页生成
		$shopurl    = "$Global[siteurl]/module.php?m=vip&f=shopinfo&cid=$abschapterid";
		$loginurl   = "$Global[siteurl]/login.php?forward=".urlencode($shopurl);
		$vippayurl  = "$Global[siteurl]/module.php?m=vip&f=mobilepay";
		$vipcontent = "<div id=\"vip\"><img src=\"$Global[siteurl]/module.php?m=vip&f=read&bid=$pbid&oid=$newchapterid&noheader=1\" usemap=\"#Face\" alt=\"\" /></div><map name=\"Face\" id=\"Face\"><area shape=\"rect\" href=\"$shopurl\" coords=\"$Global[vipdeny_shoparea]\" alt=\"放入购物车\" /><area shape=\"rect\" href=\"$loginurl\" coords=\"$Global[vipdeny_loginarea]\" alt=\"登录\" /><area shape=\"rect\" href=\"$vippayurl\" coords=\"$Global[vipdeny_vippayarea]\" alt=\"购买结帐\" /></map>";//增加map热点供点击

		$tpl = new AOKOTpl('../../html/template/content');
		$tpl->assign(array(
			'C_chaptertitle' => $chaptertitle,
			'C_booktitle'    => $book_title,
			'C_authorname'   => $book_author,
			'C_bookid'       => $pbid,
			'C_lchapterid'   => $newchapterid - 1,
			'C_nchapterid'   => $end ? '' : $newchapterid + 1,
			'C_tchapterid'   => $newchapterid,
			'C_content'      => editor_safe_replace($vipcontent),//$vipcontent,
		));
		rfow(ROOT.$dir.$newchapterid.".html", $tpl->output(1));

		$resultfile = ROOT."data/vip/$tmpurl.html";
	}else {*/
		//$resultfile = ROOT.$dir.$newchapterid.".html";
	//}
	//增加附件内容

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


if($gotonext) {
	jumpage('book_chapter.php?bid='.$bid, '添加成功');
}else {
	jumpage('book.php', '添加成功');
}