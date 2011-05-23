<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"> 
<html xmlns="http://www.w3.org/1999/xhtml"> 
<head> 
 <meta http-equiv="content-type" content="text/html; charset=utf-8" /> 
 <link rel='stylesheet' type='text/css' media='all' href="http://yunphp.sinaapp.com/css/help.css"/>
 <title>YunPHP 新浪云计算SAE专用定制框架</title>
 </head> 
 <body>
 <?php echo $welcome;?><br/>
 <a href="http://yunphp.sinaapp.com">官网</a>
 <ul>
	<?php foreach($school as $val){ ?>
	<li><?php echo $val;?></li>
	<?php }?>
	<b>简单的错误返回</b><br/>
	<a href="/index/show/success" >成功</a><br/>
	<a href="/index/show/error" >失败</a><br/>
 </ul>
<p>Running On <a href="http://sae.sina.com.cn" target="_blank"><img src="http://sae.sina.com.cn/static/image/poweredby/117X12px.gif" title="Powered by Sina App Engine" /></a></p>
 </body>
 </html>
