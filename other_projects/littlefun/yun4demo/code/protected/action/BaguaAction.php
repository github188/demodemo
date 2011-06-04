<?php 
    /** 
     * 我生活无聊，也很无奈，所以开始玩SAE, 玩Weibo,玩穿越. 差点忘了我自己叫四无浪子.
     */ 
     
//HelpAction.class.php 帮助文档的控制器 
    require_once "weibooauth.php";
    
    class BaguaAction extends Action { 
         
        public function __construct(){ 
            #include_once( 'saet.ex.class.php' ); 
            header('Content-Type:text/html; charset=utf-8'); 
            parent::__construct();
            
            define( "WB_AKEY" , '2453342288' );
            define( "WB_SKEY" , '2c545e783036afe3ae1cfef1e24ba9fb' );
        }
 
        public function index(){
            $o = new SaeTOAuth( WB_AKEY , WB_SKEY  ); 
            if( $_SERVER['SERVER_PORT'] != 80 ) $port = ':'.$_SERVER['SERVER_PORT']; 

            $keys = $o->getRequestToken(); 
            $aurl = $o->getAuthorizeURL( $keys['oauth_token'] ,false , 'http://'. $_SERVER['HTTP_APPNAME'] . '.sinaapp.com' . $port . '/gong/callback/'); 
            $_SESSION['keys'] = $keys; 
             
            $this->assign('aurl',$aurl); 
	    unset($_SESSION['littlefun_url']);
            
            $this->display('gong/index.php');
        }

        public function sina_ouath(){
            $o = new SaeTOAuth( WB_AKEY , WB_SKEY  ); 
            if( $_SERVER['SERVER_PORT'] != 80 ) $port = ':'.$_SERVER['SERVER_PORT']; 

            $keys = $o->getRequestToken(); 
            $aurl = $o->getAuthorizeURL( $keys['oauth_token'] ,false , 'http://'. $_SERVER['HTTP_APPNAME'] . '.sinaapp.com' . $port . '/gong/callback/'); 
            $_SESSION['keys'] = $keys;
             
	    $_SESSION['littlefun_url'] = 'http://www.littlefun.net/apps/gong/sina/gong.php';

            header("location: $aurl");
            exit();            
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
                    'name'=>$me['name'],
                    'screen_name'=>$me['screen_name'],
                    'profile_image_url'=>$me['profile_image_url'],
                    'friends_count'=>$me['friends_count'],
                    'followers_count'=>$me['followers_count'],
                    'verified' => $me['verified'],
                    );

            if($mmc){
                memcache_set($mmc, $uid . '_profile', $_SESSION['cur_user'], 30 * 60);
            }
            #echo $me['id'];
	    if(isset($_SESSION['littlefun_url'])){
               header("location: ". $_SESSION['littlefun_url']);
            }else {
               header("location: /gong/u/");
	    }
            exit();
        }
        
        public function u($uid=''){
            #没有登录，返回登录页面。
            if(!isset($_SESSION['cur_user'])){
                header("location: /gong/index/");
                return;
            }

            $cur_id = $_SESSION['cur_user']['id'];
            if($uid && $_REQUEST['create'] == 'Y'){
                $cache_key = $uid . '_gong_info';
                $mmc = memcache_init();
                if(!memcache_get($mmc, $cache_key) || $_REQUEST['force'] == 'Y' || $uid == $cur_id){
                    $queue = new SaeTaskQueue('create_gong'); //此处的test队列需要在在线管理平台事先建好
                    $gender = $_REQUEST['gender'];
                    if(!$gender) $gender='m';
                        //添加单个任务
                    $data = "uid=$uid&gender=$gender&oauth_id=$cur_id&key=" . WB_SKEY;
                    $queue->addTask("http://littlefun.sinaapp.com/gong/create_gong/", $data);

                    $cache_key = $uid . '_creating';
                    memcache_set($mmc, $cache_key, $data, 10);
                    $ret = $queue->push();
                    $creating = true;
                }
            }

            #当前用户
            $friends = $this->_get_friend_list($cur_id, 'all', $cur_id);
            
            $this->assign('friendList', $friends);
            $this->assign('me', $_SESSION['cur_user']);
            $this->assign('gong_list', $this->_recent_gong(0));
            
            $start = $_REQUEST['p'];
            if(!$start) $start = '0';
            $this->assign('startView', $start);
            
            if($creating){
                $this->assign('gong', array('uid'=>$uid, 'loading'=>true, 'text'=>"lading......", 'image_url'=>'http://yun4demo-gong.stor.sinaapp.com/pic1929306733.jpg'));
            }else if($uid){
                $this->assign('gong', $this->get_gong($uid, $cur_id));
            }else {
                $this->assign('gong', $this->default_gong());
            }
 
            $this->display('gong/user_profile.php');
        }
        
        /**
          首页数据
        */
        public function last_gong_jsonp(){
            $gong_list = $this->_recent_gong(0, 10, $_GET['nocache'] == 'Y');
            $gong_list_data = array();
            foreach($gong_list as $g){
		array_push($gong_list_data, array('user_profile'=>$g['user_profile']));
            }
	    $jsonData = json_encode($gong_list_data);
	    echo $_GET['callback'] . '(' . $jsonData . ');';
        }

        /**
          首页数据
        */
        public function last_hot_jsonp(){
            $gong_list = $this->_recent_hot_gong(0, 10, $_GET['nocache'] == 'Y');
            $gong_list_data = array();
            foreach($gong_list as $g){
		array_push($gong_list_data, $g['user_profile']);
            }
	    $jsonData = json_encode($gong_list_data);
	    echo $_GET['callback'] . '(' . $jsonData . ');';
        }

        public function create_gong_info_jsonp(){
            $cur_id = $_SESSION['cur_user']['id'];
	    if($cur_id){
                $gong_list = $this->_recent_hot_gong(0, 6, $_GET['nocache'] == 'Y');
                $hot_list = array();
                foreach($gong_list as $g){
	            array_push($hot_list, $g['user_profile']);
                }

                $gong_list = $this->_recent_gong(0, 6, $_GET['nocache'] == 'Y');
                $recent_list = array();
                foreach($gong_list as $g){
	            array_push($recent_list, $g['user_profile']);
                }			    

		$data = array('me' => $_SESSION['cur_user'], 'recent_list'=>$recent_list, 'hot_list'=>$hot_list);		  
	     }else {
		$data = array();
	     }
	     $jsonData = json_encode($data);
	     echo $_GET['callback'] . '(' . $jsonData . ');';        
        }
	
	public function jsonp(){
		$action = $_GET["q"];
		$cur_id = $_SESSION['cur_user']['id'];
                $uid = $_GET["uid"];
		if($cur_id){
		    if(!$action){
                        $friends = $this->_get_friend_list($cur_id, 'all', $cur_id);
                        if($uid){
                          $gong = $this->get_gong($uid, $cur_id);
                        }
                        $fans = $this->_get_follower_list($cur_id, 'all', $cur_id); 

		        $data = array('me' => $_SESSION['cur_user'], 'friends'=>$friends, 'gong'=>$gong, 'fans'=>$fans);
		
		    }else if($action == 'create_gong'){
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
                        $data = $result;	
                    }
		}else {
		    $data = array();
		}
		$jsonData = json_encode($data);
		echo $_GET['callback'] . '(' . $jsonData . ');';
	}
        
        
        #返回默认的宫信息。
        private function default_gong(){
                $gong_info = array();
                $gong_info['image_url'] = "http://yun4demo-gong.stor.sinaapp.com/pic1929306733.jpg";
                $gong_info['text'] = "";
		return $gong_info;
        }

        public function out_create_gong($uid, $gender='m'){
            if(!isset($_SESSION['cur_user'])){
                header("location: /gong/index/");
                return;
            }

            $cur_id = $_SESSION['cur_user']['id'];
            if($uid){
                $cache_key = $uid . '_gong_info';
                $mmc = memcache_init();
                if(!memcache_get($mmc, $cache_key) || $_REQUEST['force'] == 'Y' || $uid == $cur_id){
                    $queue = new SaeTaskQueue('create_gong'); //此处的test队列需要在在线管理平台事先建好
                    $gender = $_REQUEST['gender'];
                    if(!$gender) $gender='m';
                        //添加单个任务
                    $data = "uid=$uid&gender=$gender&oauth_id=$cur_id&key=" . WB_SKEY;
                    $queue->addTask("http://littlefun.sinaapp.com/gong/create_gong/", $data);

                    $cache_key = $uid . '_creating';
                    memcache_set($mmc, $cache_key, $data, 10);
                    $ret = $queue->push();
                    $creating = true;
                }
            }
            header("location: http://www.littlefun.net/apps/gong/sina/result_detail.php?uid=$uid");        
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
            $mmc = memcache_init();
            
            $role_list = $this->_get_user_role_list($uid, $gender);
            $m_friends = $this->_get_friend_list($uid, 'm', $oauth_id);
            $f_friends = $this->_get_friend_list($uid, 'f', $oauth_id);           
                        
            $role_list = $this->_full_roles_with_friends($role_list, $m_friends, $f_friends);

            if($mmc){
                if($oauth_id != $uid){
                   $friends = memcache_get($mmc, $oauth_id . '_friends_list');                
                   for($i = count($friends) - 1; $i >= 0; $i--){
                      if($friends[$i]['id'] != $uid)continue;
            	      $profile = $friends[$i];
                      break;
                   }
                   if(!profile){
                       $friends = memcache_get($mmc, $oauth_id . '_follower_list');                
                       for($i = count($friends) - 1; $i >= 0; $i--){
                          if($friends[$i]['id'] != $uid)continue;
            	          $profile = $friends[$i];
                          break;
                       }
                   }
		}else {
                    $profile = memcache_get($mmc, $oauth_id . '_profile');			
                }
                print_r($profile);
            }else {
                sae_debug("memcahced error.");
            }
	    
            $text = '@'. $profile['name'] . " 通过#宫#找到了他的后宫. 原来";
            $i=0;            
            foreach($role_list as $role){
                if($i > 2)break;
                if(isset($role['user'])){                
                    $text = $text . $role['name'] . "是@". $role['user']['screen_name'] . ", ";
                }
                $i++;
            }

            $text = $text. " @{user1} @{user2} 你找到‘后宫’了么？ http://www.littlefun.net/apps/gong/sina/.";
            
            $gong = array(user_profile => $profile, role_list=>$role_list, text=>$text);
    	    $this->model('Gong');
    	    $cm = new GongModel();
	    
            $hot_level = 0;
            if($profile['verified']) $hot_level = 5;
            $cm->saveGongInfo($uid, $gong, $hot_level);
            
            $_image_info = $this->_generate_image_info($gong);
	    #print_r($_image_info);
            $json_text = json_encode($_image_info);
            print($json_text);
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
                  $friends = memcache_get($mmc, $oauth_id . '_friends_list');
                  $f_role_result = array_rand($friends, 2);
                  if($f_role_result){      
                     $text = str_replace('{user1}', $friends[$f_role_result[0]]['name'], $text);
                     $text = str_replace('{user2}', $friends[$f_role_result[1]]['name'], $text);
		  }
		  $gong_info['text'] = $text;
                  
                  $cache_key = $uid . '_gong_info';
                  memcache_set($mmc, $cache_key, $gong_info, 3 * 60);
                  memcache_delete($mmc, $uid . '_creating');
            }
            
            sae_debug("done $image_url");
            echo "done $image_url";
        }

        public function refresh_friend(){
            if(!isset($_SESSION['cur_user'])){
                header("location: /gong/index/");
                return;
            }

            $cur_id = $_SESSION['cur_user']['id'];

            $friends = $this->_get_friend_list($cur_id, 'all', $cur_id);            
            $this->assign('friendList', $friends);
	    $this->display('gong/friend_list.php');
        }

        public function more_gong(){
            $o = new SaeTOAuth( WB_AKEY , WB_SKEY  ); 
            if( $_SERVER['SERVER_PORT'] != 80 ) $port = ':'.$_SERVER['SERVER_PORT']; 

            $keys = $o->getRequestToken(); 
            $aurl = $o->getAuthorizeURL( $keys['oauth_token'] ,false , 'http://'. $_SERVER['HTTP_APPNAME'] . '.sinaapp.com' . $port . '/gong/callback/'); 
            $_SESSION['keys'] = $keys; 
             
            $this->assign('aurl',$aurl); 
	
            $gong_list = $this->_recent_gong(0, 200, $_GET['nocache'] == 'Y');
            $this->assign('gong_list', $gong_list);

	    $this->display('gong/gong_more_list.php');
        }

        public function _recent_gong($start, $limit=20, $no_cache=False){
            $mmc = memcache_init();
            $cache_key = 'gong_list_' . $start . '_' . $limit;
            if($mmc){
                $gong_list = memcache_get($mmc, $cache_key);
            }
            if(!$gong_list || $no_cache){
	        $this->model('Gong');
    	        $cm = new GongModel();
                $gong_list = $cm->getGongHistory($start, $limit);
	        memcache_set($mmc, $cache_key, $gong_list, 60);
            }
	    return $gong_list;
        }

       public function _recent_hot_gong($start, $limit=20, $no_cache=False){
            $mmc = memcache_init();
            $cache_key = 'gong_hot_list_' . $start . '_' . $limit;
            if($mmc){
                $gong_list = memcache_get($mmc, $cache_key);
            }
            if(!$gong_list || $no_cache){
	        $this->model('Gong');
    	        $cm = new GongModel();
                $gong_list = $cm->getGongHot($start, $limit, 1);
	        memcache_set($mmc, $cache_key, $gong_list, 60);
            }
	    return $gong_list;
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

            #更新Fans中的用户信息。
            $mmc = memcache_init();
            $cache_key = $uid . '_follower_list';
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
            $image['base_image'] = 'http://littlefun.sinaapp.com/gong/images/bg_03-1.jpg';
            
            $combine_text = array();
            $combine_image = array();
	    $xy_text = array(array(73, 63), array(232, 63), array(397, 63), array(73, 278),
                             array(232,243),
                             array(397, 278), array(73, 474), array(232, 474), array(397, 474),
			    );
	    $xy_img = array(array(68, 112), array(218, 112), array(378, 112), array(68, 334),
                             array(198, 278),
                             array(390, 334), array(68, 530), array(230, 530), array(390, 530),
			    );
            
            $col = 0;
            $count = 0;
            $row = -1;
            foreach($gong['role_list'] as $role){
                $col = $count % 3;
                if($col == 0)$row += 1;

                if(isset($role['user'])){

                    $text = $role['name'] . "\n". $role['user']['screen_name'];
                    array_push($combine_text, array('text'=> $text, 
                                          'x'=>$xy_text[$count][0], 
                                          'y'=>$xy_text[$count][1], 
                                          'color'=>'#FC444C'));

                    array_push($combine_image, array('image'=> $role['user']['profile_image_url'], 
                                          'x'=>$xy_img[$count][0], 
                                          'y'=>$xy_img[$count][1], 
                                          'color'=>'#FC444C'));
                }else {
                    $text = $role['name'];
                    array_push($combine_text, array('text'=> $text, 
                                          'x'=>$xy_text[$count][0], 
                                          'y'=>$xy_text[$count][1], 
                                          'color'=>'#FC444C'));
                    array_push($combine_image, array('image'=> 'http://tp2.sinaimg.cn/1929306733/50/0/1', 
                                          'x'=>$xy_img[$count][0], 
                                          'y'=>$xy_img[$count][1], 
                                          'color'=>'#FC444C'));
                }

                $count++;
                //跳过作者。
                if($count == 4)$count++;
            }

            $u = $gong['user_profile'];
            if($u['gender'] == 'm'){
                $text = "  朕\n" . $u['screen_name'];
            }else {
                $text = " 本宫\n" . $u['screen_name'];
            }	   

            array_push($combine_text, array('text'=> $text, 
                                  'x'=>$xy_text[4][0], 
                                  'y'=>$xy_text[4][1], 
                                  'color'=>'#FC444C'));
            array_push($combine_image, array('image'=> $u['profile_image_url'], 
                                  'x'=>$xy_img[4][0], 
                                  'y'=>$xy_img[4][1], 
                                  'color'=>'#FC444C'));

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
            if($s->errno()){
               var_dump($stor->errno(), $stor->errmsg());
            }

            return $fn;
        }

        public function get_gong($uid, $oauth_id){
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
                        $friends = memcache_get($mmc, $oauth_id . '_friends_list');
                        $f_role_result = array_rand($friends, 2);  
			$text = $gong['text'];
                        
                        $text = str_replace('{user1}', $friends[$f_role_result[0]]['name'], $text);
                        $text = str_replace('{user2}', $friends[$f_role_result[1]]['name'], $text);

		        $gong_info['text'] = $text;
		}else {
	                $gong_info = array();
	                $gong_info['image_url'] = "http://yun4demo-gong.stor.sinaapp.com/pic1929306733.jpg";
	                $gong_info['text'] = "";
                }
	        if($mmc){
	             memcache_set($mmc, $cache_key, $gong_info, 5 * 60);
	        }
            }
            return $gong_info;          
        }

        public function send_weibo_jsonp(){
            #没有登录，返回登录页面。
            $data = array();
            if(isset($_SESSION['cur_user'])){
                $cur_id = $_SESSION['cur_user']['id'];
                $mmc = memcache_init();
                $cache_key = $cur_id . '_oauth_token';
                if($mmc){
                    $oauth_token = memcache_get($mmc, $cache_key);
                }
	        if($oauth_token){
                    $c = new SaeTClient(WB_AKEY, WB_SKEY , $oauth_token['oauth_token'], 
                                   $oauth_token['oauth_token_secret']); 
                    $gong = $this->get_gong($_REQUEST['uid'], $cur_id);
                    if($gong['text']){
                       $result = $c->upload($gong['text'], $gong['image_url']);
                    }
                    $data['result'] = $result;
                }
            }else {
		$data['msg'] = '没有登录.';
            }

            $jsonData = json_encode($data);
            echo $_GET['callback'] . '(' . $jsonData . ');';
        }

        public function sendWeibo(){
            #没有登录，返回登录页面。
            if(!isset($_SESSION['cur_user'])){
                header("location: /gong/index/");
                return;
            }

            $cur_id = $_SESSION['cur_user']['id'];

            $mmc = memcache_init();
            $cache_key = $cur_id . '_oauth_token';
            if($mmc){
                $oauth_token = memcache_get($mmc, $cache_key);
            }
	    if(!$oauth_token) return array();

            $c = new SaeTClient(WB_AKEY, WB_SKEY , $oauth_token['oauth_token'], 
                                $oauth_token['oauth_token_secret']); 

	    $content = $_REQUEST['content'];
	    $imgurl = $_REQUEST['imgurl'];
           
            $result = $c->upload ( $content, $imgurl );
            echo json_encode ( $result );
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

        private function _get_friend_list($uid, $gender='all', $oauth_id){
            $mmc = memcache_init();
            $cache_key = $uid . '_friends_list';
            if($mmc){
                $friends = memcache_get($mmc, $cache_key);
            }
            if(!$friends){
                $friends = $this->_get_friend_list_from_remote($uid, $oauth_id);
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
                    'name'=>$p['name'],
                    'screen_name'=>$p['screen_name'],
                    'profile_image_url'=>$p['profile_image_url'],
                    'friends_count'=>$p['friends_count'],
                    'followers_count'=>$p['followers_count'],
                    'verified' => $me['verified'],
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

        private function _get_follower_list($uid, $gender='all', $oauth_id){
            $mmc = memcache_init();
            $cache_key = $uid . '_follower_list';
            if($mmc){
                $friends = memcache_get($mmc, $cache_key);
            }
            if(!$friends){
                $friends = $this->_get_follower_list_from_remote($uid, $oauth_id);
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
        
        /**
          $uid --- 取谁的Fans列表。
          $oauth_id --- 以谁的身份去取。
        */
        private function _get_follower_list_from_remote($uid, $oauth_id){
            $mmc = memcache_init();
            $cache_key = $oauth_id . '_oauth_token';
            if($mmc){
                $oauth_token = memcache_get($mmc, $cache_key);
            }
	    if(!$oauth_token) return array();

            $c = new SaeTClient(WB_AKEY, WB_SKEY , $oauth_token['oauth_token'], 
                                $oauth_token['oauth_token_secret']); 
	        $friends = $c->followers(-1, 100, $uid);
    	    $friends = $friends['users'];
            
            #用来判断用户是否有宫信息。
            $user_id_list = array();
            $ret_friends = array();
            foreach($friends as $p){
                array_push($ret_friends, array('id'=>$p['id'],
                    'gender'=>$p['gender'],
                    'name'=>$p['name'],
                    'screen_name'=>$p['screen_name'],
                    'profile_image_url'=>$p['profile_image_url'],
                    'friends_count'=>$p['friends_count'],
                    'followers_count'=>$p['followers_count'],
                    'verified' => $me['verified'],
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
