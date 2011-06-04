<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="" xml:lang="" >
<head>
<title>宫--历史重现</title>
<meta http-equiv="content-type" content="text/html; charset=utf-8">
<meta http-equiv="imagetoolbar" content="no">
<link href="/gong/css/jquery-ui-themes.css" type="text/css" rel="stylesheet">
<script src="/gong/scripts/jquery-1.4.2.min.js"></script>
<script src="/gong/scripts/jquery-ui-1.8.10.custom.min.js"></script>
</head>
<body>
<style>
 *{ padding:0; margin:0 auto;}
body{ font-family:"宋体"; font-size:14px; padding:0; margin:0; repeat-y center 0;}
body a{outline:none;blr:expression(this.onFocus=this.blur());}
img{border:0;}
ul{ list-style:none; padding:0; margin:0;}
.container{ width:962px; overflow:hidden; clear:both; padding-bottom:10px; position:relative; z-index:1;}

</style>
<div class="container">

    <div id="header">
       <img src="/gong/images/logo.png" />
       <span style="font-size:20px;">查看你的后宫都是谁？</span>
       <a href="<?=$aurl?>"><img src="/gong/images/weibo_login.gif" /></a>
    </div>
    <div id="content">
        <hr /><br/>
    	<h1>最近找到“后宫”的人</h1>
       <?php
           foreach($gong_list as $gong) {
              $su = $gong['user_profile'];
              $gong['text'] = str_replace("@{user1} @{user2} 你找到‘后宫’了么？ http://littlefun.sinaapp.com/gong/u/.", '', $gong['text']);
              $gong['text'] = str_replace("@{user1} @{user2} 你找到‘后宫’了么？ http://www.littlefun.net/apps/gong/sina/.", '', $gong['text']);
       ?>
        <table width="100%" style='margin-top:10px;' border="0" cellspacing="0" cellpadding="0">
             <tr>
                <td width="70" rowspan="2">
                   <a href="http://weibo.com/<?=$su["id"] ?>" target="_blank">
                     <img src="<?=$su["profile_image_url"] ?>" width="60" height="60" class="imgbox" />
                   </a>
                </td>
                <td height="35" valign="top" style="color: #666;">                                
                    <a href="http://weibo.com/<?=$su["id"] ?>" target="_blank" style="color: #666;text-decoration: none;"><?=$su["screen_name"] ?></a>
	            <br/><br/>
	            <?=$gong['text'] ?>
                </td>
             </tr>                           
        </table>        
      <?php } ?>    	
    	 
    </div>

    <div id='footer'>
    	<hr /><br/>
    	<div style='text-align:center;'>
    Copyright © 2009 <a href='http://www.littlefun.net/'>LittleFun.net</a>| 浙ICP备11025245号
    
    <a href="http://sae.sina.com.cn" target="_blank">
    	<img src="http://static.sae.sina.com.cn/image/poweredby/117X12px.gif" title="Powered by Sina App Engine" />
    </a>
    </div>    
    
</div>
</body>
</html>

