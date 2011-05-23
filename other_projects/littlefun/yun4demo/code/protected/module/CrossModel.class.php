<?php
    class CrossModel extends Module {         
        public $db ; 
        private $table = 'sina_friends'; 
        public function __construct(){ 
            parent::__construct(); 
            $this->load_class('Db'); 
            $this->db = new Db(); 
        } 
        
        public function deleteFriends($uid){
            $sql = "delete from " . $this->table . " where out_id='" . $uid . "'";
            $this->db->run_sql($sql);
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
        
        /*
         @param $friends -- WeiBo返回的数据列表，
        */
        public function saveFriends($friends, $uid){
            foreach($friends as $user){
                $data = array(out_id=>$uid, fid=>$user['id'], friend=>$user['name']);
                $this->db->insertData($this->table, $data); 
                if($this->db->error() != 0)break;
            }
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
        
        /*
        创建一个用户的穿越信息。
        @param $c_id -- 需要穿越操作的用户ID
        */
        public function newCrossUser($c_id){
            global $cross_cfg, $common_event_cfg;
            //随机
            $rand = rand ( 0, count ( $cross_cfg ) - 1 );//随机人物图片

            $rand1 = rand ( 0, count ( $cross_cfg[$rand]["time"] ) - 1 );//随机时代
            $rand2 = rand ( 0, count ( $cross_cfg[$rand]["time"][$rand1]["people"] ) - 1 );//随机人物
            
            $eventCfg = array_merge($common_event_cfg,$cross_cfg[$rand]["time"][$rand1]["people"][$rand2]["event"]);//随机事件
            $rand3 = rand ( 0, count ($eventCfg) - 1 );//随机事件

            $cross ["c_rand"] = $rand;
            $cross ["c_rand1"] = $rand1;
            $cross ["c_rand2"] = $rand2;
            $cross ["c_rand3"] = $rand3;
            $cross ["out_id"] = $c_id;
            $cross ["update_time"] = date ( "Y-m-d H:i:s" );

            $sql = "delete from sina_cross where out_id=" . $c_id;
            $this->db->run_sql($sql);
            $this->db->insertData('sina_cross', $cross); 
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
        
        public function getCrossByUid($uid){
            $sql = 'select * from sina_cross where out_id=' . $uid;
            $cross = $this->db->get_line($sql);
            if($cross){
                    //随机人物图片
                    $cCfg = $cross_cfg[$cross["c_rand"]];
                    //随机时代
                    $tCfg  = $cCfg["time"][$cross["c_rand1"]];
                    //随机具体人物
                    $nCfg  = $tCfg["people"][$cross["c_rand2"]];
                    //随机事件
                    if($nCfg["event"]){
                            $eventCfg = array_merge($common_event_cfg, $nCfg["event"]);//随机事件
                    }else{
                            $eventCfg = $common_event_cfg;//随机事件
                    }
                    $eCfg = $eventCfg[$cross["c_rand3"]];
                    $cross["img"] = $cCfg["img"];
                    $cross["time"] = $tCfg["name"];
                    $cross["name"] = $nCfg["name"];
                    $cross["event"] = $eCfg;
                    return $cross;
            }else{
                    return "";
            }
        }        
    } 
?>
