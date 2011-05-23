<?php
	class SaeStorage {
		public function __construct($opt_akey,$opt,$opt_skey){
			
		}
		//删除文件
		public function delete($domain,$filename){
		}
		public function errmsg(){}
		public function errno(){}
		/**
		 * 获取文件属性，filename，length
		 *
		 * @param unknown_type $domain
		 * @param unknown_type $filename
		 * @param unknown_type $opt
		 */
		public function getAttr($domain,$filename, $opt){}
		//获取容量
		public function getDomainCapacity(){
			
		}
		public function getJsonContentsAndDecode($url,$fileContent){}
		public function getList($domain,$opt_prefix,$opt_limit){}
		public function getUrl($domain,$filename){}
		public function init($akey,$skey){}
		public function initOptUrlList($_optUrlList){}
		public function parseRetData($retData){}
		public function read(){}
		public function setAuth(){}
		public function setUrl(){}
		public function writer(){}
		
	}
?>