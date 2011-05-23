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

	class Action extends YunPHP{
		private static $_instance;
		public  $view_vars = array();
		
		public function __construct(){
			parent::__construct();
		}
 		/**
 		 * 赋值变量到模板
 		 *
 		 * @param unknown_type $key
 		 * @param unknown_type $val
 		 */
		public function assign($key,$val){
			$this->view_vars[$key] = $val;
		}
		/**
		 * 页面显示
		 *
		 * @param unknown_type $path
		 */
		public function display($path){
			extract($this->view_vars);
			include (DOCROOT.'view/'.$path);
			
		}
		/**
		 * 加载model
		 *
		 * @param unknown_type $model
		 */
		public function model($model){
			$model = ucfirst($model);
			if(file_exists(DOCROOT.'module/'.$model.'Model.class.php')){
				require_once DOCROOT.'module/'.$model.'Model.class.php';
			}else{
				throw new Exception("Action ==> $model model not exists");
			}
		}
		/**
		 * 成功的简单返回
		 */
		public function success($msg,$jump_url='',$wait_time=1){
			if($jump_url == ''){
				$jump_url = $_SERVER['HTTP_REFERER'];
			}
			$this->assign('msg',$msg);
			$this->assign('wait_time',$wait_time);
			$this->assign('jump_url',$jump_url);
			$this->display('yunphp_success.php');
			exit();
		}
		
		public function error($msg,$jump_url='',$wait_time=2){
			if($jump_url == ''){
				$jump_url = $_SERVER['HTTP_REFERER'];
			}
			$this->assign('msg',$msg);
			$this->assign('wait_time',$wait_time);
			$this->assign('jump_url',$jump_url);
			$this->display('yunphp_error.php');
			exit();
		}
		/**
		 * 获取当前实例
		 *
		 * @return unknown
		 */
		public function getInstance(){
			if(self::$_instance == NULL){
				self::$_instance = new Action();
			}
			return self::$_instance;
		}
		/**
		 * 默认的__call函数的调用，如果上层没有重写，这里将默认出现一个404的错误提示
		 */
		public function __call($name,$arguments){
			throw new Exception("404 $name function not exist!");
		}
			
	}
?>