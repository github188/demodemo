<?php 
    /** 
     * 我生活无聊，也很无奈，所以开始玩SAE, 玩Weibo,玩穿越. 差点忘了我自己叫四无浪子.
     */ 
     
//HelpAction.class.php 帮助文档的控制器 
    require_once "weibooauth.php";
    
    class GongAction extends Action { 
         
        public function __construct(){ 
            #include_once( 'saet.ex.class.php' ); 
            header('Content-Type:text/html; charset=utf-8'); 
            parent::__construct();
            
            define( "WB_AKEY" , '3928577428' );
            define( "WB_SKEY" , '010cb18e764480314554e88d4fd91594' );
        }
 
        public function index(){
            $o = new SaeTOAuth( WB_AKEY , WB_SKEY  ); 
            if( $_SERVER['SERVER_PORT'] != 80 ) $port = ':'.$_SERVER['SERVER_PORT']; 

            $keys = $o->getRequestToken(); 
            $aurl = $o->getAuthorizeURL( $keys['oauth_token'] ,false , 'http://'. $_SERVER['HTTP_APPNAME'] . '.sinaapp.com' . $port . '/gong/callback/'); 
            $_SESSION['keys'] = $keys; 
             
            $this->assign('aurl',$aurl); 
            
            $this->display('gong/index.php');
        }

        public function callback(){
            $o = new SaeTOAuth(WB_AKEY, WB_SKEY, 
                               $_SESSION['keys']['oauth_token'] , 
                               $_SESSION['keys']['oauth_token_secret']); 
            
            $last_key = $o->getAccessToken($_REQUEST['oauth_verifier']) ; 
	    
            $uid = $last_key['user_id'];
            $mmc = memcache_init();
            $cache_key = $uid . '_oauth_token';
            if($mmc){
                memcache_set($mmc, $cache_key, $last_key, 30 * 60);
            }
            $c = new SaeTClient(WB_AKEY, WB_SKEY, 
                                $last_key['oauth_token'], 
                                $last_key['oauth_token_secret']); 
            $me = $c->verify_credentials(); 
            $_SESSION['cur_user'] = array('id'=>$me['id'],
                    'gender'=>$me['gender'],
                    'screen_name'=>$me['screen_name'],
                    'profile_image_url'=>$me['profile_image_url'],
                    'friends_count'=>$me['friends_count'],
                    'followers_count'=>$me['followers_count'],
                    );

            if($mmc){
                memcache_set($mmc, $uid . '_profile', $_SESSION['cur_user'], 30 * 60);
            }
            #echo $me['id'];
            header("location: /gong/u/");
            exit();
        }
        
        public function u($uid){
            #没有登录，返回登录页面。
            if(!isset($_SESSION['cur_user'])){
                header("location: /gong/index/");
                return;
            }

            $cur_id = $_SESSION['cur_user']['id'];
            if($uid && $_REQUEST['create'] == 'Y'){
                $queue = new SaeTaskQueue('create_gong'); //此处的test队列需要在在线管理平台事先建好
                $gender = $_REQUEST['gender'];
                if(!$gender) $gender='m';
                //添加单个任务
                $data = "uid=$uid&gender=$gender&oauth_id=$cur_id&key=" . WB_SKEY;
                $queue->addTask("http://yun4demo.sinaapp.com/gong/create_gong/", $data);

                #设置创建中的标准。
                $mmc = memcache_init();
                $cache_key = $uid . '_creating';
                if($mmc){
                     memcache_set($mmc, $cache_key, $data, 10);
                }
                $ret = $queue->push();
                $creating = true;
            }

            #当前用户
            $friends = $this->_get_friend_list($cur_id, 'all', $cur_id);
            
            $this->assign('friendList', $friends);
            $this->assign('me', $_SESSION['cur_user']);
            
            if($creating){
                $this->assign('gong', array('uid'=>$uid, 'loading'=>true, 'text'=>"lading......", 'image_url'=>'http://yun4demo-gong.stor.sinaapp.com/pic1929306733.jpg'));
            }else if($uid){
                $this->assign('gong', $this->get_gong($uid));
            }else {
                $this->assign('gong', $this->default_gong());
            }
 
            $this->display('gong/user_profile.php');
        }

        
        
        #返回默认的宫信息。
        private function default_gong(){
                $gong_info = array();
                $gong_info['image_url'] = "http://yun4demo-gong.stor.sinaapp.com/pic1929306733.jpg";
                $gong_info['text'] = "通过#宫#生成了他的后宫.";
		return $gong_info;
        }

        /**
	   在任务队列里面运行，不能让外部人员访问。
         */
        public function create_gong($uid, $gender='m', $oauth_id='', $key=''){
            sae_debug("uid:$uid, gender:$gender, oauth_id:$oauth_id, key:$key");

            #没有登录，返回登录页面。
	    if($_REQUEST['key'] != WB_SKEY){
		echo "接口只能在系统内部调用。需要WB_SKEY,作为参数";
                return;
            }

	    $uid = $_REQUEST['uid'];
	    $oauth_id = $_REQUEST['oauth_id'];
            $gender = $_REQUEST['gender'];
            if(!($uid && $oauth_id)){
		echo "接口只能在系统内部调用。需要WB_SKEY,作为参数";
                return;
            }
            #print "$oauth_id --> $uid --> $gender ";
            $mmc = memcache_init();
            
            $role_list = $this->_get_user_role_list($uid, $gender);
            $m_friends = $this->_get_friend_list($uid, 'm', $oauth_id);
            $f_friends = $this->_get_friend_list($uid, 'f', $oauth_id);           
            
            #print("===============$gender======><br/>");
            #print_r($role_list);
            #exit();
            
            $role_list = $this->_full_roles_with_friends($role_list, $m_friends, $f_friends);
            #print("-------------------------><br/>");      
            #print_r($role_list);

            if($mmc){
                if($oauth_id != $uid){
                   $friends = memcache_get($mmc, $oauth_id . '_friends_list');                
                   for($i = count($friends) - 1; $i >= 0; $i--){
                      if($friends[$i]['id'] != $uid)continue;
            	      $profile = $friends[$i];
                      break;
                   }
		}else {
                    $profile = memcache_get($mmc, $oauth_id . '_profile');			
                }
                print_r($profile);
            }else {
                sae_debug("memcahced error.");
            }
            
            $gong = array(user_profile => $profile, role_list=>$role_list); 
    	    $this->model('Gong');
    	    $cm = new GongModel();
            $cm->saveGongInfo($uid, $gong);
            
            $_image_info = $this->_generate_image_info($gong);
	    #print_r($_image_info);
            $json_text = json_encode($_image_info);
            #print($json_text);
            try{
               $image_url = $this->_create_image_file($uid, $json_text);
            }catch(Exception $e){
               var_dump($e->getMessage());
	       exit();
            }

            $this->_update_gong_flag($oauth_id, $uid);
                       
            if($mmc){
	          $gong_info = array();
	          $gong_info['image_url'] = $image_url;
                  $gong_info['text'] = $profile['screen_name'] . "通过#宫#生成了他的后宫.";
                  
                  $cache_key = $uid . '_gong_info';
                  memcache_set($mmc, $cache_key, $gong_info, 3 * 60);
                  memcache_delete($mmc, $uid . '_creating');
            }
            
            sae_debug("done $image_url");
            echo "done $image_url";
        }
        
        public function check_creating(){
            $uid = $_REQUEST['uid'];
            $mmc = memcache_init();
            $cache_key = $uid . '_gong_info';
            
            $result = array();
            if($mmc){
	          $gong_info = memcache_get($mmc, $cache_key);
                  if($gong_info){
                     $result['status'] = 'done';
                     $result['text'] = $gong_info['text'];
                     $result['image_url'] = $gong_info['image_url'];
                  }else {
                     $data = memcache_get($mmc, $uid . "_creating");
	             if($data){
			 $result['status'] = 'creating';
			 $result['data'] = $data;
                     }else {
                         $result['status'] = 'timeout';
                     }
                  }
            }else {
                $result['status'] = 'error';
            }
            echo json_encode($result);
        }
        
        /**
           更新缓存中好友的宫信息。
         */
        private function _update_gong_flag($uid, $friend){
            $mmc = memcache_init();
            $cache_key = $uid . '_friends_list';
            if($mmc){
                $friends = memcache_get($mmc, $cache_key);
            }
            if($friends){
                #判断当前好友信息是否有后宫。
                for($i = count($friends) - 1; $i >= 0; $i--){
                    if($friends[$i]['id'] != $friend)continue;                    
            	    $friends[$i]['have_gong'] = true;
                    break;
                }
                if($mmc){
                   memcache_set($mmc, $cache_key, $friends, 5 * 60);
                }
            }
            #print_r($friends);
        }

        private function _generate_image_info($gong){
            $image = array();
            $image['base_image'] = 'http://yun4demo.sinaapp.com/images/cybg_03.png';
            
            $combine_text = array();
            $combine_image = array();
            
            $col = 0;
            $count = 0;
            $row = -1;
            foreach($gong['role_list'] as $role){
                $col = $count % 3;
                if($col == 0)$row += 1;
                $count++;
                if(isset($role['user'])){

                    $text = $role['name'] . "-->\n". $role['user']['screen_name'];
                    array_push($combine_text, array('text'=> $text, 
                                          'x'=>20 + (95 * $col), 
                                          'y'=>20 + (95 * $row), 
                                          'color'=>'#123456'));

                    array_push($combine_image, array('image'=> $role['user']['profile_image_url'], 
                                          'x'=>20 + (95 * $col), 
                                          'y'=>40 + (95 * $row), 
                                          'color'=>'#123456'));
                }else {
                    $text = $role['name'];
                    array_push($combine_text, array('text'=> $text, 
                                          'x'=>20 + (95 * $col), 
                                          'y'=>20 + (95 * $row), 
                                          'color'=>'#123456'));
                }

            }
            $image['combine_text'] = $combine_text;
            $image['combine_image'] = $combine_image;
            return $image;
        }

        private function _create_image_file($uid, $data){
	    $ch = curl_init();
            curl_setopt_array($ch,
               array(
                  CURLOPT_URL=>'http://www.pindic.com/gen_img.php',
                  CURLOPT_RETURNTRANSFER=>true,
                  CURLOPT_POST=>true,
                  CURLOPT_POSTFIELDS=>'image_data=' . urlencode($data),
               )
            );
                
            $s = new SaeStorage();
            $content = curl_exec($ch);
            if(curl_errno($ch)){
               $content = curl_error($ch);
            }
            curl_close($ch);
            
            $fn = $s->write("gong", "pic" . $uid . ".jpg", $content);

            return $fn;
        }

        public function get_gong($uid){
            $mmc = memcache_init();
            $cache_key = $uid . '_gong_info';
            if($mmc){
                $gong_info = memcache_get($mmc, $cache_key);
            }
	    if(!$gong_info){
                $s = new SaeStorage();
	        $this->model('Gong');
    	        $cm = new GongModel();
                $gong = $cm->getGongInfo($uid);
                if($gong){
	                $gong_info = array();
	                $gong_info['image_url'] = $s->getUrl("gong", "pic" . $uid . ".jpg");
	                $gong_info['text'] = $gong['user_profile']['screen_name'] . "通过#宫#生成了他的后宫.";
		}else {
	                $gong_info = array();
	                $gong_info['image_url'] = "http://yun4demo-gong.stor.sinaapp.com/pic1929306733.jpg";
	                $gong_info['text'] = "通过#宫#生成了他的后宫.";
                }
	        if($mmc){
	             memcache_set($mmc, $cache_key, $gong_info, 5 * 60);
	        }
            }
            return $gong_info;          
        }
        
        private function _full_roles_with_friends($role_list, $m_friends, $f_friends){
            $f_role = array_filter($role_list, 
                function ($e){ return ($e['gender'] == 'f'); } 
            );
            $m_role = array_filter($role_list, 
                function ($e){ return ($e['gender'] == 'm'); } 
            );
    	    
    	    #返回一个key的list.
            $f_role_result = array_rand($f_friends, count($f_role));                        
            $m_role_result = array_rand($m_friends, count($m_role));
            
            #取到所有女性角色的index.
            $role_index_list = array_keys($f_role);
            foreach($f_role_result as $f_index){
            	$role_index = array_shift($role_index_list);
            	$role_list[$role_index]['user'] = $f_friends[$f_index];
            }

            $role_index_list = array_keys($m_role);
            foreach($m_role_result as $m_index){
            	$role_index = array_shift($role_index_list);
            	$role_list[$role_index]['user'] = $m_friends[$m_index];
            }

            return $role_list;
        }
        /**
            取的用户的好友列表，包含$id,  $gender, $screen_name, $profile_image_url

            1.如果用户好友超过100怎么处理？
        */

        private function _get_friend_list($uid, $gender='all'){
            $mmc = memcache_init();
            $cache_key = $uid . '_friends_list';
            if($mmc){
                $friends = memcache_get($mmc, $cache_key);
            }
            if(!$friends){
                $friends = $this->_get_friend_list_from_remote($uid);
                if($mmc){
                   memcache_set($mmc, $cache_key, $friends, 5 * 60);
                }
            }
            if ($gender != 'all'){
                $new_friends = array_filter($friends, 
                    function ($e) use ($gender) { return ($e['gender'] == $gender); } );
            }else {
                $new_friends = $friends;
            }

            return $new_friends;
        }
        
	public function get_friend_list_from_remote($uid){
		$ret = $this->_get_friend_list_from_remote($uid);
		print_r($ret);
		exit();
	}
        
        /**
          $uid --- 取谁的好友列表。
          $oauth_id --- 以谁的身份去取。
        */
        private function _get_friend_list_from_remote($uid, $oauth_id){
            $mmc = memcache_init();
            $cache_key = $oauth_id . '_oauth_token';
            if($mmc){
                $oauth_token = memcache_get($mmc, $cache_key);
            }
	    if(!$oauth_token) return array();

            $c = new SaeTClient(WB_AKEY, WB_SKEY , $oauth_token['oauth_token'], 
                                $oauth_token['oauth_token_secret']); 
	        $friends = $c->friends(-1, 100, $uid);
    	    $friends = $friends['users'];
            
            #用来判断用户是否有宫信息。
            $user_id_list = array();
            $ret_friends = array();
            foreach($friends as $p){
                array_push($ret_friends, array('id'=>$p['id'],
                    'gender'=>$p['gender'],
                    'screen_name'=>$p['screen_name'],
                    'profile_image_url'=>$p['profile_image_url'],
                    'friends_count'=>$p['friends_count'],
                    'followers_count'=>$p['followers_count'],
                    ));
                array_push($user_id_list, $p['id']);                
            }
            
    	    $this->model('Gong');
    	    $cm = new GongModel();
    	    
    	    #有宫信息的用户列表。
            $user_id_have_gong = $cm->getHaveGong($user_id_list);
                        
            #判断当前好友信息是否有后宫。
            for($i = count($ret_friends) - 1; $i >= 0; $i--){
            	$ret_friends[$i]['have_gong'] = in_array($ret_friends[$i]['id'], $user_id_have_gong);
            }           
            
            return $ret_friends;
        }
        
        /**
            取得人物身边需要分配的角色。
        */
        private function _get_user_role_list($uid, $gender){
            $app_config = array(
                'm' => array(
                    array(gender=>'m', name=>'太监总管'),
                    array(gender=>'f', name=>'头号宠妃'),
                    array(gender=>'m', name=>'带刀侍卫'),
                    array(gender=>'m', name=>'太傅'),
                    array(gender=>'m', name=>'太上皇'),
                    array(gender=>'f', name=>'贵妃'),
                    array(gender=>'m', name=>'御医'),
                    array(gender=>'m', name=>'御厨'),
                ),
                'f' => array(
                    array(gender=>'f', name=>'侍女总管'),
                    array(gender=>'m', name=>'头号男宠'),
                    array(gender=>'f', name=>'贴身侍女'),
                    array(gender=>'f', name=>'嬷嬷'),
                    array(gender=>'f', name=>'太皇太后'),
                    array(gender=>'m', name=>'二号男宠'),
                    array(gender=>'m', name=>'御医'),
                    array(gender=>'f', name=>'尚宫'),
                ),
            );
            return $app_config[$gender];
        }
             
    } 
?>
