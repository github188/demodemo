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

class Passport_Dz {
	function Passport_Dz($apikey, $user = '', $pswd = '', $mail = '', $cookie = 3600){
		$this->apikey = $apikey;
		$this->thestring = "cookietime=$cookie&time=".TIMESTAMP."&username=".urlencode($user)."&password=".md5($pswd)."&email=".urlencode($mail);
	}
	function GetCode($action = 'login', $method = 'ENCODE'){
		if($action == 'logoff') {
			$action = 'logout';
			$this->thestring = '';
		}
		$code = $this->Dz($this->thestring, $this->apikey, $method);
		$this->action = $action;
		$this->authcode = $code;
	}
	function GetUri($address, $forward){
		$this->address = $address;
		$this->forward = $forward;
		$this->verify = md5($this->action.$this->authcode.$this->forward.$this->apikey);
		$this->forward = rawurlencode($this->forward);
		$this->authcode = rawurlencode($this->authcode);
		return "{$this->address}/api/passport.php?action={$this->action}&auth={$this->authcode}&forward={$this->forward}&verify={$this->verify}";
	}

	function Dz($txt, $key, $method){
		if($method == 'ENCODE') {
			srand((double)microtime() * 1000000);
			$encrypt_key = md5(rand(0, 32000));
			$ctr = 0;
			$tmp = '';
			for($i = 0;$i < strlen($txt); $i++) {
				$ctr = $ctr == strlen($encrypt_key) ? 0 : $ctr;
				$tmp .= $encrypt_key[$ctr].($txt[$i] ^ $encrypt_key[$ctr++]);
			}
			return base64_encode($this->Dz_key($tmp, $key));
		}else {
			$txt = $this->Dz_key(base64_decode($txt), $key);
			$tmp = '';
			for($i = 0;$i < strlen($txt); $i++) {
				$md5 = $txt[$i];
				$tmp .= $txt[++$i] ^ $md5;
			}
			return $tmp;
		}
	}
	function Dz_key($txt, $encrypt_key){
		$encrypt_key = md5($encrypt_key);
		$ctr = 0;
		$tmp = '';
		for($i = 0; $i < strlen($txt); $i++) {
			$ctr = $ctr == strlen($encrypt_key) ? 0 : $ctr;
			$tmp .= $txt[$i] ^ $encrypt_key[$ctr++];
		}
		return $tmp;
	}
}