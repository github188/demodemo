<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="" xml:lang="" >
<head>
<title>宫--历史重现</title>
<meta http-equiv="content-type" content="text/html; charset=utf-8">
<meta http-equiv="imagetoolbar" content="no">
<link href="/gong/css/jquery-ui-themes.css" type="text/css" rel="stylesheet">
<!--[if IE 6]>
<link href="Home_files/axurerp_pagespecificstyles_ie6.css" type="text/css" rel="stylesheet">
<![endif]-->
<script src="/gong/scripts/jquery-1.3.2.min.js"></script>
<script src="/gong/scripts/jquery-ui-1.8.10.custom.min.js"></script>
</head>
<body>
<div class="main_container">

    <div id="header">
       <img src="/gong/images/logo.png" />

       <img src="/gong/images/weibo_login.gif" />
    </div>
    <div id="content">
        <table>
            <tr>
            	<td style="width:200px;vertical-align: top;">
            	
        	<div id="ads_main">
    		<img src="/gong/images/u20_original.png" />
	</div>

    	<div id="gong_intr">
    	    <form action='/gong/u/<?=$me['id'] ?>/' method='post'>
    	    	<input type='hidden' name='uid' value='<?=$me['id'] ?>' />
    	    	<input type='hidden' name='gender' value='<?=$me['gender'] ?>' />
    	    	<input type='hidden' name='create' value='Y' />    	    	    	    	
    		<input type='submit' name='submit' value='生成我的后宫' /> &nbsp;&nbsp; 
    		<button name=''>发送测试结果到围脖</button> <br/>
    	    </form>
    	    <?php if ($gong['loading']) { ?>
   		<textarea rows='3' cols='80' id='gong_text'><?=$gong['text'] ?></textarea><br/>
   		<div id='u_loading'>
    		    <img id='gong_pic' src="/images/loading66.gif" />       	    
    		</div>
    		<script type='text/javascript'>
    		var user_id = '<?=$gong['uid'] ?>';
    		var retry_count = 0;
    		function load_img(){
    		    $.post("/gong/check_creating/", {uid: user_id}, 
		        function(data){
		           if(data['status'] == 'creating'){
		           	if(retry_count < 5){
		           	   setTimeout(load_img, 2000);
		           	   retry_count++;
		           	}else {
		           	    alert("连接超时。");
		           	}
		           }else if(data['status'] == 'done'){
		           	$('#gong_text').val(data['text']);
		           	$('#gong_pic').attr('src', data['image_url'] + '?time=' + (new Date()).getTime());
		           }else {
		           	alert("error:" + data['status'] );
		           }
		        },
		        'json');
    		}
    		setTimeout(load_img, 2000);
    		</script>
    	    <?php }else { ?>
    		<textarea rows='3' cols='80'><?=$gong['text'] ?></textarea><br/>
    		<img src="<?=$gong['image_url'] ?>" />   
    	    <?php } ?>		    		
	</div>
            	    	   
            	</td>
            	<td style="vertical-align: top;">
      		<h1><?=$me['screen_name'] ?>-</h1>
      		<img src='<?=$me['profile_image_url'] ?>' />
      		<br/><br/>
	        	<div style="border:1px solid #000;">
    	   
    	   <span style="font-family:Heiti SC;font-size:13px;font-weight:normal;font-style:normal;text-decoration:none;color:#333333;">我的好友</span>
    	   <div>    	   
    	   <?php include(DOCROOT . '/view/gong/friend_list.php'); ?>    	     
    	     
    	   </div>
            	<div>
            	
            	</td>
            </tr>
                		
	</div>
	</table>
    </div>
    
</div>
</body>
</html>

