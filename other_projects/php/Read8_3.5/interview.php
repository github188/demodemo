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
/*
	作家访谈
*/

$id = 0;

define('IN_Read8', true);
require_once str_replace(array('\\', '/'.basename(__FILE__)), array('/', ''), __FILE__).'/include/init.php';
require_once ROOT.'include/author.func.php';

if(!$id || !ris_int($id))
	back('未找到该访谈');

$interview = author_interview($id);
if(!$interview)
	back('未找到该访谈');



$db->single(__TAB_AUT_INTERVIEW__);


_header(array($interview['pseudonym'], '作家访谈'));

$tpl = new AOKOTpl('interview');
$tpl->assign($interview);
$tpl->output();

_footer();