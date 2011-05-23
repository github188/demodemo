<?php defined('YUNPHP') or exit('can not access!');

	/**
	 * YunPHP4SAE php framework designed for SAE
	 *
	 * @author heyue <heyue@foxmail.com>
	 * @copyright Copyright(C)2010, heyue
	 * @link http://code.google.com/p/yunphp4sae/
	 * @license http://www.opensource.org/licenses/bsd-license.php New BSD License
	 * @version YunPHP4SAE version 1.0.2
	 */
/**
 * 没有实例化的加载类
 *
 * @param unknown_type $class
 * @return unknown
 */
function import_class($class){
	if(file_exists(DOCROOT.'lib/'.$class.'.class.php')){
		require_once(DOCROOT.'lib/'.$class.'.class.php');
		return true;
	}else if(file_exists(YUNPHP.'lib/'.$class.'.class.php')){
		require_once(YUNPHP.'lib/'.$class.'.class.php');
		return true;
	}else{
		return false;
	}
}

/**
 * 重定义exception的函数
 * 分两种模板来处理
 *
 */
function my_exception_handler($e){
        if (DEBUG == TRUE) {
        	$file = $e->getFile();
        	$line = $e->getLine();
        	$message = "<b>YunPHP error:   </b>".$e->getMessage();
        	
        	Log::write_log('ERROR',"$message  $file $line ");
        	ob_start();
        	include_once DOCROOT.'errors/debug_error.php';
        	$buffer = ob_get_contents();
        	ob_end_clean();
        	echo $buffer;
        } else {
        	Log::write_log('ERROR',"$message  $file $line ");
        	$message = $e->getMessage();
        	$error_type = trim(array_shift(explode(" ",$message)));
        	ob_start();
            switch ($error_type) {
        		case 404:
        			include_once DOCROOT.'errors/show_404.php';
        		break;
        		default:
        			include_once DOCROOT.'errors/show_error.php';
        		break;
        	}
        	$buffer = ob_get_contents();
        	ob_end_clean();
        	echo $buffer;
        }
}
?>