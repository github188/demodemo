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
//վ����س���

define('IN_Read8', true);
require_once str_replace(array('\\', '/'.basename(__FILE__)), array('/', ''), __FILE__).'/include/init.php';

include _config('info');


if(!in_array($action, array('about', 'guide', 'contact')))
	back('δ֪����');

//�趨�����ؼ���
_header(array($action));//���ͷ��

$Lang = GetLang(1);

$tpl = new AOKOTpl('about');
$tpl->assign(array(
	'C_abouttitle' => Lang($action),
	'C_content' => show_text($_SYSTEM['INFO'][$action], $_SYSTEM['INFO']['html'][$action]),
));

$tpl->output();

_footer();
