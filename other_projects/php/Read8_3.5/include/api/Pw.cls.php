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

class Passport_Pw {
	function Passport_Pw($apikey,$user='',$pswd='',$mail='',$cookie=3600){
		$this->apikey = $apikey;
		$this->thestring = "cookietime=$cookie&time=".TIMESTAMP."&username=".urlencode($user)."&password=".md5($pswd)."&email=".urlencode($mail);
	}
	function GetCode($action='login',$method='ENCODE'){
		if($action == 'logoff') {
			$action = 'quit';
			$this->thestring = '';
		}
		$code = $this->Pw($this->thestring,$method);
		$this->action = $action;
		$this->authcode = $code;
	}
	function GetUri($address,$forward){
		$this->address = $address;
		$this->forward = $forward;
		$this->verify = md5($this->action.$this->authcode.$this->forward.$this->apikey);
		$this->forward = rawurlencode($this->forward);
		$this->authcode = rawurlencode($this->authcode);
		return "{$this->address}/passport_client.php?action={$this->action}&userdb={$this->authcode}&forward={$this->forward}&verify={$this->verify}";
	}

	function Pw($string, $action = 'ENCODE'){
		$key	= substr(md5($_SERVER['HTTP_USER_AGENT'].$this->apikey),8,18);
		$string	= $action == 'ENCODE' ? $string : base64_decode($string);
		$len	= strlen($key);
		$code	= '';
		for($i=0; $i<strlen($string); $i++) {
			$k		= $i % $len;
			$code  .= $string[$i] ^ $key[$k];
		}
		$code = $action == 'DECODE' ? $code : base64_encode($code);
		return $code;
	}
}
