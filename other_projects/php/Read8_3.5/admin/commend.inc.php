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
	exit('Access Denied (commend.inc.php)');
}



list($tp, $pf, $ps) = explode('_', $pfile, 3);

// check if legal
if(in_array($tp, array('story', 'novel')) && in_array($ps, array('face', 'subject', 'strong', 'editor', 'vip')) && ($commend = array_search($ps, $_RANK['COMDS']))) {
	if($action=='commend' && $bid && ris_int($bid) && novel_exists($bid)) {
		$db->execUpBySql("REPLACE INTO ".($tp == 'story' ? __TAB_STORY_COMMEND__ : __TAB_NOVEL_COMMEND__)." VALUES ($commend, $bid, ".TIMESTAMP.")");
		_clear_cache('rank');
		_clear_cache('cache');
		if(!empty($return)) {
			jumpage("?ptype=$ptype&pfile=$return", '推荐成功');
		}
	}elseif($action=='dcommend' && $bid && ris_int($bid)) {
		$db->delete(array(
			'commend' => $commend,
			'book_id' => $bid,
		), $tp == 'story' ? __TAB_STORY_COMMEND__ : __TAB_NOVEL_COMMEND__);
		_clear_cache('rank');
		_clear_cache('cache');
	}

	$where = 'WHERE';
	if($commend == 5) {
		if(empty($subject) || !ris_int($subject)) {
			$subject = 0;
		}
		$where .= ' subject = '.$subject.' AND';
	}

	//搜索结果
	$searchlist = array();
	if($thename) {
		$searchlist = $db->select(array(
			'field' => 'id, title',
			'from'  => $tp == 'story' ? __TAB_STORY__ : __TAB_NOVEL__,
			'where' => $where.' title LIKE \'%'.$thename.'%\'',
		));
	}


	//显示
	$tpl = new AOKOTpl($pf.'_'.$ps);
	$tpl->assign(array(
		'books' => commend($commend, false, $tp == 'story'),
	));
	if($commend == 5) {
		$tpl->assign(array(
			'subjects'  => $_CATEGORY[strtoupper($tp)]['subject'],
			'subject'   => isset($_CATEGORY[strtoupper($tp)]['IDX']['subject'][$subject]) ? $_CATEGORY[strtoupper($tp)]['IDX']['subject'][$subject] : '',
		));
	}
	$tpl->assign("C_thename",$thename);
	$tpl->assign("searchlist",$searchlist);
	$tpl->output();
}
