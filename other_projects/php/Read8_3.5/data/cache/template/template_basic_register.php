<?php
if(!defined('IN_Read8')) {
	exit('Access Denied (template_basic_register.php)');
}
?>
	<script>
		var initd = 0;
		function secode_reload(){
			$('secode').src = 'download.php?action=gethashimg&amp;xx='+Math.random();
		}
		function secode_init(){
			if(initd) {
				return;
			}
			$('secode_info').style.display = 'none';
			$('secode').style.display = '';
			secode_reload();
		}
	</script>
	
	<div class="rd8_fullborder">
<?php
if(!empty($_obj['eula'])) {
?>
	<form action="" method="post">
		<div class="rd8_head">服务条款和声明</div>
		<div class="rd8_col0" style="width: 99%;padding-left: 1%;"><?php
echo $_obj['eula'];
?>
</div>
		<div class="rd8_colt" style="text-align: center;"><input type="submit" value=" 我接受 " /> &nbsp;<input type="hidden" name="eula" value="1" /><?php
if(!empty($_obj['forward'])) {
?><input type="hidden" name="forward" value="<?php
echo $_obj['forward'];
?>
" /><?php
}
?> &nbsp;<input type="button" value=" 我拒绝 " onclick="javascript:location.href='/';" /></div>
	</form>
<?php
}else {
?>
		<script type="text/javascript" src="<?php
echo $_obj['G_siteurl'];
?>
/include/js/ajax.js"></script>
		<script type="text/javascript"><!--//--><![CDATA[//><!--
			var pswd1 = '';
			var pswd2 = '';
			function checkname(val) {
				var info = $('rd8usg');
				var checkmsg = new Array(
					'<span style="color: Green;">该用户名可用</span>',
					'<span style="color: Red;">用户名中包含非法字符</span>',
					'<span style="color: Red;">此用户名已被注册</span>',
					'<span style="color: Red;">此用户名禁止注册</span>',
					'<span style="color: Red;">用户名长度非法</span>'
				);
				var namelength = cstrlen(val);
				if(namelength < <?php
echo $_obj['minlen'];
?>
 || namelength > <?php
echo $_obj['maxlen'];
?>
) {
					info.innerHTML = checkmsg[4];
					return false;
				}
				var re = /\\|%|&| |\'|\"|\/|\*|,|<|>|\r|\t|\n|#/im;
				var m = val.match(re);
				if(m != null) {
					info.innerHTML = checkmsg[1];
					return false;
				}
				info.innerHTML = '<span style="color: Blue;">检测中，请稍等...</span>';
				var ajax = AJAX_Init();
					ajax.open("GET", "<?php
echo $_obj['G_siteurl'];
?>
/ajax.php?action=checkreg&tpl_user="+encodeURIComponent(val), true);
					ajax.onreadystatechange = function(){
						if(ajax.readyState == 4 && ajax.status == 200) {
							info.innerHTML = checkmsg[ajax.responseText];
						}
					}
					ajax.send(null);
			}
			function checkpswd(key,val) {
				if(key == 'pswd1') {
					pswd1 = val;
				}
				if(key == 'pswd2') {
					pswd2 = val;
				}
				if(pswd1 && pswd2) {
					if(pswd1 != pswd2) {
						$('rd8psg2').innerHTML = '<span style="color: Red;">两次输入的密码不一样</span>';
					}else {
						$('rd8psg2').innerHTML = '<span style="color: Green;">两次输入一致</span>';
					}
				}
			}
			function checkmail(val) {
				if(!val) {
					$('rd8mg').innerHTML = '<span style="color: Red">请输入邮箱</span>';
					return;
				}
				var re = /^\w[\w\.\-]*@\w[\w\-]+(\.\w+)+$/;
				var m = val.match(re);
				if(m == null) $('rd8mg').innerHTML = '<span style="color: Red">非法的邮箱地址</span>';
				else $('rd8mg').innerHTML = '';
			}
		//--><!]]></script>
		<form action="" method="post">
			<div class="rd8_head">注册资料</div>
			<div class="rd8_col0" style="margin-left:">
				<table>
					<tr class="rd8_col0">
						<td width="30%" align="right">用&nbsp;户&nbsp;名:&nbsp;</td>
						<td>&nbsp;<input type="text" size="40" maxlength="20" name="rd8user" value="<?php
echo $_obj['rd8user'];
?>
" onblur="checkname(this.value);" /> <span style="color: Red;">*</span> <span id="rd8usg"></span></td>
					</tr>
					<tr class="rd8_col1">
						<td align="right">密 &nbsp; &nbsp;码:&nbsp;</td>
						<td>&nbsp;<input type="password" size="40" maxlength="75" name="rd8password" onblur="checkpswd('pswd1',this.value);" /> <span style="color: Red;">*</span> <span id="rd8psg"></span></td>
					</tr>
					<tr class="rd8_col0">
						<td align="right">重复密码:&nbsp;</td>
						<td>&nbsp;<input type="password" size="40" maxlength="75" name="rd8password2" onblur="checkpswd('pswd2',this.value);" /> <span style="color: Red;">*</span> <span id="rd8psg2"></span></td>
					</tr>
					<tr class="rd8_col1">
						<td align="right">邮 &nbsp; &nbsp;箱:&nbsp;</td>
						<td>&nbsp;<input type="text" size="40" maxlength="32" name="rd8mail" value="<?php
echo $_obj['rd8mail'];
?>
" onblur="checkmail(this.value);" /> <span style="color: Red;">*</span> <span id="rd8mg"></span></td>
					</tr>
					<tr class="rd8_col1">
						<td align="right">验&nbsp;证&nbsp;码:&nbsp;</td>
						<td>&nbsp;<input type="text" size="31" name="rd8secode" onfocus="javascript:secode_init();" title="请在该框中输入右图显示的字符" />&nbsp;<img id="secode" src="images/favorite.gif" align="absmiddle" alt="请在左框中输入该图显示的字符" style="cursor: pointer;display: none;" onclick="javascript:secode_reload();" /> <span style="color: Red;">*</span> <span id="secode_info">请点击输入框显示验证码</span></td>
					</tr>
				</table>
			</div>
			<div class="rd8_colt" style="text-align: center;"><input type="hidden" name="forward" value="<?php
echo $_obj['forward'];
?>
" /><input type="submit" value=" 提  交 " style="background-color:#FFFFFF;border:1px #B6DBFF solid;" /><input type="hidden" name="action" value="register"/>&nbsp;<input type="reset" value=" 取  消 "  style="background-color:#FFFFFF;border:1px #B6DBFF solid;" /></div>
		</form>
<?php
}
?>
	</div>
