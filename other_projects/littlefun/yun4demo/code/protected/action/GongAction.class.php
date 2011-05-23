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
            $_SESSION['last_key'] = $last_key;                  
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
            #echo $me['id'];
            header("location: /gong/u/". $me['id']);
            exit();
        }
        
        public function u($uid){
            $this->assign('me', $_SESSION['cur_user']); 
            $this->display('gong/user_profile.php');
        }  

        public function create_gong($uid){
            $mmc = memcache_init();
            $cache_key = $uid . '_profile';
            if($mmc){
                $profile = memcache_get($mmc, $cache_key);
            }
            //用户还没有登录，在系统找不到用户信息。
            if(!$profile){
                #return false;
                $profile = $_SESSION['cur_user'];
                $uid = $profile['id'];
            }
            $role_list = $this->_get_user_role_list($uid, $profile['gender']);
            $m_friends = $this->_get_friend_list($uid, 'm');
            $f_friends = $this->_get_friend_list($uid, 'f');           
            
            print("=====================><br/>");
            print_r($role_list);
            
            $role_list = $this->_full_roles_with_friends($role_list, $m_friends, $f_friends);
            print("-------------------------><br/>");      
            
            foreach($role_list as $role){
            	print_r($role['gender'] . '--->' . $role['name'] . " --> ". $role['user']['screen_name'] . "--->" . $role['user']['gender'] . "<br/>");
            }   
            
            print_r($role_list);
            
            $gong = array(user_profile => $profile, role_list=>$role_list); 
    	    $this->model('Gong');
    	    $cm = new GongModel();
            $cm->saveGongInfo($uid, $gong);
            exit();            
            return $gong;            
        }

        public function get_gong($uid){
    	    $this->model('Gong');
    	    $cm = new GongModel();
            $gong = $cm->getGongInfo($uid);
            
            print_r($gong);
            exit();
            return $gong;          
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

        private function _get_friend_list_from_remote($uid){
            $c = new SaeTClient(WB_AKEY, WB_SKEY , $_SESSION['last_key']['oauth_token'], 
                                $_SESSION['last_key']['oauth_token_secret']); 
	        $friends = $c->friends(-1, 100, $uid);
    	    $friends = $friends['users'];
            $ret_friends = array();
            foreach($friends as $p){
                array_push($ret_friends, array('id'=>$p['id'],
                    'gender'=>$p['gender'],
                    'screen_name'=>$p['screen_name'],
                    'profile_image_url'=>$p['profile_image_url'],
                    'friends_count'=>$p['friends_count'],
                    'followers_count'=>$p['followers_count'],
                    ));
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
