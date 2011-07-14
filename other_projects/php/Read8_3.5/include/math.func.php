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
	exit('Access Denied (math.func.php)');
}

/*
	数学函数库
*/

function ip2longencode($ip){
	$sip = explode('.', $ip);
	if(!ris_int($sip[0]) || !ris_int($sip[1]) || !ris_int($sip[2]) || !ris_int($sip[3]) || $sip[0] > 255 || $sip[0] < 0 || $sip[1] > 255 || $sip[1] < 0 || $sip[2] > 255 || $sip[2] < 0 || $sip[3] > 255 || $sip[3] < 0) return false;
	return $sip[0] * 16777216 + $sip[1] * 65536 + $sip[2] * 256 + $sip[3];
}

function sizecount($filesize, $short = false) {
	if($filesize >= 1073741824) {
		$filesize = round($filesize / 1073741824 * 100) / 100 . ' G';
	}elseif($filesize >= 1048576) {
		$filesize = round($filesize / 1048576 * 100) / 100 . ' M';
	}elseif($filesize >= 1024) {
		$filesize = round($filesize / 1024 * 100) / 100 . ' K';
	}else {
		$filesize = $filesize . ' B'.($short ? '' : 'ytes');
	}
	return $filesize;
}

function rrand($len, $type = 7, $limit = false) {//1 - Number //2 - Lower Char //4 - Upper Char //8 - Chinese
	mt_srand((double)microtime() * 1000000);
	switch($type) {
		case 1:
			$charlist = '0123456789';
			break;
		case 2:
			$charlist = 'abcdefghijklmnopqrstuvwxyz';
			break;
		case 3:
			$charlist = '0123456789abcdefghijklmnopqrstuvwxyz';
			break;
		case 4:
			$charlist = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ';
			break;
		case 5:
			$charlist = '0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ';
			break;
		case 6:
			$charlist = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
			break;
		case 7:
			$charlist = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
			break;
	}
	$str = '';
	if($type == 8) {
		//$max = strlen($charlist) / 2;
		//for($i = 0; $i < $len; $i+=2) $str .= substr($charlist,mt_rand(0, $max) * 2,3);
		//$str = mb_convert_encoding($str,'utf-8','gbk');
	}else {
		$max = strlen($charlist) - 1;
		for($i = 0; $i < $len; $i++) $str .= $charlist[mt_rand(0, $max)];
		if($type == 1 && $limit !== false) while($str > $limit) $str = rrand($len, $type, $limit);
	}
	return $str;
}

function ris_int($var){
	return is_numeric($var) && floor($var) == $var;
}
