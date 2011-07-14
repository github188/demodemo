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

$bid = '';

define('IN_Read8', true);
require_once str_replace(array('\\', '/'.basename(__FILE__)), array('/', ''), __FILE__).'/include/init.php';

include _config('read');


if(!$_SYSTEM['READ']['fulltext'])
	back('ȫ���Ķ�������δ����������ϵ����Ա');

if(!$Global['F_uid'] && !$_SYSTEM['READ']['fulltext_guest'])
	back('���ȵ�¼');


if(!$bid || !ris_int($bid) || !($bookinfo = novel_exists($bid)))
	back('δ�ҵ�������Ϣ');




		$chapter = array();
		$rows = $db->select(array(
			'field' => 'title, priority',
			'from'  => __TAB_NOVEL_CHAPTER__,
			'where' => array('book_id' => $bid),
			'order' => 'priority ASC',
		));
		foreach($rows as $row) {
			$chapter[] = array(
				'C_title'   => $row['title'],
				'C_orderid' => $row['priority'],
				'C_content' => str_replace(array('<br />', "\n\n", "\n"), array("\n", "\n", '<br />'), strip_tags(novel_content(novel_url($bookinfo, $row['priority'])), '<img><br>')),
			);
		}
		$tpl = new AOKOTpl('../../html/template/fulltext');
		$tpl->assign(array(
			'chapter'      => $chapter,
			'C_booktitle'  => $bookinfo['title'],
			'C_authorname' => $bookinfo['author'],
			'C_bookid'     => $bid,
		));
		$tpl->output();
