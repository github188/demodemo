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
<script src="/gong/scripts/jquery-1.4.2.min.js"></script>
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
    		<button name=''>生成我的后宫</button> &nbsp;&nbsp; 
    		<button name=''>发送测试结果到围脖</button> <br/>
    		<img src="http://yun4demo-gong.stor.sinaapp.com/pic1929306733.jpg" />    		    		
	</div>
            	    	   
            	</td>
            	<td style="vertical-align: top;">
      		<h1><?=$me['screen_name'] ?>-</h1>
      		<img src='<?=$me['profile_image_url'] ?>' />
      		<br/><br/>
	        	<div style="border:1px solid #000;">
    	   
    	   <span style="font-family:Heiti SC;font-size:13px;font-weight:normal;font-style:normal;text-decoration:none;color:#333333;">看看名人的后宫</span>
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

