<?php
 $user = $_REQUEST['uid'];
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>测试结果浏览</title>
<link href="/apps/statics/gong/style.css" rel="stylesheet" type="text/css" />
<script src="/apps/statics/jquery.min.js" type="text/javascript"></script>
<script src="/apps/statics/jquery.jcarousellite.min.js" type="text/javascript"></script>
<script src="js/hougong.js" type="text/javascript"></script>
<script src="/apps/statics/jquery.tmpl.min.js" type="text/javascript"></script>
<script src="http://littlefun.sinaapp.com/js/gong_jsonp.js" type="text/javascript"></script>
<script >
   var user_id = '<?=$user ?>';
   var action='create_gong';
</script>
</head>
<body>
<script id="user_info" type="text/x-jquery-tmpl">
<li><a href="${id}.html"><img src="${profile_image_url}" width="75" height="75" /></a>
    <a href="http://weibo.com/${id}" style='color:#fff' target="_blank"><p>${name}</p></a>
</li>
</script>

<div class="header">
	<div class="logo"><a href="http://www.littlefun.net" target="_blank"><img src="/apps/statics/gong/logo_02.jpg" /></a>
</div>
 <?php include('header_scroll.php'); ?> 
  <div class="userinfo">
  	<a href="#"><img src="/apps/statics/gong/wb_login.gif" /></a>
  </div>
</div>
<div class="banner"><img src="/apps/statics/gong/main_03.jpg" width="975" height="75" /></div>
<div class="main">
	<div class="mainc" style="margin-left:0px;">
		<div class="mainc_send">
			<div class="box" style="padding:6px 0px 6px 30px; "><a id="create_my_gong" href="javascript:void(0);" class="btn_com" >我也去试试</a><a href="javascript:void(0);" class="btn_com" onclick="weibo_Submit()">发送到微博</a></div>
		</div>
		<div class="mainc_imgbox">
			<div class="box" style="background-image:url('/apps/statics/gong/img_03.jpg');height:646px;">                 
                <img id="test_Img" src="http://littlefun.sinaapp.com/images/loading66.gif" />
            </div>
		</div>
		<div class="mainc_textarea" id="send_textbox" style="display:none;">
			<div class="box">
				<textarea name="" id="test_Text">我通过#我的后宫#发现，@新蓝房产品地客竟然是我的皇后, 新蓝房产品地客是我的头号宠妃，新蓝房产品地客竟然是我的皇后, 新蓝房产品地客是我的头号宠妃，新蓝房产品地客竟然是我的皇后, 新蓝房产品地客是我的头号宠妃，新蓝房产品地客竟然是我的皇后。你生成后宫了吗？ http://www.pindic.com</textarea>
			</div>
		</div>
		<div class="we_send">
	<a href="javascript:void(0);" class="send" style="margin-left:0;" onclick="weibo_Submit()"></a>
</div>
	</div>
	<div class="de_ad"><img src="/apps/statics/gong/in_img.jpg" width="120" height="750" /></div>
	<div class="de_right">
		<div class="de_starlist">
			<div class="box">
				<h2 style="width:296px;">&nbsp;</h2>
				<div class="box_tit" style="width:290px;">
					<table width="100%" border="0" cellspacing="0" cellpadding="0">
					  <tr>
						<td width="44%">看看名人的后宫 </td>
						<td width="33%">&nbsp;</td>
						<td width="23%"><a href="http://littlefun.sinaapp.com/gong/more_gong/" class="a_link">更多&gt;&gt;</a></td>
					  </tr>
					</table>
				</div>
				<div class="de_hglist" id='hot_list'>
					<ul>
						<li><a href="#" target="_blank"><img src="/apps/statics/gong/in_img.jpg" width="75" height="75" /></a><p>用户名用户名</p></li>
						<li><a href="#" target="_blank"><img src="/apps/statics/gong/in_img.jpg" width="75" height="75" /></a><p>用户名用户名</p></li>
						<li><a href="#" target="_blank"><img src="/apps/statics/gong/in_img.jpg" width="75" height="75" /></a><p>用户名用户名</p></li>
						<li><a href="#" target="_blank"><img src="/apps/statics/gong/in_img.jpg" width="75" height="75" /></a><p>用户名用户名</p></li>
						<li><a href="#" target="_blank"><img src="/apps/statics/gong/in_img.jpg" width="75" height="75" /></a><p>用户名用户名</p></li>
						<li><a href="#" target="_blank"><img src="/apps/statics/gong/in_img.jpg" width="75" height="75" /></a><p>用户名用户名</p></li>
					</ul>
				</div>
			</div>
		</div>
		<div class="de_starlist" style="margin-top:12px;">
			<div class="box">
				<h2 style="width:296px;">&nbsp;</h2>
				<div class="box_tit" style="width:290px;">
					<table width="100%" border="0" cellspacing="0" cellpadding="0">
					  <tr>
						<td width="44%">最近生成后宫的人 </td>
						<td width="33%">&nbsp;</td>
						<td width="23%"><a href="http://littlefun.sinaapp.com/gong/more_gong/" class="a_link">更多&gt;&gt;</a></td>
					  </tr>
					</table>
				</div>
				<div class="de_hglist" id='recent_list'>
					<ul>
						<li><a href="#" target="_blank"><img src="/apps/statics/gong/in_img.jpg" width="75" height="75" /></a><p>用户名用户名</p></li>
						<li><a href="#" target="_blank"><img src="/apps/statics/gong/in_img.jpg" width="75" height="75" /></a><p>用户名用户名</p></li>
						<li><a href="#" target="_blank"><img src="/apps/statics/gong/in_img.jpg" width="75" height="75" /></a><p>用户名用户名</p></li>
						<li><a href="#" target="_blank"><img src="/apps/statics/gong/in_img.jpg" width="75" height="75" /></a><p>用户名用户名</p></li>
						<li><a href="#" target="_blank"><img src="/apps/statics/gong/in_img.jpg" width="75" height="75" /></a><p>用户名用户名</p></li>
						<li><a href="#" target="_blank"><img src="/apps/statics/gong/in_img.jpg" width="75" height="75" /></a><p>用户名用户名</p></li>
					</ul>
				</div>
			</div>
		</div>
	</div>
</div>
 <?php include('inc_footer.php'); ?> 
</body>
</html>
