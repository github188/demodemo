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
//后台过滤设置

if(!defined('IN_Read8') || !defined('IN_Read8_ACP') || !isset($PHP_SELF)) {
	exit('Access Denied (setbans)');
}


$db->single(__TAB_BANLIST__);

if($action=='add' && ($banword || $banname || $banip)) {
	$kind = $banname ? 'name': ($banword?'word':'ip');
	$word=trim($banword.$banname.$banip);
	if(!$word) back('内容不存在');
	//$sql="insert into ".__TAB_BANLIST__." set kind='$kind',word='$word'";
	//$db->query($sql);
	$db->insert(array(
		'kind' => $kind,
		'word' => $word,
	));
	//日志记录
	$logcon="增加后台过滤<$sqlrow=$word>";

}elseif($action=='del' && $delid) {
	//$sql="delete from ".__TAB_BANLIST__." where id='$delid'";
	//$db->query($sql);
	$db->delete(array(
		'id' => $delid,
	));
	//日志记录
	$logcon="删除后台过滤<ID:$delid>";
}
//UpdateCache('banlist');


$wordlist = $namelist = $iplist = array();

//内容显示
$sql="select id, kind, word from ".__TAB_BANLIST__." where kind<>'username' order by id desc";
//$result=$db->query($sql);
$lists = $db->selectBySql($sql);
foreach($lists as $row) {
	${$row['kind'].'list'}[] = array(
		'C_id'       => $row['id'],
		'C_wordname' => $row['word'],
	);
}


//显示
$tpl = new AOKOTpl("setbans");
$tpl->assign("wordlist",$wordlist);
$tpl->assign("namelist",$namelist);
$tpl->assign("iplist",$iplist);
$tpl->output();

