<?php 
    /** 
     * 我生活无聊，也很无奈，所以开始玩SAE, 玩Weibo,玩穿越. 差点忘了我自己叫四无浪子.
     */ 
     
//HelpAction.class.php 帮助文档的控制器 
    require_once "weibooauth.php";
    
    class CrossAction extends Action { 
         
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
            $aurl = $o->getAuthorizeURL( $keys['oauth_token'] ,false , 'http://'. $_SERVER['HTTP_APPNAME'] . '.sinaapp.com' . $port . '/cross/weibolist/'); 
            $_SESSION['keys'] = $keys; 
             
            $this->assign('aurl',$aurl); 
            
            $this->display('cross/index.php');         
        }
         
        public function callback(){ 
            $o = new SaeTOAuth( WB_AKEY , WB_SKEY , $_SESSION['keys']['oauth_token'] , $_SESSION['keys']['oauth_token_secret']  ); 
            $last_key = $o->getAccessToken(  $_REQUEST['oauth_verifier'] ) ; 
            $_SESSION['last_key'] = $last_key; 
            
            $c = new SaeTClient( WB_AKEY , WB_SKEY , $last_key['oauth_token'] , $last_key['oauth_token_secret']  ); 
            $me = $c->verify_credentials(); 
                                 
            if($last_key){ 
                echo "<a href='/cross/weibolist'>用新浪微博登陆成功，我的首页</a>" . $me['name']. '....'; 
                foreach($me as $e){
                	echo "xx:". $e . "<br/>";
                }
            }else{ 
                echo "登陆错误"; 
            } 
        } 
     
        public function weibolist(){ 
            $c = new SaeTClient( WB_AKEY , WB_SKEY , $_SESSION['last_key']['oauth_token'] , $_SESSION['last_key']['oauth_token_secret']  ); 
            // echo "ok...";
            
            $flag = "yunphp 的新浪开放接口的实现"; 
            //向微博发送数据 
            if( isset($_REQUEST['text']) || isset($_REQUEST['avatar']) ) 
            { 
                if( isset($_REQUEST['pic']) ) 
                    $rr = $c ->upload( $_REQUEST['text'] , $_REQUEST['pic'] ); 
                else 
                    $rr = $c->update( $_REQUEST['text'] );     
                $flag = '发送成功！'; 
            } 
            

            $ms  = $c->home_timeline();  
            $me = $c->verify_credentials(); 
            $this->assign('ms',$ms); 
            $this->assign('me',$me); 
            $this->assign('flag',$flag);
	    
	    $friends = $c->friends(-1, 20, $me['id']);
	    $friends = $friends['users'];
	    
	    $this->model('Cross');
	    $cm = new CrossModel();
	    $cm->deleteFriends($me['id']);
	    $cm->saveFriends($friends, $me['id']);
	    
            $this->assign('friends', $friends);       
             
            #echo "ok...";
            $this->display('cross/weibolist.php'); 
        } 
        
        public function do_cross($uid){
       	    $this->model('Cross');
       	    $cm = new CrossModel();
       	    #$cm->newCrossUser($uid);
       	    $info = $cm->getCrossByUid($uid);
       	    #$this->assign('friends', $friends);       
       	    #$this->display('cross/weibolist.php'); 
       	    echo "uid:". $uid . "<br/>";   	    
       	    echo "img:". $info['img'] . "<br/>";
       	    echo "time:". $info['time'] . "<br/>";
       	    echo "name:". $info['name'] . "<br/>";
       	    echo "event:". $info['event'] . "<br/>";       	           	           	    
        }

        public function help(){ 
            $this->display('weibo/help.php'); 
        } 
        public function source(){ 
            header("Content-type: text/html; charset=utf-8"); 
            highlight_file(DIR_ACTION.'WeiboAction.class.php'); 
        } 
         
    } 
?>
