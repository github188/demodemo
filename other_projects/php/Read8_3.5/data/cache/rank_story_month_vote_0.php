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
	exit('Access Denied (rank_story_month_vote_0.php)');
}

/* SQL: 	 array (
  'field' => 'n.title,
					n.id AS bid,
					n.author,
					n.subject,
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
					ne.bury_total,n.description',
  'from' => 'rd8_read8_story AS n',
  'join' => 
  array (
    0 => 
    array (
      'table' => 'rd8_read8_story_external AS ne',
      'on' => 'n.id = ne.book_id',
    ),
  ),
  'order' => 'ne.vote_month DESC, n.lastupdate DESC',
  'limit' => '10',
  'filter' => '_book_rank_callback',
) */

$_RANK['story_month_vote_0'] = array (
);