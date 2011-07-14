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
if(!defined('IN_Read8') || !defined('IN_Read8_ACP') || !isset($PHP_SELF)) {
	exit('Access Denied (js.php)');
}

$tpl = new AOKOTpl('js');



if($action=="getjscode") {
	$emsg = '';
	if(empty($pkind) && empty($psubject))
		$emsg .= '<li>书籍类型与书籍分类至少设定一种。</li>';

	if(empty($plimit))
		$emsg .= '<li>必须设置调用数量</li>';

	if(empty($pcachetime))
		$tpl->assign("C_warn",'<li><span style="color: Red;">缓存时间未设置，这将非常消耗服务器资源</span></li>');

	if(!$emsg) {
		$arguments['kind'] = isset($pkind) ? $pkind : '';
		$arguments['subject'] = isset($psubject) ? $psubject : '';
		$arguments['limit'] = $plimit;
		$arguments['charset'] = isset($pcharset) ? $pcharset : '';
		$arguments['cachetime'] = isset($pcachetime) ? $pcachetime : '';
		$arguments['openewindow'] = isset($popeninewindow) ? $popeninewindow : '';
		$arguments['openewindow'] = $arguments['openewindow'] ? (empty($pnewindowmethod) ? 'target' : 'rel') : '';
		$argument = base64_encode(serialize($arguments));
		$hash = md5($argument);
		$code = '<script type="text/javascript" src="'.$_SYSTEM['SYSTEM']['SITE_ADDR'].'/js.php?argument='.$argument.'&hash='.$hash.'"></script>';
		$tpl->assign("C_code",$code);
	}else {
		$tpl->assign("C_emsg",$emsg);
	}
}

include _class('novel', 'subject');
include _class('novel', 'content');
include _class('story', 'subject');
include _class('story', 'content');
$height=count($_CATEGORY['NOVEL']['subject']);
$height=intval($height * 16);

$tpl->assign(array(
	'novel_subject' => $_CATEGORY['NOVEL']['subject'],
	'novel_content' => $_CATEGORY['NOVEL']['content'],
	'story_subject' => $_CATEGORY['STORY']['subject'],
	'story_content' => $_CATEGORY['STORY']['content'],
));
$tpl->assign("C_height",$height);
$tpl->output();

