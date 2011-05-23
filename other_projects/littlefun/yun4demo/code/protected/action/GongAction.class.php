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

            #$keys = $o->getRequestToken(); 
            #$aurl = $o->getAuthorizeURL( $keys['oauth_token'] ,false , 'http://'. $_SERVER['HTTP_APPNAME'] . '.sinaapp.com' . $port . '/cross/weibolist/'); 
            #$_SESSION['keys'] = $keys; 
             
            #$this->assign('aurl',$aurl); 
            
            $this->display('gong/index.php');
        }
         
    } 
?>
