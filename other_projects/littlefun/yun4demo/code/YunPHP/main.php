<?php defined('YUNPHP') or exit('can not access!');

	$uri = $_SERVER['REQUEST_URI'];
	
	include_once (YUNPHP.'core/YunPHP.class.php');
	include_once (YUNPHP.'core/Action.class.php');
	include_once (YUNPHP.'core/Module.class.php');
	include_once (YUNPHP.'core/common.php');
	
	require_once(YUNPHP.'lib/Configure.class.php');
	require_once(YUNPHP.'lib/Log.class.php');
	require_once(YUNPHP.'lib/Router.class.php');
	
	define('DIR_ACTION',DOCROOT.'action/');
	define('DIR_MODULE',DOCROOT.'module/');
	define('DIR_VIEW',DOCROOT.'view/');
	
	set_exception_handler('my_exception_handler');
	
	Configure::load('config');
	
	$RTR = new Router($uri);
	
	
	$uri_argvs = $RTR->getUriArgvs();
	$class  = $RTR->getClass();
	$method = $RTR->getMethod();
	$action = ucfirst($class);
	$action = $action.'Action';
	
	include_once (DIR_ACTION."$action.class.php");

	if(!class_exists($action)){
		throw new Exception("404 $action not exist!");
	}
	
	$app = new $action();
	call_user_func_array(array($app,$method),array_slice($uri_argvs,2));
	
//this is the end of the main.php