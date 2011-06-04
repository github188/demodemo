<?php
    class GongModel extends Module {         
        public $db ; 
        private $table = 'sina_gong'; 
        public function __construct(){ 
            parent::__construct(); 
            $this->load_class('Db'); 
            $this->db = new Db(); 
        } 
        
        public function saveGongInfo($uid, $result, $level){
            $sql = "delete from " . $this->table . " where uid='" . $uid . "'";
            $this->db->run_sql($sql);
            
            $mysqldate = date('Y-m-d H:i:s', time());
	    $phpdate = strtotime($mysqldate);
            $data = array(uid=>$uid, result=>serialize($result), update_time=>time(), level=>$level);
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
           array_push($user_list, 0);
       	   $sql = 'select uid from ' . $this->table . ' where uid in (' . join(',', $user_list) . ')';
   	          	   
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
        
        public function getGongHot($start, $limit=20, $level=1){
       	   $sql = 'select * from ' . $this->table . " where level >= $level order by level desc, update_time desc limit $start, $limit";
   	          	   
       	   $gong_list = $this->db->getData($sql);
           if($this->db->errno() != 0 && DEBUG){
               throw new Exception("Sql Error ==> $sql");
           }
	   
	   $ret_gong = array();
	   foreach($gong_list as $gong){	   
	   	array_push($ret_gong, unserialize($gong['result']));
	   }
	   #print_r($ret_gong);
	   #exit();
       	   return $ret_gong;        
        }        
        
        public function getGongHistory($start, $limit=20){
       	   $sql = 'select * from ' . $this->table . " order by update_time desc limit $start, $limit";
   	          	   
       	   $gong_list = $this->db->getData($sql);
           if($this->db->errno() != 0 && DEBUG){
               throw new Exception("Sql Error ==> $sql");
           }
	   
	   $ret_gong = array();
	   foreach($gong_list as $gong){	   
	   	array_push($ret_gong, unserialize($gong['result']));
	   }
	   #print_r($ret_gong);
	   #exit();
       	   return $ret_gong;        
        }
    } 
?>
