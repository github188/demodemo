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
$obno = true;
$ajax = $bid = $digg = $story= '';

define('IN_Read8', true);
require_once str_replace(array('\\', '/'.basename(__FILE__)), array('/', ''), __FILE__).'/include/init.php';

$db->single($story ? __TAB_STORY_EXT__ : __TAB_NOVEL_EXT__);

if($ajax) {
	header('Content-Type: application/json; charset='.SYSCHARSET);
	if(!ris_int($bid) || !($story ? story_exists($bid) : novel_exists($bid)))
		exit('alert(\'该书不存在\');');

	if($digg > 0) {
		$cookie = _cookie_decode(($story ? 'story' : 'novel').'_digg'.$bid);
		if(!$cookie || TIMESTAMP - $cookie > 86400) {
			_cookie_encode(($story ? 'story' : 'novel').'_digg'.$bid, TIMESTAMP, 86400);//每天只能一次

			$db->execUpBySql('UPDATE '.($story ? __TAB_STORY_EXT__ : __TAB_NOVEL_EXT__).' SET digg_today = digg_today + 1 WHERE book_id = '.$bid);
			exit('$(\'rd8_digg_count\').innerHTML = parseInt($(\'rd8_digg_count\').innerHTML) + 1;$(\'rd8_digg\').innerHTML = \'顶过了\';$(\'rd8_digg\').style.color = \'Gray\';');
		}
	}else {
		$cookie = _cookie_decode(($story ? 'story' : 'novel').'_bury'.$bid);
		if(!$cookie || TIMESTAMP - $cookie > 86400) {
			_cookie_encode(($story ? 'story' : 'novel').'_bury'.$bid, TIMESTAMP, 86400);//每天只能一次

			$db->execUpBySql('UPDATE '.($story ? __TAB_STORY_EXT__ : __TAB_NOVEL_EXT__).' SET bury_today = bury_today + 1 WHERE book_id = '.$bid);
			exit('$(\'rd8_bury\').innerHTML = \'埋掉了\';$(\'rd8_bury\').style.color = \'Gray\';');
		}
	}


}
?>