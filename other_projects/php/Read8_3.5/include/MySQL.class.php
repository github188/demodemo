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
	exit('Access Denied (MySQL.class.php)');
}

class sql_db{
	var $querynum = 0;
	var $logit = 1;

	function sql_db($dbhost, $dbuser, $dbpw, $dbname, $pconnect = 0){
		global $forcego;
		if($pconnect) {
			if(!mysql_pconnect($dbhost, $dbuser, $dbpw) && empty($forcego)) $this->halt();
		}else {
			if(!mysql_connect($dbhost, $dbuser, $dbpw) && empty($forcego)) $this->halt();
		}
		mysql_select_db($dbname);

		$ver = $this->ver();
		if($ver > '4.1') {
			$this->query("SET character_set_connection=utf8, character_set_results=utf8, character_set_client=binary");
			if($ver > '5.0.1') {
				$this->query("SET sql_mode=''");
			}
		}
		unset($ver);
	}

	function sql_close(){
		return mysql_close();
	}

	function query($sql){
		$query = mysql_query($sql);
		if(!$query) {
			!empty($this->logit) && function_exists('_log') && _log('mysql', array($this->error(), $sql));
			return false;
		}
		$this->querynum++;
		return $query;
	}

	function fetch($query, $method = MYSQL_ASSOC){
		return mysql_fetch_array($query, $method);
	}

	function fetch_one($sql, $method = MYSQL_ASSOC){
		$query = $this->query($sql);
		if(false === $query) return false;
		return $this->fetch($query, $method);
	}

	function fetch_all($sql, $method = MYSQL_ASSOC){
		$query = $this->query($sql);
		if(false === $query) return false;
		$all = array();
		while($one = $this->fetch($query, $method)) {
			$all[] = $one;
		}
		return $all;
	}

	function num_rows($query){
		return mysql_num_rows($query);
	}

	function affected_rows(){
		return mysql_affected_rows();
	}

	function num_fields($query){
		return mysql_num_fields($query);
	}

	function nextid($table = false){
		if(!$table) return mysql_insert_id();
		$status = $this->status($table);
		return $status['Auto_increment'];
	}

	function error(){
		return $this->errno().': '.mysql_error();
	}

	function errno(){
		return mysql_errno();
	}

	function free_result($query){
		return mysql_free_result($query);
	}

	function status($table){
		return $this->fetch_one("SHOW TABLE STATUS LIKE '$table'");
	}

	function halt(){
		back($this->error(), 1);
	}

	function ver($server = true){
		if($server) return mysql_get_server_info();
		return mysql_get_client_info();
	}
}

?>