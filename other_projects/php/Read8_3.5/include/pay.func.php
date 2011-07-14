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
	exit('Access Denied (pay.func.php)');
}


include _config('system');

$_PAY['ALI']['URL'] = $_SYSTEM['SYSTEM']['SITE_ADDR'].'/include/api/alipay.php';



define('PAY_API_ALI_SUCCESS', 'success');
define('PAY_API_ALI_FAIL',       'fail');


function payGateAli($subject, $tradeno, $fee, $description, $url, $async = true){
	global $_SYSTEM;

	$url .= (strpos($url, '?') ? '&' : '?').'async='.strval(intval($async));

	$fields = array(
		'_input_charset' => 'utf-8',
		'body'           => $description,
		'notify_url'     => $url,
		'out_trade_no'   => $tradeno,
		'partner'        => $_SYSTEM['PAY']['ali_partner'],
		'payment_type'   => '1',
		'return_url'     => $url,
		'seller_email'   => $_SYSTEM['PAY']['ali_seller_email'],
		'service'        => 'create_direct_pay_by_user',
		'subject'        => $subject,
		'total_fee'      => $fee,
	);

	unset($fields[$async ? 'return_url' : 'notify_url']);

	$link = 'http://www.alipay.com/cooperate/gateway.do?';
	$hash = '';
	foreach($fields as $key => $val) {
		$link .= $key.'='.rawurlencode($val).'&';
		$hash .= $key.'='.$val.'&';
	}
	return $link.'sign='.md5(substr($hash, 0, -1).$_SYSTEM['PAY']['ali_sign']).'&sign_type=MD5';
}

function payGateAliResponse($async){
	$fields = $async ? $_POST : $_GET;

	ksort($fields);
	$hash = '';
	foreach($fields as $key => $val) {
		if($key == 'sign' || $key == 'sign_type' || $key == 'f' || $key == 'async' || !$val) continue;
		$hash .= $key.'='.$val.'&';
	}
	if(md5(substr($hash, 0, -1).$_SYSTEM['PAY']['ali_sign']) != $fields['sign']) return false;
	$async && $verify = file_get_contents('http://notify.alipay.com/trade/notify_query.do?notify_id='.$fields['notify_id'].'&partner='.$_SYSTEM['PAY']['ali_partner']);
	if($async && $verify != 'true') return false;
	return $fields['out_trade_no'];
}
