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

/*
	CURL & fsockopen HTTP Request
	Sep. 18th. 2007
	(C) 2007 www.read8.net
*/

class HTTPRequest{
	var $method = 'GET';
	var $timeout = 60;

	var $accept = '*/*';
	var $referer = '';
	var $cookiefile = 'cookie.txt';
	var $UserAgent = 'Mozilla/5.0 (compatible; HTTPRequest/1.0; http://www.read8.net; )';

	var $autoreferer = true;
	var $followlocation = true;
	var $returntransfer = true;
	var $binarytransfer = true;

	function HTTPRequest($url){
		$this->cookiefile = ROOT.'data/cookie.txt';
		$this->ParseUrl($url);
		$this->proxy_host = $this->proxy_port = '';

		if(function_exists('curl_init') && function_exists('curl_setopt') && function_exists('curl_exec')) {
			$this->agent = 'CURL';
		}elseif(function_exists('fsockopen')) {
			$this->agent = 'SOCKET';
		}
	}
	function UseProxy($host, $port = 0){
		if(!$port) {
			$proxy = explode(':', $host);
			$host = $proxy[0];
			if(empty($proxy[1])) $port = 80;
			else $port = $proxy[1];
		}
		$this->proxy_host = $host;
		$this->proxy_port = $port;
	}

	function Connect(){
		switch($this->agent) {
			case 'CURL':
				$this->ch = curl_init();
				if(false === $this->ch) return false;
				if($this->proxy_host && $this->proxy_port) curl_setopt($this->ch, CURLOPT_PROXY, $this->proxy_host.':'.$this->proxy_port);
				curl_setopt($this->ch, CURLOPT_TIMEOUT, $this->timeout);
				if($this->returntransfer) curl_setopt($this->ch, CURLOPT_BINARYTRANSFER, $this->binarytransfer);
				break;
			case 'SOCKET':
				if($this->proxy_host && $this->proxy_port) {
					$this->fp = fsockopen(gethostbyname($this->proxy_host), $this->proxy_port, $this->errno, $this->errstr, $this->timeout);
				}else {
					$this->fp = fsockopen(gethostbyname($this->host), $this->port, $this->errno, $this->errstr, $this->timeout);
				}
				if(false === $this->fp) return false;
				break;
		}
		return true;
	}
	function Post(){
		//
	}
	function UploadFile(){
		//
	}
	function Request(){
		if(!$this->referer) $this->referer = $this->host;
		switch($this->agent) {
			case 'CURL':
				curl_setopt($this->ch, CURLOPT_URL, $this->url);
				if($this->method == 'GET') curl_setopt($this->ch, CURLOPT_HTTPGET, true);
				elseif($this->method == 'POST') {
					curl_setopt($this->ch, CURLOPT_POST, true);
					curl_setopt($this->ch, CURLOPT_POSTFIELDS, $this->postfields);
				}
				curl_setopt($this->ch, CURLOPT_REFERER, $this->referer);
				curl_setopt($this->ch, CURLOPT_COOKIEFILE, $this->cookiefile);
				curl_setopt($this->ch, CURLOPT_COOKIEJAR, $this->cookiefile);
				curl_setopt($this->ch, CURLOPT_USERAGENT, $this->UserAgent);
				$this->canautoreferer = version_compare(PHP_VERSION, '5.1.0', '>=');
				if($this->canautoreferer) {
					curl_setopt($this->ch, CURLOPT_AUTOREFERER, $this->autoreferer);
					curl_setopt($this->ch, CURLOPT_FOLLOWLOCATION, $this->followlocation);
				}elseif(!$this->autoreferer || !$this->followlocation) {
					curl_setopt($this->ch, CURLOPT_FOLLOWLOCATION, $this->followlocation);
				}else {
					curl_setopt($this->ch, CURLOPT_HEADER, true);
					curl_setopt($this->ch, CURLOPT_FOLLOWLOCATION, false);
				}
				curl_setopt($this->ch, CURLOPT_RETURNTRANSFER, true);
				$content = curl_exec($this->ch);

				if($this->followlocation && $this->autoreferer && !$this->canautoreferer) {
					list($header, $content) = explode("\r\n\r\n", $content, 2);
					if(preg_match("/Location: (.+)/", $header, $location))
						$content = $this->Location($location[1]);
				}
				break;
			case 'SOCKET':
				$this->cookie = $this->ParseCookie($this->cookiefile);
				$request = $this->method.' '.$this->request.' HTTP/1.1'."\r\n";
				$request .= 'Host: '.$this->host.($this->port == 80 ? '' : ':'.$this->port)."\r\n";
				$request .= 'Accept: '.$this->accept."\r\n";
				$request .= 'Referer: '.$this->referer."\r\n";
				$this->cookie && $request .= 'Cookie: '.$this->cookie."\r\n";
				$request .= 'User-Agent: '.$this->UserAgent."\r\n";
				if($this->method == 'GET') {
					$request .= 'Pragma: no-cache'."\r\n";
					$request .= 'Cache-Control: no-cache'."\r\n";
					$request .= 'Connection: close'."\r\n\r\n";
				}elseif($this->method == 'POST') {
					$request .= 'Content-Type: application/x-www-form-urlencoded'."\r\n";
					$request .= 'Content-Length: '.strlen($this->postfields)."\r\n";
					$request .= 'Connection: Keep-Alive'."\r\n";
					$request .= "\r\n".$this->postfields."\r\n\r\n";
				}
				fwrite($this->fp, $request);
				$content = '';
				while(!feof($this->fp))
					$content .= fread($this->fp, 1024);
				fclose($this->fp);

				list($header, $content) = explode("\r\n\r\n", $content, 2);
				if(preg_match_all("/Set-Cookie: ([^;=]+=[^;]+);/", $header, $cookies))
					$this->ParseCookie($this->cookiefile, implode('; ', $cookies[1]));

				if($this->followlocation && preg_match("/Location: (.+)/", $header, $location))
					$content = $this->Location($location[1]);
				break;
		}
		if($this->returntransfer) return $content;
		else return false === $content;
	}
	function Location($url){
		$http = new HTTPRequest($url);
		$http->agent = $this->agent;
		$http->timeout = $this->timeout;

		$http->accept = $this->accept;
		if($this->autoreferer) $http->referer = $this->url;
		else $http->referer = $this->referer;
		$http->cookiefile = $this->cookiefile;
		$http->UserAgent = $this->UserAgent;

		$http->autoreferer = $this->autoreferer;
		$http->followlocation = $this->followlocation;
		$http->returntransfer = $this->returntransfer;
		$http->binarytransfer = $this->binarytransfer;

		if(false === $http->Connect()) exit("connect error");
		return $http->Request();
	}

	function ParseUrl($url){
		$this->url = trim($url);
		$info = parse_url($this->url);
		$this->host = $info['host'];
		$this->port = empty($info['port']) ? 80 : $info['port'];
		$this->request = (empty($info['path']) ? '/' : $info['path']).(empty($info['query']) ? '' : '?'.$info['query']).(empty($info['fragment']) ? '' : '#'.$info['fragment']);
	}
	function ParseCookie($cookiefile, $cookie = ''){
		$rem = '# fsockopen HTTP Cookie File (http://www.read8.net)';
		if($cookie) {
			rfow($cookiefile, $rem."\n\n".$cookie);
		}else {
			$content = rfor($cookiefile);
			if(false === strpos($content, $rem)) return '';
			$cookie = explode("\n\n", $content);
			return $cookie[1];
		}
	}
}
