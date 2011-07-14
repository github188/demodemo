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
	exit('Access Denied (wap.func.php)');
}

/*
	wap 函数库
*/

require_once ROOT.'include/string.func.php';




function wap_header($title, $keywords = '', $description = '', $id = 'index'){
	if(!defined('WAP_HEADER_SENDED')) {
		define('WAP_HEADER_SENDED', true);
	}else {
		return;
	}

	header('Content-Type: text/vnd.wap.wml; charset=utf-8');
	echo '<'.'?xml version="1.0" encoding="UTF-8"?'.'>'.LF;
?>
<!DOCTYPE wml PUBLIC "-//WAPFORUM//DTD WML 1.1//EN" "http://www.wapforum.org/DTD/wml_1.1.xml">
<wml>
<?php
	if($keywords || $description) {
?>
	<head>
<?php
		if($keywords) {
?>
		<meta name="keywords" content="<?php echo is_array($keywords) ? implode(', ', $keywords) : $keywords; ?>" />
<?php
		}
		if($description) {
?>
		<meta name="description" content="<?php echo $description; ?>" />
<?php
		}
?>
	</head>
<?php
	}
?>
	<card id="wap_<?php echo $id; ?>" title="<?php echo $title; ?>">
<?php
}

function wap_footer(){
	if(!defined('WAP_FOOTER_SENDED')) {
		define('WAP_FOOTER_SENDED', true);
	}else {
		return;
	}
?>
	</card>
</wml>
<?php
	exit;
}

function wap_exit($msg = ''){
	wap_header('错误', '', '', 'error');

	if($msg)
		wap_line($msg);

	wap_line('您可以尝试 <anchor><prev />返回上一页</anchor> <a href="?">返回首页</a>');

	wap_footer();
}


function wap_line($title){
?>
	<p><?php echo $title; ?></p>
<?php
}



function wap_index(){
	global $_SYSTEM;

	wap_header($_SYSTEM['SYSTEM']['SITE_NAME'], $_SYSTEM['SEO']['keywords'], $_SYSTEM['SEO']['description']);

	wap_line('欢迎访问'.$_SYSTEM['SYSTEM']['SITE_NAME']);
	wap_line('最近更新');
	wap_lastest();

	wap_footer();
}


function wap_lastest(){
?>
	<p>
<?php
	$book = _novel();
	foreach($book as $k => $v) {
?>
		<?php echo $k + 1; ?>. <a href="?bid=<?php echo $v['bid']; ?>"><?php echo $v['title']; ?></a><br />
<?php
	}
?>
	</p>
<?php
}



function wap_novel($book, $page = 0){
	global $_SYSTEM;

	$book = html_show(convert_novel_classes($book));

	wap_header($book['title'], $book['tag'], $book['description'], 'novel');

	wap_line($book['title']);

	if(!$page) {
		wap_line('作者: '.$book['author']);
		wap_line('类型: '.$book['subject'].' - '.$book['content']);
		wap_line('介绍: '.$book['intro']);
		wap_line('<a href="?bid='.$book['bid'].'&amp;page=1">立即阅读</a>');
	}else {
		$chapters = array_chunk(_novel_chapter($book['bid']), 10);
?>
		<p>
<?php
		$cc = count($chapters);
		if(!ris_int($page))
			$page = 1;

		if($page > $cc)
			$page = $cc;

		if($page < 1)
			$page = 1;

		foreach($chapters[$page - 1] as $k => $v) {
			if(isset($v['volume_title'])) {
?>
			<?php echo $v['volume_title']; ?><br />
<?php
			}
?>
			<?php echo ($page - 1) * 10 + $k + 1; ?>. <a href="?bid=<?php echo $book['bid']; ?>&amp;cid=<?php echo $v['priority']; ?>"><?php echo $v['title']; ?></a><br />
<?php
		}
?>
		</p>
		<p>
<?php
		if($page > 1) {
?>
			<a href="?bid=<?php echo $book['bid']; ?>&amp;page=<?php echo $page - 1; ?>">上一页</a> &nbsp;
<?php
		}
		if($page < $cc) {
?>
			<a href="?bid=<?php echo $book['bid']; ?>&amp;page=<?php echo $page + 1; ?>">下一页</a> &nbsp;
<?php
		}
?>
			<input type="text" name="page" size="5" value="<?php echo $page; ?>" />/<?php echo $cc; ?>
			<anchor title="gotopage">跳
				<go href="?bid=<?php echo $book['bid']; ?>" method="get">
					<postfield name="page" value="$(page)"/>
				</go>
			</anchor>
		</p>
<?php
	}
?>
		<p><a href="?"><?php echo $_SYSTEM['SYSTEM']['SITE_NAME']; ?></a> - <?php echo $book['title']; ?></p>
<?php

	wap_footer();
}



function wap_chapter($book, $cid, $page){
	global $_SYSTEM, $psz;

	$book = html_show(convert_novel_classes($book));
	$title = novel_get_chapter_title_by_priority($book['bid'], $cid);

	if(!$title)
		wap_exit('未找到该章节');

	wap_header($title, '', '', 'chapter');

	wap_line($book['title']);
	wap_line($title);
	wap_line('作者: '.$book['author']);

	if(false === $psz)
		$psz = _cookie('psz');

	if(false === $psz || !ris_int($psz) || ($psz && ($psz < 100 || $psz > 10000)))
		$psz = 500;

	$c = trim(strip_tags(html_show_crlf_decode(novel_content(ROOT.novel_url($book, $cid)))));// novel contents
	if($psz) {
		$pages = ceil(rmb_strlen($c) / $psz / 2);// page

		if(!ris_int($page))
			$page = 1;

		if($page > $pages)
			$page = $pages;

		if($page < 1)
			$page = 1;

		wap_line(html_show_crlf(rmb_substr($c, ($page - 1) * $psz, $psz)));
	}else {
		wap_line(html_show_crlf($c));
	}
	if($psz) {
?>
		<p>
<?php
		if($page > 1) {
?>
			<a href="?bid=<?php echo $book['bid']; ?>&amp;cid=<?php echo $cid; ?>&amp;page=<?php echo $page - 1; ?>">上一页</a> &nbsp;
<?php
		}
		if($page < $pages) {
?>
			<a href="?bid=<?php echo $book['bid']; ?>&amp;cid=<?php echo $cid; ?>&amp;page=<?php echo $page + 1; ?>">下一页</a> &nbsp;
<?php
		}
?>
			<input type="text" name="page" size="5" value="<?php echo $page; ?>" />/<?php echo $pages; ?>
			<anchor title="gotopage">跳
				<go href="?bid=<?php echo $book['bid']; ?>&amp;cid=<?php echo $cid; ?>" method="get">
					<postfield name="page" value="$(page)"/>
				</go>
			</anchor>
		</p>
<?php
	}
?>
		<p>
<?php
	$prev = novel_get_chapter_title_by_priority($book['bid'], $cid - 1);
	$next = novel_get_chapter_title_by_priority($book['bid'], $cid + 1);

	if($prev) {
?>
			<a href="?bid=<?php echo $book['bid']; ?>&amp;cid=<?php echo $cid - 1; ?>">上一章: <?php echo $prev; ?></a>
<?php
	}
	if($prev && $next) {
?>
			<br />
<?php
	}
	if($next) {
?>
			<a href="?bid=<?php echo $book['bid']; ?>&amp;cid=<?php echo $cid + 1; ?>">下一章: <?php echo $next; ?></a>
<?php
	}
?>
		</p>

		<p>
			页面:&nbsp;
			<a href="?bid=<?php echo $book['bid']; ?>&amp;cid=<?php echo $cid; ?>&amp;psz=0">全</a>&nbsp;
			<a href="?bid=<?php echo $book['bid']; ?>&amp;cid=<?php echo $cid; ?>&amp;psz=2000">大</a>&nbsp;
			<a href="?bid=<?php echo $book['bid']; ?>&amp;cid=<?php echo $cid; ?>&amp;psz=1000">中</a>&nbsp;
			<a href="?bid=<?php echo $book['bid']; ?>&amp;cid=<?php echo $cid; ?>&amp;psz=500">小</a>&nbsp;
			<input type="text" name="psz" size="5" value="<?php echo $psz; ?>" />字/页
			<anchor title="gotopage">自定义
				<go href="?bid=<?php echo $book['bid']; ?>&amp;cid=<?php echo $cid; ?>" method="get">
					<postfield name="psz" value="$(psz)"/>
				</go>
			</anchor>
		</p>

		<p><a href="?"><?php echo $_SYSTEM['SYSTEM']['SITE_NAME']; ?></a> - <a href="?bid=<?php echo $book['bid']; ?>"><?php echo $book['title']; ?></a> - <?php echo $title; ?></p>
<?php
	wap_footer();
}







