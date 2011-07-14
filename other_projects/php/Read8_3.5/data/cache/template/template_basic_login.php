<?php
if(!defined('IN_Read8')) {
	exit('Access Denied (template_basic_login.php)');
}
?>
<?php
if($_obj['action'] == "viewstatus") {
?>
	<?php
if(!empty($_obj['F_uid'])) {
?>
		<div style="width: 160px;text-align: center;margin-bottom: 10px;">欢迎您，<?php
echo $_obj['F_username'];
?>
</div>
		<div style="width: 160px;text-align: center;"><a href="my/profile.php">进入个人用户中心</a></div>
		<div style="width: 160px;text-align: center;"><a href="author/profile.php">进入作家管理中心</a></div>
	<?php
}else {
?>
		<form method="post" action="login.php?action=login">
			<div style="padding-left: 3px;margin-top: 20px;">用户名(<span style="text-decoration: underline;">U</span>): <input maxlength="20" size="13" name="loginuser" accesskey="U" style="border:1px #B6DBFF solid;"/></div>
			<div style="padding-left: 3px;margin-top: 10px;">密 &nbsp;码(<span style="text-decoration: underline;">P</span>): <input type="password" maxlength="30" size="13" name="loginpwd" accesskey="P" style="border:1px #B6DBFF solid;"/></div>
			<?php
if(!empty($_obj['C_loghashenable'])) {
?>
			<div style="padding-left: 3px;margin-top: 10px;">验证码(<span style="text-decoration: underline;">C</span>): <input type="text" name="hash" size="13" accesskey="C" title="请在该框中输入右图显示的字符" style="border:1px #B6DBFF solid;"/></div>
			<?php
}
?>
			<div style="padding-left: 3px;margin-top: 10px;">状 &nbsp;态(<span style="text-decoration: underline;">S</span>): <label accesskey="S"><input type="checkbox" name="hidden" value="1" style="border:1px #B6DBFF solid;"/>隐身</label><?php
if(!empty($_obj['C_loghashenable'])) {
?><img src="download.php?action=gethashimg" alt="请在左框中输入该图显示的字符" style="cursor: pointer;" onclick="javascript:this.src='download.php?action=gethashimg&amp;xx='+Math.random();" /><?php
}
?></div>
			<div style="padding-left: 3px;margin-top: 10px;">保 &nbsp;持(<span style="text-decoration: underline;">K</span>):
				<label accesskey="K">
					<select name="cookie_time" style="border:1px #B6DBFF solid;">
						<!--option value="0">至页面关闭</option-->
						<option value="3600">一&nbsp;&nbsp;小&nbsp;&nbsp;时</option>
						<option value="86400">一&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;天</option>
						<option value="604800">一&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;周</option>
						<option value="2592000">一&nbsp;&nbsp;个&nbsp;&nbsp;月</option>
						<option value="31536000">一&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;年</option>
					</select>
				</label>
				<input type="hidden" name="forward" value="<?php
echo $_obj['forward'];
?>
" />
			</div>
			<div style="width: 160px;text-align: center;margin: 10px 0;"><input type="submit" value="登录"  style="border:1px #B6DBFF solid;background-color:#FFFFFF;"/> <input type="reset" value="清除" /></div>
			<div style="width: 160px;text-align: center;"><a href="register.php" rel="external">用户注册</a> &nbsp; <a href="register.php?action=sendpwd">忘记密码</a></div>
		</form>
	<?php
}
?>
<?php
}else {
?>
	<div class="rd8_main">
		<div class="rd8_full">
			<div class="rd8_fullborder">
				<form action="login.php?action=login" method="post">
					<div class="rd8_head">用户登录</div>
					<div style="width: 1004px;text-align: center;margin-top: 20px;">用户名(<span style="text-decoration: underline;">U</span>): <input maxlength="20" size="26" name="loginuser" accesskey="U" style="border:1px #CCC solid;"/></div>
					<div style="width: 1004px;text-align: center;margin-top: 10px;">密 &nbsp;码(<span style="text-decoration: underline;">P</span>): <input type="password" size="26" maxlength="30" name="loginpwd" accesskey="P" style="border:1px #CCC solid;"/></div>
					<?php
if(!empty($_obj['C_loghashenable'])) {
?>
					<div style="width: 886px;text-align: center;margin-top: 10px;">验证码(<span style="text-decoration: underline;">C</span>): <input type="text" size="17" name="hash" accesskey="C" title="请在该框中输入右图显示的字符" style="border:1px #CCC solid;"/>&nbsp;<img src="download.php?action=gethashimg" alt="请在左框中输入该图显示的字符" style="cursor: pointer;" onclick="javascript:this.src='download.php?action=gethashimg&amp;xx='+Math.random();" /></div>
					<?php
}
?>
					<div style="width: 976px;text-align: center;margin-top: 10px;">
						保 &nbsp;持(<span style="text-decoration: underline;">K</span>):
						<label accesskey="K">
							<select name="cookie_time" style="width: 116px;border:1px #CCC solid;">
								<!--option value="0">&nbsp;&nbsp;至页面关闭</option-->
								<option value="3600">&nbsp;&nbsp;一&nbsp;&nbsp;小&nbsp;&nbsp;时</option>
								<option value="86400">&nbsp;&nbsp;一&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;天</option>
								<option value="604800">&nbsp;&nbsp;一&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;周</option>
								<option value="2592000">&nbsp;&nbsp;一&nbsp;&nbsp;个&nbsp;&nbsp;月</option>
								<option value="31536000">&nbsp;&nbsp;一&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;年</option>
							</select>
						</label>
						<label accesskey="S"><input type="checkbox" name="hidden" value="1" />隐身</label>
						<input type="hidden" name="forward" value="<?php
echo $_obj['forward'];
?>
" style="border:1px #B6DBFF solid;"/>
					</div>
					<div style="width: 1004px;text-align: center;margin-top: 10px;"><input type="submit" value="   登  录   " name="submit" style="border:1px #CCC solid;background-color:#FFFFFF;"/></div>
					<div class="rd8_colt" style="width: 1004px;text-align: center;margin-top: 20px;"><a href="register.php">用户注册</a> &nbsp; &nbsp; &nbsp;<a href="register.php?action=sendpwd">忘记密码</a></div>
				</form>
			</div>
		</div>
	</div>
<?php
}
?>
