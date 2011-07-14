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
//图片处理函数库

if(!defined('IN_Read8')) {
	exit('Access Denied (image.func.php)');
}

if(!defined('IMG_UPLOAD_PNG')) {
	define('IMG_UPLOAD_PNG', 1);
}

if(!defined('IMG_UPLOAD_GIF')) {
	define('IMG_UPLOAD_GIF', 2);
}

if(!defined('IMG_UPLOAD_JPEG')) {
	define('IMG_UPLOAD_JPEG', 4);
}

if(!defined('IMG_UPLOAD_BMP')) {
	define('IMG_UPLOAD_BMP', 8);
}

//生成VIP章节图片
function MakeVipImage($content,$filename){
	global $Global;
	//$content=iconv('gb2312','UTF-8',$content);
	$font="data/font/".$Global['vippic_font'];

	//内容排版
	$content=explode("\n",$content);
	$newstr='';
	foreach($content as $val){
		if(!trim($val)) continue;
		$val="    ".trim($val);
		$newstr.=implode("\n",mb_str_split($val, $Global['vippic_charperline']))."\n\n";
	}
	$fontposarr=ImageTTFBBox($Global['vippic_size'],0,$font,$newstr);
	$xlen=$fontposarr[4]-$fontposarr[6];
	$ylen=$fontposarr[1]-$fontposarr[7];

	//开始生成
	$im = ImageCreate($xlen,$ylen+50);
	$Global['vippic_bgcolor']=explode(" ",$Global['vippic_bgcolor']);
	ImageColorAllocate($im, $Global['vippic_bgcolor'][0],$Global['vippic_bgcolor'][1],$Global['vippic_bgcolor'][2]);//背景
	$Global['vippic_color']=explode(" ",$Global['vippic_color']);
	$color = ImageColorAllocate($im, $Global['vippic_color'][0],$Global['vippic_color'][1],$Global['vippic_color'][2]);//文字颜色

	ImageTTFText($im, $Global['vippic_size'], 0, 10, 20, $color, $font, $newstr);//写入文字

	ImageJPEG($im,$filename);
	ImageDestroy($im);
}

//增加用户编号水印
function AddUserMark($pic) {
	global $Global;
	header("Content-type: image/jpeg");
	$copyright=str_replace("[rd8vip]",$Global['F_username'],$Global['vipimagecopyright']);

	list($width, $height) = GetImageSize($pic);
	$pich=ImageCreateFromJPEG($pic);
	$gray=ImageColorAllocate($pich,225,225,225);
	$black=ImageColorAllocate($pich,255,255,255);

	//写入欢迎&vip版权
	$font="data/font/".$Global['vipimagecopyrightfont'];
	$fontposarr=ImageTTFBBox($Global['vipimagecopyrightsize'],0, $font, $copyright);
	$xlen=$fontposarr[4]-$fontposarr[6];
	ImageTTFText($pich, 12, 0, $width-$xlen, $height-40, $Global['vippic_usermarkcolor'], $font, $copyright);

	//写入uid
	$font="data/font/".$Global['vippic_usermarkfont'];
	for($ii=1; $ii<10; $ii++){
		$tmpheight=$height*$ii/10;
		ImageTTFText($pich, $Global['vippic_usermarksize'], 0, $width/2, $tmpheight, $gray, $font, $Global['F_uid']);
	}
	ImageJPEG($pich);
	ImageDestroy($pich);
}

//版权内容
function AddMark($pic) {
	global $Global, $_SYSTEM;
	$copyright=str_replace('http://','',$_SYSTEM['SYSTEM']['SITE_ADDR']);
	$size=GetImageSize($pic);
	$pich=ImageCreateFromJPEG($pic);
	$color=ImageColorAllocate($pich,255,255,255);
	ImageString($pich,5,$size[0]-strlen($copyright)*10,$size[1]-20,$copyright,$color);
	ImageJPEG($pich,$pic);
	ImageDestroy($pich);
}

function ImageVIP($content){
	global $Global;

	include _config('vip');

	$font = ROOT.'data/font/'.$_SYSTEM['VIP']['font'];
	$copyright_font = ROOT.'data/font/'.$_SYSTEM['VIP']['copyright_font'];
	$mark_font = ROOT.'data/font/'.$_SYSTEM['VIP']['mark_font'];
	if(!is_file($font) || !is_file($copyright_font) || !is_file($mark_font)) {
		readfile(ROOT.'images/lackoffonts.png');
		exit;
	}

	$content = strip_tags(str_replace(array('<br />', '<BR>'), LF, $content));
	$format = '';
	foreach(explode(LF, $content) as $v) {
		if(!trim($v))
			continue;

		$v = '    '.trim($v);
		$format .= implode(LF, mb_str_split($v, $_SYSTEM['VIP']['line'])).LF.LF;
	}

	$box = ImageTTFBBox($_SYSTEM['VIP']['size'], 0, $font, $format);
	$width  = $box[4] - $box[6] + 50;
	$height = $box[1] - $box[7] + 75;


	$im = ImageCreate($width, $height);

	list($r, $g, $b) = explode(' ', $_SYSTEM['VIP']['background']);
	ImageColorAllocateAlpha($im, $r, $g, $b, $_SYSTEM['VIP']['background_alpha']);

	list($r, $g, $b) = explode(' ', $_SYSTEM['VIP']['color']);
	$co = ImageColorAllocate($im, $r, $g, $b);

	ImageTTFText($im, $_SYSTEM['VIP']['size'], 0, 25, 25, $co, $font, $format);

	$box = ImageTTFBBox($_SYSTEM['VIP']['copyright_size'], 0, $copyright_font, str_replace('[rd8vip]', $Global['F_username'], $_SYSTEM['VIP']['copyright']));
	$copyright_width  = $box[4] - $box[6];
	$copyright_height = $box[1] - $box[7];

	ImageTTFText($im, $_SYSTEM['VIP']['copyright_size'], 0, $width - $copyright_width - 25, $height - $copyright_height - 25, $co, $copyright_font , str_replace('[rd8vip]', $Global['F_username'], $_SYSTEM['VIP']['copyright']));

	$box = ImageTTFBBox($_SYSTEM['VIP']['mark_size'], 0, $mark_font, $Global['F_uid']);
	$mark_width  = $box[4] - $box[6] + 50;
	$mark_height = $box[1] - $box[7] + 25;

	list($r, $g, $b) = explode(' ', $_SYSTEM['VIP']['mark_color']);
	$co = ImageColorAllocate($im, $r, $g, $b);

	$if = ceil($width / $mark_width);
	$jf = ceil($height / $mark_height);
	for($i = 0; $i <= $if; $i++) {
		for($j = 0; $j <= $jf; $j++) {
			ImageTTFText($im, $_SYSTEM['VIP']['mark_size'], 15, $i * $mark_width, $j * $mark_height, $co, $mark_font, $Global['F_uid']);
		}
	}

	ImagePNG($im);
}


//内容分段函数
function mb_str_split($string, $split_length, $encoding = 'UTF-8') {
	$string = convert($string, 'utf-8', 'gbk');

	mb_internal_encoding('gbk');
	$totlen = strlen($string);
	$offset = 0;
	$return = array();
	while(strlen($string)) {
		$length = $split_length * 2;
		if(strlen($string) < $length)
			$length = strlen($string);

		$offset = $split_length;
		$return[] = convert(mb_strcut($string, 0, $offset), 'gbk', 'utf-8');
		$string = mb_strcut($string, $offset);
	}
	return $return;
}


function GetImageType($filename){
	if(function_exists('exif_imagetype')) {
		$imagetype = exif_imagetype($filename);
	}else {
		$info = GetImageSize($filename);
		if(false === $info)
			return false;

		$imagetype = $info[2];
	}
	$ext = GetImageExt($imagetype);
	if(false === $ext)
		return false;

	return array($ext, GetImageMime($imagetype));
}

function GetImageExt($imagetype){
	switch($imagetype) {
		case 1:
			return 'gif';
		case 2:
			return 'jpg';
		case 3:
			return 'png';
		case 6:
			return 'bmp';
	}
	return false;
}

function GetImageMime($imagetype){
	if(function_exists('image_type_to_mime_type'))
		return image_type_to_mime_type($imagetype);

	switch($imagetype) {
		case 1:
			return 'image/gif';
		case 2:
			return 'image/jpeg';
		case 3:
			return 'image/png';
		case 4:
		case 13:
			return 'application/x-shockwave-flash';
		case 5:
			return 'image/psd';
		case 6:
			return 'image/bmp';
		case 7:
		case 8:
			return 'image/tiff';
		case 9:
		case 11:
		case 12:
			return 'application/octet-stream';
		case 10:
			return 'image/jp2';
		case 14:
			return 'image/iff';
		case 15:
			return 'image/vnd.wap.wbmp';
		case 16:
			return 'image/xbm';
	}
	return false;
}



function ImageWaterMarkImage($src, $pos, $w, $a){
	$im = ImageCreateFromJPEG($src);
	$wa = ImageCreateFromPNG($w);

	$swh = GetImageSize($src);
	$twh = GetImageSize($w);
	switch($pos) {
		case 1:
			$x = 8;
			$y = 8;
			break;
		case 2:
			$x = ($swh[0] - $twh[0]) / 2;
			$y = 8;
			break;
		case 3:
			$x = $swh[0] - $twh[0] - 8;
			$y = 8;
			break;
		case 4:
			$x = 8;
			$y = ($swh[1] - $twh[1]) / 2;
			break;
		case 5:
			$x = ($swh[0] - $twh[0]) / 2;
			$y = ($swh[1] - $twh[1]) / 2;
			break;
		case 6:
			$x = $swh[0] - $twh[0] - 8;
			$y = ($swh[1] - $twh[1]) / 2;
			break;
		case 7:
			$x = 8;
			$y = $swh[1] - $twh[1] - 8;
			break;
		case 8:
			$x = ($swh[0] - $twh[0]) / 2;
			$y = $swh[1] - $twh[1] - 8;
			break;
		case 9:
			$x = $swh[0] - $twh[0] - 8;
			$y = $swh[1] - $twh[1] - 8;
			break;
	}

	ImageCopyMerge($im, $wa, $x, $y, 0, 0, $twh[0], $twh[1], $a);
	ImageJPEG($im, $src);
}

function ImageWaterMarkText($src, $pos, $t, $c){
	$im = ImageCreateFromJPEG($src);

	list($r, $g, $b) = explode(' ', $c);
	$co = ImageColorAllocate($im, $r, $g, $b);

	$swh = GetImageSize($src);
	$twh = array(strlen($t) * 8, 15);
	switch($pos) {
		case 1:
			$x = 8;
			$y = 8;
			break;
		case 2:
			$x = ($swh[0] - $twh[0]) / 2;
			$y = 8;
			break;
		case 3:
			$x = $swh[0] - $twh[0] - 8;
			$y = 8;
			break;
		case 4:
			$x = 8;
			$y = ($swh[1] - $twh[1]) / 2;
			break;
		case 5:
			$x = ($swh[0] - $twh[0]) / 2;
			$y = ($swh[1] - $twh[1]) / 2;
			break;
		case 6:
			$x = $swh[0] - $twh[0] - 8;
			$y = ($swh[1] - $twh[1]) / 2;
			break;
		case 7:
			$x = 8;
			$y = $swh[1] - $twh[1] - 8;
			break;
		case 8:
			$x = ($swh[0] - $twh[0]) / 2;
			$y = $swh[1] - $twh[1] - 8;
			break;
		case 9:
			$x = $swh[0] - $twh[0] - 8;
			$y = $swh[1] - $twh[1] - 8;
			break;
	}

	ImageString($im, 4, $x, $y, $t, $co);

	ImageJPEG($im, $src);
}

