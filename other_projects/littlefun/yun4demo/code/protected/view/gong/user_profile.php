<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="" xml:lang="" >
<head>
<title>宫--历史重现</title>
<meta http-equiv="content-type" content="text/html; charset=utf-8">
<meta http-equiv="imagetoolbar" content="no">
<link href="/gong/css/jquery-ui-themes.css" type="text/css" rel="stylesheet">


<link href="/css/chuanyue.css" rel="stylesheet" type="text/css" />
<script language="javascript" type="text/javascript" src="/js/jquery.min.js"></script>
<script language="javascript" type="text/javascript" src="/js/jquery.jcarousellite.min.js"></script>
<script type="text/javascript" src="/js/chuanyue.js"></script>

<style>
 *{ padding:0; margin:0 auto;}
body{ font-family:"宋体"; font-size:14px; padding:0; margin:0; repeat-y center 0;}
body a{outline:none;blr:expression(this.onFocus=this.blur());}
img{border:0;}
ul{ list-style:none; padding:0; margin:0;}
.container{ width:962px; overflow:hidden; clear:both; padding-bottom:10px; position:relative; z-index:1;}

</style>

</head>
<script type="text/javascript">
	var weibo_Name='<?=$me['name'] ?>';
	var platForm= 'sina';
	var nStart = <?=$startView ?>;
	var refresh_friend_url = '/gong/refresh_friend/';
	var send_weibo_url = '/gong/sendWeibo/';
</script>

<body>
<div class="container">
	<input type='hidden' id='weibo_Name' name='weibo_Name' value='<?=$me['name'] ?>' />
    <div id="header">
       <img style="float:left;" src="/gong/images/logo.png" />
	
	<div style="float:left;background:#fff;">
           <img src='<?=$me['profile_image_url'] ?>' />
	   <?=$me['screen_name'] ?>
       </div>
    </div>
    <div id="content">
        <table border="0">
            <tr>
            	<td style="width:200px;vertical-align: top;">
            	<br/><br/>
    	<div id="gong_intr">
    	    <form action='/gong/u/<?=$me['id'] ?>/' method='post'>
    	    	<input type='hidden' name='uid' value='<?=$me['id'] ?>' />
    	    	<input type='hidden' name='gender' value='<?=$me['gender'] ?>' />
    	    	<input type='hidden' name='create' value='Y' />    	    	    	    	
    		<input type='submit' name='submit' value='生成我的后宫' /> &nbsp;&nbsp; 
    	    </form>
    	    <?php if ($gong['loading']) { ?>
   		<textarea rows='3' cols='60' id='test_Text' ><?=$gong['text'] ?></textarea><br/>
   		<div id='u_loading'>
    		    <img id='test_Img' width='399px' height='403px'  src="/images/loading66.gif" />       	    
    		</div>
    		<script type='text/javascript'>
    		var user_id = '<?=$gong['uid'] ?>';
    		var retry_count = 0;
    		function load_img(){
    		    $.post("/gong/check_creating/", {uid: user_id}, 
		        function(data){
		           if(data['status'] == 'creating'){
			        retry_count++;
		           	if(retry_count < 5){
		           	   window.setTimeout(load_img, 2000);
		           	}else {
		           	    alert("连接超时。");
		           	}
		           }else if(data['status'] == 'done'){
		           	$('#test_Text').val(data['text']);
		           	$('#test_Img').attr('src', data['image_url'] + '?time=' + (new Date()).getTime());
		           }else {
		           	alert("error:" + data['status'] );
		           }
		        },
		        'json');
    		}
    		window.setTimeout(load_img, 2000);
    		</script>
    	    <?php }else { ?>
    		<textarea rows='3' cols='60' id='test_Text'><?=$gong['text'] ?></textarea><br/>
    		<img id='test_Img' width='399px' height='403px' src="<?=$gong['image_url'] ?>" />   
    	    <?php } ?>	
    	    
            <div class="mainbtna">
            		<a href="javascript:void(0)" id="send_Other" onclick="weibo_Submit()">
            			<img src="/images/sina_send_03.gif" alt="发送到微博" />
            		</a>
            </div>
    	    	    		
	</div>
            	    	   
            	</td>
            	<td style="vertical-align: top;">

	           <div style="border:1px solid #000;">
    	   
		        <div class="mainr" style='float:left;'>
		        	<h2>我的关注：</h2>
		            <div class="gz_scroll">
			    	   <?php include(DOCROOT . '/view/gong/friend_list.php'); ?>  
		            </div>
		            <div class="mainra"><a href="javascript:void(0);" class="more"><img src="/images/change_03.gif" /></a></div>
		        </div>
	       		            	   
	    	   <div>    	   
            	</td>
            	<td>
            	    <div class="mainc" >
	               <div class='messagewrap'>
	                    <a href="/gong/more_gong/" style='float:right;'>更多</a>
		            <h2>最近找到后宫：</h2> 
		            <div class="gong_list">
			    	   <?php include(DOCROOT . '/view/gong/recent_gong_list.php'); ?>  
		            </div>
		        </div>	
		   </div>	        
            	
            	</td>
            </tr>
                		
	</div>
	</table>
    </div>
    
</div>
</body>
</html>

