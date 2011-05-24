<?php
    class GongModel extends Module {         
        public $db ; 
        private $table = 'sina_gong'; 
        public function __construct(){ 
            parent::__construct(); 
            $this->load_class('Db'); 
            $this->db = new Db(); 
        } 
        
        public function saveGongInfo($uid, $result){
            $sql = "delete from " . $this->table . " where uid='" . $uid . "'";
            $this->db->run_sql($sql);
            
            $mysqldate = date('Y-m-d H:i:s', time());
	    $phpdate = strtotime($mysqldate);
            $data = array(uid=>$uid, result=>serialize($result), update_time=>time());
            $this->db->insertData($this->table, $data);
            if($this->db->errno() != 0){
                if(DEBUG){
                    throw new Exception("Sql Error ==> $sql");
                }
                @Log::write_log("ERROR", "Sql Error ==> $sql");
                return false;
            }else {
                return true;
            }
        }
        
        public function getGongInfo($uid){
            $sql = 'select * from ' . $this->table .  ' where uid=' . $uid;
            $cross = $this->db->get_line($sql);
            if($cross){
                    $data = unserialize($cross['result']);
                    $data['update_time'] = $cross['update_time'];
                    return $data;
            }else{
                    return "";
            }
        } 
        
        public function getHaveGong($user_list){
       	   $sql = 'select uid from ' . $this->table . ' where uid in (' . join(',', $user_list) . ')';

           print_r("$sql");       	          	   
       	   $cross = $this->db->getData($sql);
           if($this->db->errno() != 0 && DEBUG){
               throw new Exception("Sql Error ==> $sql");
           }
       	   
       	   if($cross){
       	   	return array_map(function($e){return $e['uid'];}, $cross);
       	   }else{
                return array();
           }  	   
        }
    } 
?>
