<?php
if(!defined('IN_Read8')) {
	exit('Access Denied (template_basic_jumpage.php)');
}
?>
<style type="text/css">
	#msgboard {
		filter: revealTrans(transition=23,duration=0.5) blendTrans(duration=0.5);
		position: absolute;
		left: 210px;
		top: 150px;
		width: 360px;
		height: 172px !important;
		height: 169px;
		z-index: 1;
		visibility: hidden;
		border: 1px solid #5296FF;
		font-size: 12px;
	}
	#msgboard div {
		margin: 1px;
	}
	#msgboard div.title {
		background: #8DACE9;
		color: #DEEEF3;
		font-weight: bold;
		text-align: center;
		line-height: 20px;
		border: 1px solid #5296FF;
	}
	#msgboard div.content {
		background: #EBF3E6;
		line-height: 30px;
		border: 1px solid #5296FF;
		height: 145px;
	}
	.content a:link,.content a:visited{
		color:#5296FF;
		text-decoration:none;
	}
	.content a:hover{
		color:#5296FF;
		text-decoration:underline;
	}
</style>
	<div id="msgboard">
	<noscript>
		<div class="title"><?php
echo $_obj['G_sitename'];
?>
欢迎您的光临！</div>
		<div class="content">
			<div style="padding-left: 20px;"><br /><?php
echo $_obj['title'];
?>
<br />自动转入来源页面</div>
			<div><ul><li><a href="<?php
echo $_obj['URL'];
?>
">如果您的浏览器不支持自动跳转或您不想等待请点击这里</a></li></ul></div>
		</div>
	</noscript>
		<div class="title"><?php
echo $_obj['G_sitename'];
?>
欢迎您的光临！</div>
		<div class="content">
			<div style="padding-left: 20px;"><br /><?php
echo $_obj['title'];
?>
<br />自动转入来源页面</div>
			<div><ul><li><a href="<?php
echo $_obj['URL'];
?>
">如果您的浏览器不支持自动跳转或您不想等待请点击这里</a></li></ul></div>
		</div>
	</div>
</div>
	<script type="text/javascript">
		var ie = document.all && !window.opera;
		function Show(divid){
			ie && document.getElementById(divid).filters.revealTrans.apply();
			document.getElementById(divid).style.visibility="visible";
			ie && document.getElementById(divid).filters.revealTrans.play();
		}
		function Hide(divid){
			ie && document.getElementById(divid).filters.revealTrans.apply();
			document.getElementById(divid).style.visibility="hidden";
			ie && document.getElementById(divid).filters.revealTrans.play();
		}
		Show('msgboard');
	</script>
<script type="text/javascript">setTimeout("Hide('msgboard')",2000);</script>
</body>
</html>
<meta http-equiv="refresh" content="3; url=<?php
echo $_obj['URL'];
?>
" />
