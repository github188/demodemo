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
            $aurl = $o->getAuthorizeURL( $keys['oauth_token'] ,false , 'http://'. $_SERVER['HTTP_APPNAME'] . '.sinaapp.com' . $port . '/gong/u/'); 
            $_SESSION['keys'] = $keys; 
             
            $this->assign('aurl',$aurl); 
            
            $this->display('gong/index.php');
        }
        
        public function u($uid){
            $o = new SaeTOAuth( WB_AKEY , WB_SKEY  ); 
            if( $_SERVER['SERVER_PORT'] != 80 ) $port = ':'.$_SERVER['SERVER_PORT']; 

            #$keys = $o->getRequestToken(); 
            #$aurl = $o->getAuthorizeURL( $keys['oauth_token'] ,false , 'http://'. $_SERVER['HTTP_APPNAME'] . '.sinaapp.com' . $port . '/cross/weibolist/'); 
            #$_SESSION['keys'] = $keys; 
             
            #$this->assign('aurl',$aurl); 
            
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
                return false;
            }
            $role_list = $this->_get_user_role_list($uid, $profile['gender']);
            $m_friends = this->_get_friend_list($uid, 'm');
            $f_friends = this->_get_friend_list($uid, 'f');
            $this->_full_roles_with_friends($role_list, $m_friends, $f_friends);
            $gong = array(user_profile => $profile, role_list=>$role_list);            
            
            return $friends;            
        }
        
        private function _full_roles_with_friends($role_list, $m_friends, $f_friends){
            $f_role = array_filter($role_list, 
                function ($e){ return ($e['gender'] == 'f'); } 
            );
            $m_role = array_filter($role_list, 
                function ($e){ return ($e['gender'] == 'm'); } 
            );
            $f_role_result = array_rand($f_friends, count($f_role));
            $m_role_result = array_rand($m_friends, count($m_role));            
            
            for($i = 0; $i < count($f_role); $i++){
                 $f_role['user'] = $f_role_result[$i];
            }
            for($i = 0; $i < count($m_role); $i++){
                 $m_role['user'] = $m_role_result[$i];
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

            return $friends;
        }

        private function _get_friend_list_from_remote($uid){
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
            )
            return $app_config[$gender];
        }
             
    } 
?>
