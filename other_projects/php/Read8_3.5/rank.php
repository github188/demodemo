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

$times = $types = $comds = $story = $sc = '';
$page = 1;

define('IN_Read8', true);
require_once str_replace(array('\\', '/'.basename(__FILE__)), array('/', ''), __FILE__).'/include/init.php';


if($times && in_array($times, $_RANK['TIMES']) && $types && in_array($types, $_RANK['TYPES'])) {
	$sql = array(
		'field' => 'n.title,
					n.id AS bid,
					n.author,
					n.subject,
					n.content,
					n.state,
					n.dateline,
					n.lastupdate,
					ne.click_week,
					ne.click_month,
					ne.click_total,
					ne.vote_week,
					ne.vote_month,
					ne.vote_total,
					ne.rate_week,
					ne.rate_month,
					ne.rate_total,
					ne.favo_week,
					ne.favo_month,
					ne.favo_total,
					ne.commend_week,
					ne.commend_month,
					ne.commend_total,
					ne.digg_week,
					ne.digg_month,
					ne.digg_total,
					ne.bury_week,
					ne.bury_month,
					ne.bury_total,'
					.($story ? 'n.description' : 'n.author_id, n.intro, n.path'),
		'from'  => $db->table($story ? __TAB_STORY__ : __TAB_NOVEL__, 'n'),
		'join'  => array(
			array('table' => $db->table($story ? __TAB_STORY_EXT__ : __TAB_NOVEL_EXT__, 'ne'), 'on' => 'n.id = ne.book_id'),
		),
		'order' => 'ne.'.$types.'_'.$times.' DESC, n.lastupdate DESC',
	);

	$sql['filter'] = '_book_rank_callback';

	$seokeyword = array($_RANK['DESC']['TIME'][$times].$_RANK['DESC']['TYPE'][$types].'榜');
}elseif($comds && isset($_RANK['COMDS'][$comds])) {
	$sql = array(
		'field' => 'n.title,
					n.id AS bid,
					n.author,
					n.subject,
					n.content,
					n.state,
					n.dateline,
					n.lastupdate,
					ne.click_week,
					ne.click_month,
					ne.click_total,
					ne.vote_week,
					ne.vote_month,
					ne.vote_total,
					ne.rate_week,
					ne.rate_month,
					ne.rate_total,
					ne.favo_week,
					ne.favo_month,
					ne.favo_total,
					ne.commend_week,
					ne.commend_month,
					ne.commend_total,
					ne.digg_week,
					ne.digg_month,
					ne.digg_total,
					ne.bury_week,
					ne.bury_month,
					ne.bury_total,'
					.($story ? 'n.description' : 'n.author_id, n.intro, n.path'),
		'from'  => $db->table($story ? __TAB_STORY_COMMEND__ : __TAB_NOVEL_COMMEND__, 'nd'),
		'join'  => array(
			array('table' => $db->table($story ? __TAB_STORY__     : __TAB_NOVEL__,     'n'), 'on' => 'n.id = nd.book_id'),
			array('table' => $db->table($story ? __TAB_STORY_EXT__ : __TAB_NOVEL_EXT__, 'ne'), 'on' => 'nd.book_id = ne.book_id'),
		),
		'order' => 'nd.dateline DESC, n.lastupdate DESC',
	);

	$sql['filter'] = '_commend_rank_callback';

	$seokeyword = array($_RANK['DESC']['COMD'][$comds].'推荐榜');
}else {
	back('未知排行榜');
}


_header($seokeyword);//输出头部

$tpl = new AOKOTpl('rank');


if($sc && ris_int($sc))
	$sql['where']['n.subject'] = $sc;

$sql['limit'] = 20;
$sql['page']    = $page;
$sql['counter'] = 1;


$tpl->assign(array(
	'rank_title' => $seokeyword,
	'ranklist'   => $db->select($sql),
	'pagelink'   => page($db->counter, 20),
));


$tpl->output();


_footer();






