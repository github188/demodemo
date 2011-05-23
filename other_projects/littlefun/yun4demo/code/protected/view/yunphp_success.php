<!DOCTYPE html PUBLIC '-//W3C//DTD XHTML 1.0 Transitional//EN' 'http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd'>
<html>
<head>
<title>页面提示</title>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<meta http-equiv='Refresh' content="<?php echo $wait_time ?>;URL=<?php echo $jump_url; ?>">
<style type="text/css">
div.message{width:500px; height:150px; left:50%; margin:-75px 0 0 -250px; position:absolute; top:50%; text-align:center; border:1px solid #d1d7dc}
</style>
</head>
<body>
<div class="message">
	<h1 style="color: red; background: #f0f0f0; font-size: 18px;"><?php echo $msg ?></h1>
	<div>
		<?php if($jump_url == ''){ ?>
			系统将在 <span style="color:blue;font-weight:bold"><?php echo $wait_time ?></span> 秒后自动跳转,如果不想等待,直接点击 <a href="#" onclick="history.back(-1)">这里</a> 跳转			
		<?}else{ ?>
		系统将在 <span style="color:blue;font-weight:bold"><?php echo $wait_time ?></span> 秒后自动跳转,如果不想等待,直接点击 <a href="<?php echo $jump_url ?>">这里</a> 跳转
		<?} ?>
		<div style="margin-top:10px;"><img src="/images/loader.gif"/></div>
	</div>
</div>
</body>
</html>
