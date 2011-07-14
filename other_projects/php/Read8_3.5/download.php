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
//验证码程序/打包下载程序

$obnocallback=1;
$type = '';
define('IN_Read8', true);
require_once str_replace(array('\\', '/'.basename(__FILE__)), array('/', ''), __FILE__).'/include/init.php';
require_once ROOT.'include/image.func.php';

//@set_time_limit(0);

if($action=='gethashimg') {//生成验证码
	ob_end_clean();
	_secode_generate();
	exit;
}elseif(!empty($bid)) {//打包下载
	include _config('read');

	if($type == 'zip') {
		if(!$_SYSTEM['READ']['download_package'])
			back('打包下载功能尚未开启，请联系管理员');

		if(!$Global['F_uid'] && !$_SYSTEM['READ']['download_package_guest'])
			back('请先登录');
	}else {
		if(!$_SYSTEM['READ']['download_text'])
			back('TXT 下载功能尚未开启，请联系管理员');

		if(!$Global['F_uid'] && !$_SYSTEM['READ']['download_text_guest'])
			back('请先登录');
	}

	if(!$bid || !ris_int($bid) || !($bookinfo = novel_exists($bid)))
		back('未找到该书信息');

	$booktitle  = $bookinfo['title'];
	$bookauthor = $bookinfo['author'];
	$bookpinyin = $bookinfo['pinyin'];
	$sitename   = $_SYSTEM['SYSTEM']['SITE_NAME'];
	$siteurl    = $_SYSTEM['SYSTEM']['SITE_ADDR'];
	$Lang       = GetLang(1);
	$filecomm   = convert("名称:  $booktitle\n\n作者:  $bookauthor\n\n发表时间:  ".rdate(TIMESTAMP, 'Y-m-d H:i')."\n\n制作时间:  $sitename\n\n网址: $siteurl\n\n    欢迎访问本站", SYSCHARSET, 'gbk');

	if($type == 'zip') {
		require_once ROOT.'include/zip.cls.php';
		$zipname = ROOT."data/zip/$bid.zip";
		$bookpath = ROOT.novel_url($bookinfo, 0);
		if(!is_file($zipname) || TIMESTAMP - filemtime($zipname) > 86400) {
			$zip        = new zip($zipname);
			$zipfilearr = rlistfile($bookpath);
			$gbktitle   = convert($booktitle, SYSCHARSET, 'gbk');
			foreach($zipfilearr as $val) {
				$val = convert($val, SYSCHARSET, 'gbk');
				$zip->addFile("$bookpath/$val", "$gbktitle/$val");
			}
			$zip->save($filecomm);
			unset($zip);
		}

		ob_end_clean();

		header('Content-Encoding: none');
		header('Content-Type: application/zip');
		header('Content-Length: '.filesize(ROOT.'data/zip/'.$bid.'.zip'));
		header('Content-Disposition: attachment; filename='.$bookpinyin.'.zip');

		readfile(ROOT.'data/zip/'.$bid.'.zip');
		exit;
	}elseif($type == 'txt') {
		require_once ROOT.'include/booked.func.php';
		$txt = ROOT."data/txt/$bid.txt";
		if(!is_file($txt) || TIMESTAMP-filemtime($txt)>86400) {
			$filecomm = str_replace("\n", "\r\n", $filecomm);
			rfoa($txt, $filecomm);
			$rows = $db->select(array(
				'field' => 'title, priority',
				'from'  => __TAB_NOVEL_CHAPTER__,
				'where' => array('book_id' => $bid),
				'order' => 'priority ASC',
			));
			foreach($rows as $row) {
				rfoa($txt, str_replace(array('&nbsp;', '<br />', "\n", '????'), array(' ', "\n", "\r\n", '    '), convert("\n\n\n".$row['title']."\n".strip_tags(novel_content(novel_url($bookinfo, $row['priority'])), '<img><br>'), SYSCHARSET, 'gbk')));
			}
		}
		ob_end_clean();

		header('Content-Encoding: none');
		header('Content-Type: text/plain');
		header('Content-Length: '.filesize(ROOT.'data/txt/'.$bid.'.txt'));
		header('Content-Disposition: attachment; filename='.$bookpinyin.'.txt');

		readfile(ROOT.'data/txt/'.$bid.'.txt');
		exit;
	}
}

