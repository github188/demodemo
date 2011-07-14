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
//站点相关程序

define('IN_Read8', true);
require_once str_replace(array('\\', '/'.basename(__FILE__)), array('/', ''), __FILE__).'/include/init.php';

include _config('info');


if(!in_array($action, array('about', 'guide', 'contact')))
	back('未知内容');

//设定搜索关键词
_header(array($action));//输出头部

$Lang = GetLang(1);

$tpl = new AOKOTpl('about');
$tpl->assign(array(
	'C_abouttitle' => Lang($action),
	'C_content' => show_text($_SYSTEM['INFO'][$action], $_SYSTEM['INFO']['html'][$action]),
));

$tpl->output();

_footer();
