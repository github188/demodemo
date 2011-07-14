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
/**
 * 数据连接中间层
 * @2007-9-5 15:43
 *
 * 一、属性
 * 1> $Model->debug 是否为调试模式.
 *
 * 用法:
 * $Model->debug = true ;
 *
 * 调用相应的方法,返回你要执行的SQL语句
 *
 * 二、方法
 *
 * 1> $Model->single($table,$pk,$field = array());
 *
 * 设置为单一文件模式方法
 * @param $field Array 此文件所对应的表的字段
 * @param $table string 表名
 * @param $pk string  主健字段
 * 调用此方法后即启动单一文件模式
 *
 * 2> $Model->save($data,$table='',$pk = '');
 *
 * 执行更新或插入操作
 *
 * @param $data Array 键名为所要操作的字段名,键值为所要操作的字段值.
 *
 *
 *
 */

class AOKOM{
	// 数据库访问
	var $DB;
	//表名称
	var  $table;
	//字段名称
	var $field = array();
	//表主键
	var $pk ;
	//是否为单一模式
	var $iSingle = false ;
	//是否为调试模式
	var $debug = false ;
	//分页时所包函的记录总数
	var $counter ;

	var $errno;

	//基本SQL
	var $INSERT_SQL = "INSERT INTO `%s` (%s) VALUES (%s)";
	var $UPDATE_SQL = "UPDATE `%s` SET %s WHERE %s ";
	var $DELETE_SQL = "DELETE FROM `%s` WHERE %s ";
	var $INLIST_SQL = "`%s` IN(%s)";

	/**
	 * 构造函数
	 * 需要支持的功能:
	 * 1.有待扩展成支持多种数据库
	 * 2.有待改进成支持同一文件有各种操作
	 *
	 */

	 function AOKOM($db){
		$this->DB = $db;
	}
    /**
	 * 动态函数，findBy,deleteBy有待实现
	 *
	 */
	 function __call($m, $a)
	{
		return false;
	}
	/**
	 * 设置为单一文件模式函数
	 *
	 * @param unknown_type $field
	 * @param unknown_type $table
	 * @param unknown_type $pk
	 */
	 function single($table,$pk='',$field = array()){
		$this->field = $field ;
		$this->table = $table ;
		$this->pk = $pk ;
		$this->iSingle = true ;
	}

	/**
	 * 插入数据
	 *
	 * @param unknown_type $data
	 * @param unknown_type $table
	 * @return unknown
	 */

	 function insert($data,$table = '')
	{
		if($this->iSingle){//判断是否为单一模式
			if(is_int(key($data))){//判断是否有键名
				$data = array_values($data);
				$data = array_combine($this->field,$data);
			}
			$table = empty($table) ? $this->table : $table ;
		}
		$value = array();
		$field = array();
		foreach($data as $f => $v)
		{
			array_push($field, $this->quote($f, '`'));
			array_push($value, $this->quote($v));
		}
		$sql = sprintf($this->INSERT_SQL, $table, join(",", $field), join(",", $value));
		if($this->debug) return $sql;
		return $this->execInBySql($sql);
	}

	/**
	 * 更新操作
	 *
	 * @param unknown_type $data
	 * @param unknown_type $table
	 * @param unknown_type $pk
	 * @return unknown
	 */
	 function update($data,$pk,$table = ''){
		if($this->iSingle){//判断是否为单一模式
			if(is_int(key($data))){//判断是否有键名
				$data = array_values($data);
				$data = array_combine($this->field,$data);
				foreach ($data as $ke=>$ve){
					if(is_null($ve)) unset($data[$ke]);
				}
			}
			$table = empty($table) ? $this->table : $table ;
		}
		$pk = $this->buildSql($pk);
		$value = array();
		foreach($data as $f => $v)
		{
			if(is_null($v))
				unset($data[$f]);
			else
				array_push($value, $this->quote($f, '`') . "=" . $this->quote($v));
		}
		$sql = sprintf($this->UPDATE_SQL , $table , join(",",$value) , $pk);
		if($this->debug) return $sql;
		return $this->execUpBySql($sql);
	}

	/**
	 * 保存数据,根据$pk值判断是更新还是插入
	 *
	 * @param unknown_type $date
	 * @param unknown_type $table
	 * @param unknown_type $pk
	 * @return unknown
	 */
	/*
	 function save($data,$table='',$pk = ''){

		if($pk){
			$pstr = $this->buildSql($pk);
			return $this->update($data,$table,$pstr);
		}
		else{
			return $this->insert($data,$table);
		}
	}
	*/

	/**
	 * 按主健删除数据,支持数组
	 *
	 * @param string $f 字段名
	 * @param string or array $v 字段值或数组
	 * @param string $table 所要操作的表名称
	 * @return num
	 */

	 function delete( $condition, $table = '')
	{

		$f = $this->buildSql($condition);
		$table = empty($table) ? $this->table : $table ;
		$sql = sprintf($this->DELETE_SQL , $table, $f);
		return $this->execUpBySql($sql) ;
	}

	/**
	 * 取得记录集的函数
	 *
	 * 参数是一个关键数组，有以下选项<br>
	 * field 字段列表<br>
	 * from 表名<br>
	 * where 查询条件数组<br>
	 * group 编组<br>
	 * order 排序字段<br>
	 * limit int 取的记录数<br>
	 * page
	 * @param array $condition 查询数组
	 * @return array 记录集
	 */

	 function select($condition=array())
	{
		if(!isset($condition['field']))
			$condition['field'] = "*";
		$sql = "SELECT ". ($condition['field']) ;
		if(!isset($condition['from']))
			$condition['from'] = $this->table ;
		$from= " FROM ".($condition['from']) . " ";
		$where=$join=$group=$order=$limit="";
		foreach( $condition as $k => $v )
		{
			switch($k)
			{
				case 'join':
				if(isset($v[0]) && is_array($v[0]))	//多表join
				{
					foreach($v as $j)
					{
						if(!isset($j['type']))
							$j['type'] = 'LEFT';
						$join .= "{$j['type']} JOIN {$j['table']} ON {$j['on']} ";
					}
				}
				else	//单表join
				{
					//得到 $type,$table, $on,
					extract($condition['join']);
					if(!isset($type))
						$type = 'LEFT';
					$join = "$type JOIN $table ON $on ";
				}
				break;
				case 'where':
					if(is_string($v) && false !== strpos(strtoupper($v),'WHERE'))
						$where = $v ;
					else
						$where = "WHERE ". $this->buildSql($v);
					break;
				case 'group':
					$group = " GROUP BY " . $v;
					break;
				case 'order':
					$order = " ORDER BY " . $v;
					break;
				case 'limit':
					$offset = 0;
					if(!empty($condition['page']))
						$offset = ($condition['page'] - 1) * $v;
					$count = $v;
					break;
			}
		}
		if(!empty($condition['counter']) && $condition['counter'] == true)
		{
			if(empty($condition['page']) || empty($condition['limit']))
				return false ;
			$character = empty($group) ? '*' : str_replace('GROUP BY','DISTINCT',$group);
			$countSql = "SELECT count($character) AS CNT ";
			$result = $this->DB->fetch_one($countSql.$from.$join.$where);
			$this->counter = $result['CNT'] ;
		}
		$wholeSql = $sql.$from.$join.$where.$group.$order;
		if(!empty($condition['limit']))
		$wholeSql = $this->limit($wholeSql, $count, $offset );

		if($this->debug) return $wholeSql ;
		$re = $this->DB->fetch_all($wholeSql);
		if(!empty($condition['filter']))
			$re = array_map($condition['filter'], $re ? $re : array());
		return $re;

	}

   /**
	 * 执行一条sql查询并返回结果
	 *
	 * @param string $sql
	 */

	 function selectBySql($sql, $filter='')
	{
		$re = $this->DB->fetch_all($sql);
		$this->errno = $this->DB->errno();
		if(!empty($filter) && !empty($re))
			$re = array_map($filter, $re);
		return $re;
	}
	/**
	 * 执行一条更新SQL语句
	 *
	 * @param unknown_type $sql
	 * @return int 影响行数
	 */
	 function execUpBySql($sql){
		$this->DB->query($sql);
		$this->errno = $this->DB->errno();
		return $this->DB->affected_rows();
	}
	/**
	 * 执行一条插入SQL语句
	 *
	 * @param unknown_type $sql
	 * @return unknown
	 */
	 function execInBySql($sql){
		$this->DB->query($sql);
		$this->errno = $this->DB->errno();
		return $this->DB->nextid();
	}
	/**
	 * 返回一条结果集,如果查询结果为空则返回false
	 *
	 * @param unknown_type $condition
	 * @return unknown
	 */
	 function selectOne($condition){
		$condition['limit'] = 1 ;
		$re =$this->select($condition) ;
		if($this->debug)
			return $re;
		return empty($re[0]) ? false : $re[0] ;
	}

	 function table($tblname, $tblalias = ''){
		if($tblalias) {
			return $tblname.' AS '.$tblalias;
		}else {
			//return ;
		}
	}

	 function count(){
		return $this->DB->querynum;
	}

	 function nextid($table = ''){
		if(!$table && $this->iSingle) {
			$table = $this->table;
		}
		return $this->DB->nextid($table);
	}

	 function ver($server = true){
		return $this->DB->ver($server);
	}

	/**
	 * 将字符串转化成符合数据库标准的字串
	 *
	 * @param string $str 要转化的字符串
	 * @return string 转化后的字符串
	 */

	 function quote($str, $q = "'"){
		return $q.$str.$q;
	}
	/**
	 * 根据给定的条件，组合成新的条件语句
	 *
	 * @param unknown_type $c
	 * @return unknown
	 */
	 function buildSql($c)
	{
		$sql = "";

		if(!is_array($c)){//单一模式,字符.
			if($this->pk)
				return $this->quote($this->pk, '`') . " = " . $this->quote($c);
			else
				return false;
		}

		if(is_int(key($c)) && !is_array($c[key($c)])){//单一模式,数组
			if($this->pk)
				return sprintf($this->INLIST_SQL, $this->pk , join(",", array_map(array($this,'quote'),$c)));
			else
				return false;
		}

		foreach($c as $k => $v){//
			$op = " AND ";
			if(is_int($k) && is_array($v)){//
				if(count($v) == 2 && isset($v[1]) && is_bool($v[1]))
				{
					$sql .= "(";
					$sql .= $this->buildSql($v[0]);
					$sql .= ")";
					$op = $v[1] == true ? " AND " : " OR ";
				}
			}else {
				if(is_array($v)){
					if(count($v) == 2 && isset($v[1]) && is_bool($v[1])){
						if(is_array($v[0])){
							$sql .= sprintf($this->INLIST_SQL, $k, join(",", array_map(array($this,'quote'),$v[0])));
						}
						else{
							$sql .= $k . " = " .$this->quote($v[0]);
						}
						$op = $v[1] == true ? " AND " : " OR ";
					}
					else{
						$sql .= sprintf($this->INLIST_SQL, $k, join(",", array_map(array($this,'quote'),$v)));
					}
				}
				else{
					$sql .= $k . " = " .$this->quote($v);
				}
			}
			$sql .= $op;
		}
		$sql = substr($sql, 0, -strlen($op));
		return $sql;
	}
	 function limit($sql,$count,$offset = 0){
		if($offset > 0 )
			return $sql.sprintf(" LIMIT %d, %d", $offset, $count);
		else
			return $sql." LIMIT $count";
	}

	function error(){
		return $this->DB->error();
	}
}
