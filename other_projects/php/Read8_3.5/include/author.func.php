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


function author_by_pseudonym($pseudonym){
	global $db;

	return $db->selectOne(array(
		'from'  => __TAB_MY_AUT__,
		'where' => array(
			'pseudonym' => $pseudonym,
			'active'    => 1,
		),
	));
}

function author_interview($id = false){
	if(false !== $id && !ris_int($id))
		return false;

	global $db;

	$sql = array(
		'field' => 'ai.id, ai.author_id, ai.title, ai.content, ai.dateline, a.pseudonym',
		'from'  => $db->table(__TAB_AUT_INTERVIEW__, 'ai'),
		'join'  => array('table' => $db->table(__TAB_MY_AUT__, 'a'), 'on' => 'ai.author_id = a.id'),
		'where' => array(
			'ai.active' => 1,
			'a.active'  => 1,
		),
		'filter'=> 'html_show',
	);

	if($id) {
		$sql['where']['ai.id'] = $id;
	}else {
		$sql['order'] = 'ai.dateline DESC';
	}

	return $db->selectOne($sql);
}

