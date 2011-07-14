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
//注册检查、书评插入、最新热点关键词 的ajax程序

define('IN_Read8', true);
require_once str_replace(array('\\', '/'.basename(__FILE__)), array('/', ''), __FILE__).'/include/init.php';

switch($action) {
	case 'checkreg':
		if(!$_SYSTEM['USER']['open'] || $Global['F_uid'])
			exit;

		$hid = 0;

		if($row=$db->selectOne(array(
			'field' => 'uid',
			'from'  => __TAB_USER__,
			'where' => array('username' => $tpl_user),
		))) $hid = 2;//已有用户名

		if(Banlist($tpl_user, 'name')) $hid = 3;//禁止用户名

		echo $hid;

		exit;
	case 'gethot':
		$fd = rforr('http://top.baidu.com/', false);
		$startpos = strpos($fd, '十大小说风云榜') + strlen('十大小说风云榜');
		$endpos = strpos($fd, '</table>', $startpos);//截取标记
		$book_intro = trim(substr($fd, $startpos, $endpos - $startpos));
		preg_match_all("/href[^>]+> ([".chr(0xa1)."-".chr(0xff)."]+)<\/a>/", $book_intro, $c);
		$carr = array_slice($c[1], 0, 4);//太多关键字容易被认为堆砌
		$t = convert(implode('-',$carr), 'gbk', SYSCHARSET);
		$m = convert(implode(',',$carr), 'gbk', SYSCHARSET);
		exit("document.getElementById('ins$pos').disabled=false;document.getElementById('tmpseo$pos').value='${$pos}';");
}