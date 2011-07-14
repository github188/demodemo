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
//通行证接口

if(!defined('IN_Read8')) exit('Access Denied');

function GetApiData($apiinfo, $action = '', $method = 'ENCODE', $loguser = '', $pswd = '', $mail = '', $cookie = 3600){
	global $_SYSTEM;

	$i = 1;
	$apisid = md5(rrand(10));
	foreach($apiinfo as $apicfg) {
		$action = $action == 'logoff' ? $action : 'login';
		$user = convert($loguser, SYSCHARSET, $apicfg['charset']);
		require_once ROOT.'include/api/'.$apicfg['gate'].'.cls.php';
		$clsName = 'Passport_'.$apicfg['gate'];
		$Pspt = new $clsName($apicfg['code'], $user, $pswd, $mail, $cookie);
		$Pspt->GetCode($action, $method);
		$forward = $_SYSTEM['SYSTEM']['SITE_ADDR']."/login.php?asid=$apisid&step=".($i+1);
		$uri[$i] = $Pspt->GetUri($apicfg['address'], $forward);
		$i++;
		unset($Pspt);
	}
	$uri['forward'] = empty($GLOBALS['forward']) ? $_SYSTEM['SYSTEM']['SITE_ADDR'] : $GLOBALS['forward'];
	ApiCache($apisid, base64_encode(serialize($uri)));
	return $apisid;
}

function ApiCache($asid, $content = ''){
	if(strlen($asid) != 32 && !preg_replace("/^\w+$/",$asid)) back('system_api_error');
	if(!$content) {
		$fp = rfor(ROOT."data/cache/api_$asid.php");
		$fp = trim(substr($fp,strpos($fp,"\n")));
		return unserialize(base64_decode($fp));
	}else {
		if(!rfow(ROOT."data/cache/api_$asid.php", "<?php if(!defined('IN_Read8')) exit('Access Denied'); ?>\n".$content)) {
			exit('No write permission on ./data/cache/');
		}
	}
}
