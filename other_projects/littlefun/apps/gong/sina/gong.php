<?php
 $user = $_REQUEST['uid'];
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>我的后宫</title>
<link href="/apps/statics/gong/style.css" rel="stylesheet" type="text/css" />
<script src="/apps/statics/jquery.min.js" type="text/javascript"></script>
<script src="/apps/statics/jquery.jcarousellite.min.js" type="text/javascript"></script>
<script src="/apps/statics/jquery.tmpl.min.js" type="text/javascript"></script>
<script src="http://littlefun.sinaapp.com/js/gong_jsonp.js" type="text/javascript"></script>
<script >
   var user_id = '<?=$user ?>';
   var action='';
</script>
</head>
<body>
<script id="user_info" type="text/x-jquery-tmpl">
  <table width="100%" border="0" cellspacing="0" cellpadding="0"> 
    <tr> 
    <td width="32%" rowspan="2">
    <a href="#" target="_blank">
    <img src="${profile_image_url}" /></a>
    </td>
    <td width="68%" height="20">
        <span class="color_fff"><strong>${name}</strong></span>
    </td> 
    </tr>
    <tr>
    <td>
{{if have_gong }}
<a href="${id}.html"  class="test" onclick="test_huogong()">看他后宫</a>
{{else}}
<a href="http://littlefun.sinaapp.com/gong/out_create_gong/${id}/?gender=${gender}"  class="test" onclick="test_huogong()">帮他生成后宫</a>
{{/if}}
</td> 
    </tr>
  </table>
</script>
<div class="header">
	<div class="logo"><a href="http://www.littlefun.net" target="_blank"><img src="/apps/statics/gong/logo_02.jpg" /></a>
</div>
 <?php include('header_scroll.php'); ?> 
	<div class="userinfo">
		<table width="70%" border="0" cellspacing="0" cellpadding="0" style="float:right;">
		  <tr>
			<td rowspan="2" width="50"><img id='meImg' src="/apps/statics/gong/main_03.jpg" width="40" height="40" /></td>
			<td><span class="color_fff"><strong><span id='meName'>XXX用户名</span></strong></span></td>
		  </tr>
		  <tr>
			<td><a href="jarvascript:void(0)" class="exit" style="float:left">退出</a></td>
		  </tr>
		</table>

	</div>
</div>
<div class="banner"><img src="/apps/statics/gong/main_03.jpg" width="960" height="60" /></div>
<div class="main">
	<div class="mainl">
		<div class="box">
			<h2>&nbsp;</h2>
			<div class="box_tit">
					<table width="100%" border="0" cellspacing="0" cellpadding="0">
					  <tr>
						<td width="44%">我的粉丝 </td>
						<td width="10%"><input type="checkbox" name="fans" value=""/></td>
						<td width="46%">只显示有后宫的</td>
					  </tr>
					</table>
				</div>
			<div class="fans_list">
				<div class="fans_scroll" id="fans_scroll">
					<ul>
						<li>
							
						</li>
					</ul>
				</div>
				<div class="change">
					<a class="more" id="gzmore" href="javascript:void(0);"><strong>换一换</strong></a>				
                </div>
			</div>
		</div>
	</div>
	<div class="mainc">
		<div class="mainc_send">
			<div class="box" style="padding:6px 0px 6px 30px; "><input name="" type="hidden" value="" id="myselft" /><a href="javascript:void(0);" class="schg" onclick="test_huogong()"></a><a href="javascript:void(0);" class="send" onclick="weibo_Submit()"></a></div>
		</div>
		<div class="mainc_imgbox">
			<div class="box" id="testimg"><img id='gongImg' src="/apps/statics/gong/img_03.jpg" /></div>
		</div>
		<div class="mainc_textarea" id="send_textbox" style="display:none;">
			<div class="box">
				<textarea name="" class="textarea" id="text_area">我通过#我的后宫#发现，@新蓝房产品地客竟然是我的皇后, 新蓝房产品地客是我的头号宠妃，新蓝房产品地客竟然是我的皇后, 新蓝房产品地客是我的头号宠妃，新蓝房产品地客竟然是我的皇后, 新蓝房产品地客是我的头号宠妃，新蓝房产品地客竟然是我的皇后。你生成后宫了吗？ http://www.pindic.com</textarea>
			</div>
		</div>
	</div>
	<div class="mainr">
		<div class="box">
			<h2>&nbsp;</h2>
			<div class="box_tit">
					<table width="100%" border="0" cellspacing="0" cellpadding="0">
					  <tr>
						<td width="44%">我的关注</td>
						<td width="10%"><input type="checkbox" id='have_gong_friends' name="fans" value=""/></td>
						<td width="46%">只显示有后宫的</td>
					  </tr>
					</table>
				</div>
			<div class="fans_list">
				<div class="fans_scroll" id="myfriends">
					<ul>
						<li>
							
						</li>
					</ul>
				</div>
				<div class="change">
					<a class="more" id="friends_smore" href="javascript:void(0);"><strong>换一换</strong></a>				</div>
			</div>
		</div>
	</div>
	<div id="tishi" style="display:none;">
	<div class="tishi_info"><a href="javascript:void(0)" onclick="tishi_close()"><strong>x</strong></a></div>
	<div class="tishi_content" id="ts_con"></div>
</div>
</div>
<div class="we_send">
	<a href="javascript:void(0);" class="send" style="margin-left:0;"  onclick="weibo_Submit()"></a>
</div>
<div class="footer">
    Copyright © 2009 <a href='http://www.littlefun.net/'>LittleFun.net</a>| 浙ICP备11025245号
    
    <a href="http://sae.sina.com.cn" target="_blank"> 
    	<img src="http://static.sae.sina.com.cn/image/poweredby/117X12px.gif" title="Powered by Sina App Engine" /> 
    </a> 
</div>


</body>
</html>

