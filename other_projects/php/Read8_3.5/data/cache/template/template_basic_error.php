<?php
if(!defined('IN_Read8')) {
	exit('Access Denied (template_basic_error.php)');
}
?>
	<div id="rd8_error">
		<div id="rd8_errormsg">
			<div class="rd8_head">错误信息</div>
			<div id="rd8_errorinfo"><?php
echo $_obj['msg'];
?>
</div>
		<?php
if(!empty($_obj['forward'])) {
?><div class="rd8_errorlink"><a href="javascript:history.back(1);">点击这里返回上一页</a></div><?php
}
?>
		<?php
if(!empty($_obj['login'])) {
?>
			<div class="rd8_errorlink"><?php
if(!empty($_obj['F_uid'])) {
?>您已登录为: <?php
echo $_obj['F_username'];
?>
，该帐号不具有执行该操作的权限<br />您可以选择: <a href="login.php?action=logoff">[退出]</a> 或者 以其他用户身份登录: <?php
}else {
?>您尚未登录<?php
}
?></div>
			<form method="post" action="login.php?action=login">
				<div class="rd8_errorlink">用户名(<span style="text-decoration: underline;">U</span>): <input maxlength="20" size="13" name="loginuser" accesskey="U" /></div>
				<div class="rd8_errorlink">密 &nbsp;码(<span style="text-decoration: underline;">P</span>): <input type="password" maxlength="30" size="13" name="loginpwd" accesskey="P" /></div>
				<?php
if(!empty($_obj['C_loghashenable'])) {
?>
				<div class="rd8_errorlink">验证码(<span style="text-decoration: underline;">C</span>): <input type="text" name="hash" size="13" accesskey="C" title="请在该框中输入右图显示的字符" /></div>
				<?php
}
?>
				<div class="rd8_errorlink">状 &nbsp;态(<span style="text-decoration: underline;">S</span>):<label accesskey="S"><input type="checkbox" name="hidden" value="1" />隐身</label><?php
if(!empty($_obj['C_loghashenable'])) {
?><img src="download.php?action=gethashimg" alt="请在左框中输入该图显示的字符" style="cursor: pointer;" onclick="javascript:this.src='download.php?action=gethashimg&amp;xx='+Math.random();" /><?php
}
?></div>
				<div class="rd8_errorlink">保 &nbsp;持(<span style="text-decoration: underline;">K</span>):
					<label accesskey="K">
						<select name="cookie_time">
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
				<div class="rd8_errorlink"><input type="submit" value=" 登录 " /> <input type="reset" value=" 清除 " /></div>
			</form>
		<?php
}
?>
		</div>
	</div>