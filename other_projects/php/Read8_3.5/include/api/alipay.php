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




$async = '';

define('IN_Read8', true);
require '../init.php';
require_once ROOT.'include/pay.func.php';

$db->single(__TAB_PAY__);

if(!$async) {
	echo PAY_API_ALI_FAIL;
	exit;
}
$tradeno = payGateAliResponse($async);

if(!$tradeno) {
	echo PAY_API_ALI_FAIL;
	exit;
}

$pid = substr($tradeno, 17);
$trade = $db->selectOne(array(
	'field' => 'uid, price, vmoney, dateline',
	'where' => array(
		'id'     => $pid,
		'state'  => 0,
		'active' => 1,
	),
));
if(!$trade || gmdate('YmdHis', $trade['dateline']) != substr($tradeno, 3, 14)) {
	echo PAY_API_ALI_FAIL;
	exit;
}

$db->update(array(
	'state'   => 1,
	'suctime' => TIMESTAMP,
), array(
	'id'      => $pid,
	'state'   => 0,
	'active'  => 1,
));


$db->execUpBySql('UPDATE '.__TAB_USER__.' SET vmoney = vmoney + '.$trade['vmoney'].' WHERE uid = '.$trade['uid']);


echo PAY_API_ALI_SUCCESS;