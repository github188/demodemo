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
//数据调用程序


define('IN_Read8', true);
require_once str_replace(array('\\', '/'.basename(__FILE__)), array('/', ''), __FILE__).'/include/init.php';

$arguments = $hash = '';
isset($_GET['argument']) && $argument = $_GET['argument'];
isset($_GET['hash']) && $hash = $_GET['hash'];

$arguments = unserialize(base64_decode($argument));
if(empty($arguments) || !is_array($arguments))
	jsdie('Bad Request. Please check your argument.');
if(empty($hash) || $hash != md5($argument))
	jsdie('Bad Request. Please check your hash.');


foreach($arguments as $k => $v)
	$$k = $v;


if(empty($limit) || !ris_int($limit))
	jsdie('Bad Request. Please limit a number.');


$table = '';

switch($kind) {
	case 'vip':
		$table = __TAB_NOVEL__;
		$where = 'vip = 1';
		break;
	case 'original':
		$table = __TAB_NOVEL__;
		$where = 'author_id > 0';
		break;
	case 'copied':
		$table = __TAB_NOVEL__;
		$where = 'author_id = 0';
		break;
	default :
		$args = explode('_', $subject);
		if(count($args) == 3 && $args[2] && ris_int($args[2])) {
			$table = $args[0] == 'story' ? __TAB_STORY__ : __TAB_NOVEL__;
			$where = ($args[1] == 'content' ? 'content' : 'subject').' = '.$args[2];
		}
		break;
}

if(!$table)
	jsdie('Bad Request. Please choose a method.');


$jscachefile = ROOT."data/cache/js_$hash.php";

$update = false;
if(!file_exists($jscachefile) || (TIMESTAMP - filemtime($jscachefile) >= $cachetime))
	$update = true;

if($update) {
	$content = "<?php if(!defined('IN_Read8')) exit('Access Denied'); ?>\n";

	//$sql = "SELECT b.id, b.title, b.type_id, b.author, c.dateline, c.title as chapter_title, c.$cid AS cid, v.name as volume_name FROM ".__TAB_BOOK__." b LEFT JOIN ".__TAB_CHAPTER__." c ON b.newchapterid=c.id LEFT JOIN ".__TAB_VOLUME__." v ON v.id=c.volume_id WHERE $where ORDER BY b.updatetime DESC LIMIT $limit";
	//$result=$db->query($sql);
	$wblock = $db->select(array(
		'field' => 'id, title, author, subject, content, dateline, lastupdate',
		'from'  => $table,
		'where' => 'WHERE state IN (1, 2, 3) AND '.$where,
		'order' => 'lastupdate DESC',
		'limit' => $limit,
		'filter'=> 'convert_'.($table == __TAB_NOVEL__ ? 'novel' : 'story').'_classes',
	));

	$wblock = replace(html_show($wblock, false));
	if(!empty($charset) && $charset != SYSCHARSET) $wblock = convert($wblock, SYSCHARSET, $charset);

	addjs('document.writeln("<style type=\"text/css\" />");');
	addjs('document.writeln("	.update div.content {background: #F7F7F7;margin-top: 1px;width: 100%;padding: 5px 0;}");');
	addjs('document.writeln("	.update div div.left {float: left;margin-left: 3px;width: 70%;}");');
	addjs('document.writeln("	.update div div.right {float: right;margin-right: 3px;width: 20%;}");');
	addjs('document.writeln("</style>");');
	addjs('document.writeln("<div class=\"update\">");');

	$external = empty($openewindow) ? '' : ($openewindow == 'target' ? ' target=\"_blank\"' : (($openewindow == 'rel') ? ' rel=\"external\"' : ''));
	foreach($wblock as $val) {
		addjs('document.writeln("<table>");');
		addjs('document.writeln("<tr><td align=\"left\" width=\"70%\">&nbsp;['.$val['subject'].']&nbsp;<a href=\"'.$_SYSTEM['SYSTEM']['SITE_ADDR'].'/'.($table == __TAB_NOVEL__ ? 'novel' : 'story').'.php?bid='.$val['id'].'\"'.$external.' style=\"color: Green;\">'.$val['title'].'</a>&nbsp;</td>");');
		/*switch($kind) {
			case 'vip':
				addjs('document.writeln("<a href=\"'.$_SYSTEM['SYSTEM']['SITE_ADDR'].'/shopinfo.php?cid='.$val['C_chapterid'].'\"'.$external.' style=\"color: Gray;\"><span>'.$val['C_volumetitle'].'&nbsp;'.$val['C_chaptertitle'].'</a></div>");');
				break;
			default:
				addjs('document.writeln("<a href=\"'.$_SYSTEM['SYSTEM']['SITE_ADDR'].'/html/'.$val['C_typeid'].'/'.$val['C_bookid'].'/'.$val['C_chapterid'].'.html\"'.$external.' style=\"color: Gray;\">'.$val['C_volumetitle'].'&nbsp;'.$val['C_chaptertitle'].'</a></div>");');
				break;
		}*/
		addjs('document.writeln("<td align=\"right\" width=\"30%\">'.$val['author'].'</a>&nbsp;&lt;'.rdate($val['lastupdate'], 'm-d').'&gt;</td></tr>");');
		addjs('document.writeln("</table>");');
	}
	unset($wblock);

	addjs('document.writeln("</div>");');

	;
	if(!rfow($jscachefile, $content))
		jsdie('Can\'t write cache file. Please check your permission.');
}

(!include $jscachefile) && jsdie('Can\'t read cache file. Please check your permission.');


function addjs($text){
	global $content;
	$content .= "$text\n";
}
function jsdie($msg){
	die('document.write("<span style=\"color: Red;\">'.$msg.'</span>");');
}


//删除换行函数 ok
function replace($string){
	if(is_array($string)) {
		foreach($string as $key => $val) {
			$string[$key] = replace($val);
		}
	}else {
		$string = str_replace("\n", '', $string);
	}
	return $string;
}
