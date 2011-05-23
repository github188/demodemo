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

		public function show($type) {
			if($type == 'success'){
				$this->success("恭喜您，登录成功！");
			}else if($type == 'error'){
				$this->error("登录失败");
			}else{
				$this->error("你的访问有误");
			}
		}
	}

?>