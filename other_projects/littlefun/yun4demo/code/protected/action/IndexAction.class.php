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

        public function image_form(){
           echo <<<END
<html><body>
<h4>Json image</h4>
<form action="http://www.pindic.com/gen_img.php" method="POST" >
 Data:<textarea name='image_data' rows='10' cols='80'>
{"base_image":"http:\/\/yun4demo.sinaapp.com\/images\/cybg_03.png",
"combine_text":[
{"text":"\u592a\u76d1\u603b\u7ba1-->\u91d1\u709c-\u54c1\u5730\u5ba2","x":20,"y":300,"color":"#123456"},
{"text":"\u5934\u53f7\u5ba0\u5983-->\u4e11\u4e11\u599e632","x":20,"y":300,"color":"#123456"},
{"text":"\u5e26\u5200\u4f8d\u536b-->\u5fae\u7b11\u4e0d\u6c42\u751a\u89e3","x":20,"y":300,"color":"#123456"},
{"text":"\u592a\u5085-->pegasus_zy","x":20,"y":300,"color":"#123456"},
{"text":"\u592a\u4e0a\u7687-->\u795e\u7ecf\u7684\u795e1234","x":20,"y":300,"color":"#123456"},
{"text":"\u8d35\u5983-->\u9759\u5b50\u8d75","x":20,"y":300,"color":"#123456"},
{"text":"\u5fa1\u533b-->\u6211\u7231\u4f59\u679c","x":20,"y":300,"color":"#123456"},
{"text":"\u5fa1\u53a8-->crisye","x":20,"y":300,"color":"#123456"}],
"combine_image":[
{"image":"http:\/\/tp4.sinaimg.cn\/1761180383\/50\/5597040227\/1","x":20,"y":300,"color":"#123456"},
{"image":"http:\/\/tp4.sinaimg.cn\/1890166503\/50\/1292128384\/0","x":20,"y":300,"color":"#123456"},
{"image":"http:\/\/tp3.sinaimg.cn\/1902672142\/50\/1298341813\/1","x":20,"y":300,"color":"#123456"},
{"image":"http:\/\/tp3.sinaimg.cn\/1902314142\/50\/1293257917\/1","x":20,"y":300,"color":"#123456"},
{"image":"http:\/\/tp2.sinaimg.cn\/1892056797\/50\/1300283641\/1","x":20,"y":300,"color":"#123456"},
{"image":"http:\/\/tp2.sinaimg.cn\/1709061893\/50\/5596899497\/0","x":20,"y":300,"color":"#123456"},
{"image":"http:\/\/tp4.sinaimg.cn\/1750272871\/50\/1278050960\/1","x":20,"y":300,"color":"#123456"},
{"image":"http:\/\/tp1.sinaimg.cn\/1370601600\/50\/5600965665\/1","x":20,"y":300,"color":"#123456"}]}
 </textarea> <br/>
   <input type='submit' name='submit' value='submit' />        
</form>
</body></htm>
END;
        }

        public function image_x2(){
            $bgImage = SITE_ROOT . "images/cybg_03.jpg";                         
            $bgData = file_get_contents($bgImage);
            header('Content-Type: image/jpeg');
            echo $bgData;
	    exit();
	}
        public function json_image(){
            $data = $_REQUEST['image_data'];
            $uid = $_REQUEST['uid'];
            
            $image_fmt = json_decode($data, True);
            
            $name_list = array();
            foreach($image_fmt['combine_text'] as $t){
                array_push($name_list, $t['text']);                
            }
            $text = join("\n", $name_list);

            $bgImage = SITE_ROOT . "images/cybg_03.jpg";                         
            $bgData = file_get_contents($bgImage);
            
            $img = new SaeImage($bgData);
            $img->annotate("\n\n\n". $text, 1, SAE_North,
                array("name"=>SAE_MicroHei, "size"=>22, "weight"=>300, "color"=>"#211c03"));

            $outData = $img->exec('jpg', false);
    	    if ($outData === false){
                var_dump($img->errno(), $img->errmsg());
            }else {
                #header('Content-Type: image/jpeg');
                $s = new SaeStorage();
                $fn = $s->write("gong", "pic" . $uid . ".jpg", $outData);
                echo $fn;
            }
         }
     }

?>
