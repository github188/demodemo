<?php defined('YUNPHP') or exit('can not access!');
	/**
	 * YunPHP4SAE php framework designed for SAE
	 *
	 * @author heyue <heyue@foxmail.com>
	 * @copyright Copyright(C)2010, heyue
	 * @link http://code.google.com/p/yunphp4sae/
	 * @license http://www.opensource.org/licenses/bsd-license.php New BSD License
	 * @version YunPHP4SAE version 1.0.2
	 */
	class Db extends SaeMysql {
		
		/**
		 * 通过主键获取数据
		 *
		 * @param unknown_type $table
		 * @param unknown_type $id
		 * @return unknown
		 */
		public function getById($table,$id){
			if($table == ''|| $id == ''){
				if(DEBUG){
					throw new Exception("Error == > Parameter Error!");
				}
				@Log::write_log('ERROR',"Table $table  is empty or $id is null");
				return false;
			}
			$sql = "select * from $table where id = $id";
			
			$res = $this->get_line($sql);
			return $res;
		}
		/**
		 * 删除单条记录
		 *
		 * @param 表名 $table
		 * @param 主键 $id
		 * @return 
		 */
		public function delById($table,$id){
			if($table == ''|| $id == ''){
				if(DEBUG){
					throw new Exception("Error ==> Parameter Error!");
				}
				@Log::write_log('ERROR',"Table $table  not empty or $id is null");
				return false;
			}
			$sql = "delete from $table where id = $id";
			$this->run_sql($sql);
			if($this->errno() != 0){
				if(DEBUG){
					throw new Exception("Sql Error ==> $sql");
				}
				@Log::write_log("Sql Error ==> $sql ");
				return false;
			}else{
				return true; 
			}
		}
		/**
		 * 插入一条数据
		 *
		 * @param 表名 $table
		 * @param 要插入的数组 $data
		 * @return unknown
		 */
		public function insertData($table,$data=array()){
			if($table == '' || !is_array($data)){
				if(DEBUG){
					throw new Exception("Error ==> Paramter Error!");
				}
				@Log::write_log('ERROR',"Parameter error!");
				return false;
			}		
			
			$keys = '';
			$values = '';
			foreach ($data as $k => $v){
				$keys .=" `$k` ,";
				$values .= " '$v' ,";		
			}
			$keys = substr($keys,0,-1);
			$values = substr($values,0,-1);
		
			$sql = "INSERT INTO `$table` ($keys) VALUES ($values)";
			$this->run_sql($sql);
			
			if($this->errno() != 0){
				if(DEBUG){
					throw new Exception("Sql Error ==> $sql");
				}
				@Log::write_log("Sql Error == >$sql");
				return false;
			}else{
				return true;
			}
			
		}
		
		public function updateData($table,$data,$condition = ''){
			if($table == '' || !is_array($data)){
				if(DEBUG){
					throw new Exception("Error ==> Paramter Error!");
				}
				@Log::write_log('ERROR',"$table $data Parameter error!");
				return false;
			}
			$str = '';
			foreach ($data as $k => $v) {
				$v = stripslashes($v);
				$str .= " `$k` = '$v',";
			}
			$str = substr($str,0,-1);
			if($condition != ''){
				$str .= "WHERE $condition";
			}
			$sql = "UPDATE $table SET $str";
			$this->run_sql($sql);
			if($this->errno() != 0){
				if(DEBUG){
					throw new Exception("Sql Error ==> $sql");
				}
				@Log::write_log('ERROR',"Sql Error ==> $sql");
				return false;
			}{
				return true;
			}
		}
		/**
		 * 删除记录
		 *
		 * @param unknown_type $table
		 * @param unknown_type $condition
		 * @return unknown
		 */
		public function deleteData($table,$condition){
			if($table == '' ||$condition == ''){
				if(DEBUG){
					throw new Exception("Error ==> Paramter Error!");
				}
				@Log::write_log('ERROR',"$table $data Parameter error!");
				return false;
			}
			
			$sql = "delete from $table WHERE $condition";
			$this->run_sql($sql);
			if($this->errno() != 0 ){
				if(DEBUG){
					throw new Exception("Sql Error ==>$sql");
				}
				@Log::write_log('ERROR',"Sql Error ==> $sql");
				return false;
			}else{
				return true;
			}
		}
		/**
		 * 读取数据
		 *
		 * @param unknown_type $table
		 * @param unknown_type $condition
		 */
		public function selectData($table,$condition=''){
			if($table == ''){
				@Log::write_log('ERROR',"$table $data Parameter error!");
				return false;
			}
			$sql = "select * from $table ";
			if($condition !=''){
				$sql .= " WHERE $condition";
			}
			$res = $this->get_data($sql);
			if($this->errno() != 0){
				@Log::write_log('ERROR',"Sql Error ==>$sql");
				return false;		
			}else{
				return $res;	
			}
		}
		
		public function __destruct(){
			$this->close_db();
		}
	
	}
?>