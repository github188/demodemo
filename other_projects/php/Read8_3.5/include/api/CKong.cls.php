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
if(!defined('IN_Read8')) exit('Access Denied');

class Passport_CKong {
	function Passport_CKong($apikey,$user='',$pswd='',$mail='',$cookie=3600){
		$this->apikey = $apikey;
		$this->thestring = "cookie_time=$cookie&time=".TIMESTAMP."&name=".urlencode($user)."&pwd=".md5($pswd)."&email=".urlencode($mail);
	}
	function ChangePassword($newpswd){
		$this->thestring .= "&newpswd=".md5($pswd);
	}
	function ExternalInformation($column,$value){
		$this->thestring .= "&$column=$value";
	}
	function GetCode($action='login',$method='ENCODE'){
		if($action == 'logoff') $this->thestring = '';
		$code = $this->CKong($this->thestring,$this->apikey,$method);
		$this->action = $action;
		$this->authcode = $code;
	}
	function GetUri($address,$forward){
		$this->address = $address;
		$this->forward = $forward;
		$this->verify = md5($this->action.$this->authcode.$this->forward.$this->apikey);
		$this->forward = rawurlencode($this->forward);
		$this->authcode = rawurlencode($this->authcode);
		return "{$this->address}/passport.php?action={$this->action}&authcode={$this->authcode}&forward={$this->forward}&verify={$this->verify}";
	}

	function CKong($string, $apipswd, $action = 'ENCODE'){
		$key = md5($_SERVER['HTTP_USER_AGENT'].$apipswd);
		$string = $action == 'ENCODE' ? $string : base64_decode($string);
		$len = strlen($key);
		$code = '';
		for($i=0; $i<strlen($string); $i++) {
			$k = $i % $len;
			$code .= $string[$i] ^ $key[$k];
		}
		$code = $action == 'DECODE' ? $code : base64_encode($code);
		return $code;
	}
}
