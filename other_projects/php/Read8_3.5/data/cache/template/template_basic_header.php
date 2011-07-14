<?php
if(!defined('IN_Read8')) {
	exit('Access Denied (template_basic_header.php)');
}
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=<?php
echo $_obj['G_encode'];
?>
" />
<meta name="Keywords" content="<?php
echo $_obj['keywords'];
?>
" />
<?php
if(!empty($_obj['description'])) {
?><meta name="Description" content="<?php
echo $_obj['description'];
?>
" /><?php
}
?>
<meta name="Author" content="Read8 Group & AOKOSOFT" />
<meta name="Copyright" content="Copyright (C) 2004 - 2008 AOKOSOFT Inc." />
<title><?php
echo $_obj['title'];
?>
</title>
<base href="<?php
echo $_obj['G_siteurl'];
?>
/" />
<link rel="icon" href="favicon.ico" />
<link rel="shortcut icon" href="favicon.ico" />
<link rel="Bookmark" href="favicon.ico" /> 
<link rel="stylesheet" type="text/css" media="all" href="template/<?php
echo $_obj['G_tpldir'];
?>
/basic.css" />
<script type="text/javascript" src="include/js/common.js"></script>
<script type="text/javascript"><!--//--><![CDATA[//><!--
	var G_siteurl = '<?php
echo $_obj['G_siteurl'];
?>
';

	function setTab(area,id1,id2) {
		var tabArea=document.getElementById(area);
		var contents=tabArea.childNodes;
		for(i=0; i<contents.length; i++) {
			if(contents[i].className=='rd8_catecon'){contents[i].style.display='none';}
		}
		document.getElementById(id1).style.display='block';
	
		var tabs=document.getElementById('rd8_tabs').getElementsByTagName('li');
		for(i=0; i<tabs.length; i++) {tabs[i].className='';}
		document.getElementById(id2).className='rd8_selectedtab';
	}	
	function recsetTab(area,id1,id2) {
		var tabArea=document.getElementById(area);
		var contents=tabArea.childNodes;
		for(i=0; i<contents.length; i++) {
			if(contents[i].className=='rd8_rectabcon'){contents[i].style.display='none';}
		}
		document.getElementById(id1).style.display='block';
	
		var tabs=document.getElementById('rd8_rectabs').getElementsByTagName('li');
		for(i=0; i<tabs.length; i++) {tabs[i].className='';}
		document.getElementById(id2).className='rd8_selectedtab';
	}
	function newssetTab(area,id1,id2) {
		var tabArea=document.getElementById(area);
		var contents=tabArea.childNodes;
		for(i=0; i<contents.length; i++) {
			if(contents[i].className=='rd8_newscon'){contents[i].style.display='none';}
		}
		document.getElementById(id1).style.display='block';
	
		var tabs=document.getElementById('rd8_newstabs').getElementsByTagName('li');
		for(i=0; i<tabs.length; i++) {tabs[i].className='';}
		document.getElementById(id2).className='rd8_selectedtab';
	}
//--><!]]></script>
</head>

<body>
<!--主体开始 -->
<div id="rd8_main">	
	<!--头部开始 -->
	<div id="rd8_header">
		<!--网站LOGO -->
		<a href=""><img src="images/logo.png" class="rd8_logo" alt="<?php
echo $_obj['G_sitename'];
?>
" /></a>
		<div class="rd8_nav">
			<!--网站导航栏 -->
			<a href="">网站首页</a>&nbsp;&nbsp;|&nbsp;&nbsp;<a href="viplib.php">电子书店</a>&nbsp;&nbsp;|&nbsp;&nbsp;<a href="novellib.php">大众书库</a>&nbsp;&nbsp;|&nbsp;&nbsp;<a href="my/profile.php">用户面板</a>&nbsp;&nbsp;|&nbsp;&nbsp;<a href="my/favorite.php">我的书架</a>&nbsp;&nbsp;|&nbsp;&nbsp;<a href="my/pay.php">帐户充值</a>&nbsp;&nbsp;|&nbsp;&nbsp;<a href="author/new.php">新建作品</a>&nbsp;&nbsp;|&nbsp;&nbsp;<a href="author/book.php">我的作品</a>&nbsp;&nbsp;|&nbsp;&nbsp;<a href="author/column.php">会 客 室</a>&nbsp;&nbsp;|&nbsp;&nbsp;<a href="<?php
echo $_obj['G_bbsurl'];
?>
">交流论坛</a><br />
			<?php
	if(!empty($_obj['novel_cats'])) {
		if(!is_array($_obj['novel_cats']))
			$_obj['novel_cats'] = array(array('novel_cats' => $_obj['novel_cats']));
		$_tmp_arr_keys = array_keys($_obj['novel_cats']);
		if($_tmp_arr_keys[0] != '0')
			$_obj['novel_cats'] = array(0 => $_obj['novel_cats']);
		$_stack[$_stack_cnt++] = $_obj;
		foreach($_obj['novel_cats'] as $rowcnt=>$novel_cats) {
			$novel_cats['I'] = $rowcnt;
			$novel_cats['II'] = $rowcnt+1;
			$novel_cats['III'] = $rowcnt%7;
			$novel_cats['F_uid'] = $GLOBALS['_assign']['F_uid'];
			$novel_cats['F_username'] = $GLOBALS['_assign']['F_username'];
			$novel_cats['G_sitename'] = $GLOBALS['_assign']['G_sitename'];
			$novel_cats['G_siteurl'] = $GLOBALS['_assign']['G_siteurl'];
			$novel_cats['G_style'] = $GLOBALS['_assign']['G_style'];
			$novel_cats['G_tpldir'] = $GLOBALS['_assign']['G_tpldir'];
			$novel_cats['G_PHPSELF'] = $GLOBALS['_assign']['G_PHPSELF'];
			$novel_cats['G_encode'] = $GLOBALS['_assign']['G_encode'];
			$novel_cats['TIMESTAMP'] = $GLOBALS['_assign']['TIMESTAMP'];
			$novel_cats['A'] = $rowcnt % 2;
			$novel_cats['B'] = $rowcnt % 2;
			$_obj = &$novel_cats;
?><?php
if($_obj['II'] <= "14") {
?><a href="index.php?nc=<?php
echo $_obj['id'];
?>
"><?php
echo $_obj['title'];
?>
</a><?php
if(!empty($_obj['B'])) {
?>&nbsp;&nbsp;|&nbsp;&nbsp;<?php
}
?><?php
}
?><?php
}
$_obj=$_stack[--$_stack_cnt];}
?><?php
	if(!empty($_obj['story_cats'])) {
		if(!is_array($_obj['story_cats']))
			$_obj['story_cats'] = array(array('story_cats' => $_obj['story_cats']));
		$_tmp_arr_keys = array_keys($_obj['story_cats']);
		if($_tmp_arr_keys[0] != '0')
			$_obj['story_cats'] = array(0 => $_obj['story_cats']);
		$_stack[$_stack_cnt++] = $_obj;
		foreach($_obj['story_cats'] as $rowcnt=>$story_cats) {
			$story_cats['I'] = $rowcnt;
			$story_cats['II'] = $rowcnt+1;
			$story_cats['III'] = $rowcnt%7;
			$story_cats['F_uid'] = $GLOBALS['_assign']['F_uid'];
			$story_cats['F_username'] = $GLOBALS['_assign']['F_username'];
			$story_cats['G_sitename'] = $GLOBALS['_assign']['G_sitename'];
			$story_cats['G_siteurl'] = $GLOBALS['_assign']['G_siteurl'];
			$story_cats['G_style'] = $GLOBALS['_assign']['G_style'];
			$story_cats['G_tpldir'] = $GLOBALS['_assign']['G_tpldir'];
			$story_cats['G_PHPSELF'] = $GLOBALS['_assign']['G_PHPSELF'];
			$story_cats['G_encode'] = $GLOBALS['_assign']['G_encode'];
			$story_cats['TIMESTAMP'] = $GLOBALS['_assign']['TIMESTAMP'];
			$story_cats['A'] = $rowcnt % 2;
			$story_cats['B'] = $rowcnt % 2;
			$_obj = &$story_cats;
?><a href="index.php?sc=<?php
echo $_obj['id'];
?>
"><?php
echo $_obj['title'];
?>
</a><?php
if(!empty($_obj['B'])) {
?><?php
if($_obj['II'] < "6") {
?>&nbsp;&nbsp;|&nbsp;&nbsp;<?php
}
?><?php
}
?><?php
}
$_obj=$_stack[--$_stack_cnt];}
?><br />
			<a href="novellib.php?finished=1">全本文章</a>&nbsp;&nbsp;|&nbsp;&nbsp;<a href="rank.php?times=total&amp;types=click">总点击榜</a>&nbsp;&nbsp;|&nbsp;&nbsp;<a href="rank.php?times=month&amp;types=click">月点击榜</a>&nbsp;&nbsp;|&nbsp;&nbsp;<a href="rank.php?times=week&amp;types=click">周点击榜</a>&nbsp;&nbsp;|&nbsp;&nbsp;<a href="rank.php?times=total&amp;types=commend">总推荐榜</a>&nbsp;&nbsp;|&nbsp;&nbsp;<a href="rank.php?times=month&amp;types=commend">月推荐榜</a>&nbsp;&nbsp;|&nbsp;&nbsp;<a href="rank.php?times=week&amp;types=commend">周推荐榜</a>&nbsp;&nbsp;|&nbsp;&nbsp;<a href="novellib.php?oderbycreate=1">最新入库</a>&nbsp;&nbsp;|&nbsp;&nbsp;<a href="#">最近更新</a>&nbsp;&nbsp;|&nbsp;&nbsp;<a href="#">字数排行</a>
		</div>
	</div>
	<!--头部结束 -->
		<!--广告栏 -->
		<!--img src="images/banner.gif" class="rd8_banner"/>
		<img src="images/banner.gif" class="rd8_banner"/-->
