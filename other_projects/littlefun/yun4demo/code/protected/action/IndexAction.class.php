<?php

	class IndexAction extends Action{
		public function __construct(){
			parent::__construct();
		}
		
		public function index(){
			$welcome = 'Welcome to YunPHP';
			$school = array('北京师范大学','北京大学','清华大学','人民大学');
			$this->assign('welcome',$welcome);
			$this->assign('school',$school);
			$this->display('index.php');
		}

     }

?>
