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
if(!defined('IN_Read8') || !defined('IN_Read8_ACP') || !defined('IN_Read8_ACP_COLLECT') || !isset($PHP_SELF)) {
	exit('Access Denied (collect_blacklist.php)');
}

require_once ROOT.'include/collect.func.php';

//$db->single(__TAB_COLLECT_SINGLE_RULE__);


switch($action) {
	case 'import':
		//
		break;
	case 'export':
		//
		break;
	case 'delete':
		//
		break;
	case 'edit':
		//
		break;
	case 'update':
		if(!empty($blacklist))
			_collect_blacklist(explode(LF, str_replace(CR, '', stripslashes($blacklist))));

		jumpage("?ptype=$ptype&pfile=$pfile", '编辑成功');
		break;
	default :
		$tpl = new AOKOTpl($pf.'_'.$ps);

		$tpl->assign('list', implode(CR.LF, html_show(_collect_blacklist())));

		$tpl->output();
}
