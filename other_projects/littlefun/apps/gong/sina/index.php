<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>我的后宫主页</title>
<link href="/apps/statics/gong/style.css" rel="stylesheet" type="text/css" />
<script src="/apps/statics/jquery.min.js" type="text/javascript"></script>
<script src="/apps/statics/jquery.jcarousellite.min.js" type="text/javascript"></script>
<script src="/apps/statics/jquery.tmpl.min.js" type="text/javascript"></script>
<script src="http://littlefun.sinaapp.com/js/gong_jsonp.js" type="text/javascript"></script>
<!--
<script src="/apps/statics/gong/hougong.js" type="text/javascript"></script>
-->
</head>
<script > 
   var user_id = '';
   var action='index';
</script>

<body>
<div class="header">
	<div class="logo"><a href="http://www.littlefun.net" target="_blank"><img src="/apps/statics/gong/logo_02.jpg" /></a></div>
 <?php include('header_scroll.php'); ?> 
  <div class="userinfo">
  	<a href="http://littlefun.sinaapp.com/gong/sina_ouath/"><img src="/apps/statics/gong/wb_login.gif" /></a>
  </div>
</div>
<div class="main">
	<div class="mainl">
		<div class="box">
			<h2>&nbsp;</h2>
			<div class="box_tit">
					<table width="100%" border="0" cellspacing="0" cellpadding="0">
					  <tr>
						<td width="44%">看看名人的后宫 </td>
						<td width="33%">&nbsp;</td>
						<td width="23%"><a href="#" class="a_link">更多&gt;&gt;</a></td>
					  </tr>
					</table>
				</div>
<script>
$(document).ready(function(){
    $("#user_info").template( "user_info" );
    jQuery.getJSON("http://littlefun.sinaapp.com/gong/last_hot_jsonp/?limit=12&callback=?",
    function(data){
        $("#hot_gong_list table").remove();
        $.tmpl("user_info", data).appendTo('#hot_gong_list');
    },
    'json');
});
</script>
<script id="user_info" type="text/x-jquery-tmpl">
    <table width="100%" border="0" cellspacing="0" cellpadding="0">
	    <tr>
	       <td width="32%" rowspan="2">
             <a href="http://weibo.com/${id}" target="_blank">
                <img src="${profile_image_url}" />
             </a>
           </td>
	       <td width="68%" height="20">&nbsp;&nbsp;<span class="color_fff"><strong>${name}</strong></span>
</td>
	    </tr>
	    <tr>
	       <td><a href="http://littlefun.sinaapp.com/gong/sina_ouath/"  class="test" onclick="test_huogong()">看他后宫</a></td>
	    </tr>
    </table>
</script>
			<div id="hot_gong_list" class="fans_list">
				
        <table width="100%" border="0" cellspacing="0" cellpadding="0">
							  <tr>
								<td width="32%" rowspan="2"><a href="#" target="_blank"><img src="/apps/statics/gong/main_03.jpg" wid /></a></td>
								<td width="68%" height="20"><span class="color_fff"><strong>用户名用户名用户名</strong></span></td>
							  </tr>
							  <tr>
								<td><a href="javascript:void(0);"  class="test">看他后宫</a></td>
							  </tr>
							</table>
			</div>
		</div>
	</div>
	<div class="in_mainr">
		<div class="in_banner">
			<a href="#" target="_blank"><img src="/apps/statics/gong/main_03.jpg" width="734" height="90" /></a>
		</div>
		<div class="in_img">
			<div class="box"><img src="/apps/statics/gong/in_img.jpg" /></div>
		</div>
		<div class="in_login">
			<a href="http://littlefun.sinaapp.com/gong/sina_ouath/"><img src="/apps/statics/gong/wb_login.gif" /></a>
		</div>
	</div>
</div>
 <?php include('inc_footer.php'); ?> 
</body>
</html>
