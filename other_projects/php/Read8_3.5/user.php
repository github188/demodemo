<?php
/**
*
*  Copyright:  (c) 2004 - 2008 [AOKOSOFT] Read8
*  Web Site:   http://www.aokosoft.com http://www.read8.net
*  Powered by: [AOKOSOFT] Read8 Group
*  Support:    http://bbs.read8.net
*  License:    http://www.read8.net/license
*
*/

define('IN_Read8', true);
require_once str_replace(array('\\', '/'.basename(__FILE__)), array('/', ''), __FILE__).'/include/init.php';


if(TIMESTAMP > $_SYSTEM['SYSTEM']['task']) {
	$db->update(array(
		'value'   => TIMESTAMP + 300,
	), array(
		'config'  => 'SYSTEM',
		'`index`' => 'task',
	), __TAB_SYSTEM__);
	_config_update('system');
	ignore_user_abort(true);
	header('Location: user.php');
	define('IN_Read8_TASK', true);
	include ROOT.'include/task.inc.php';
	exit;
}


if($Global['F_uid']) {
	include _config('story');

	$acp = $db->selectOne(array(
		'field' => 'id, isfounder',
		'from'  => __TAB_USER_ACP__,
		'where' => array(
			'uid' => $Global['F_uid'],
		),
	));
?>
$('rd8_userlogin').innerHTML = '<?php echo $Global['F_username']; ?>, 欢迎回来! <?php if($_SYSTEM['STORY']['open']) { ?> &nbsp; &nbsp;<a href="my/story_new.php">发表短篇<\/a><?php } ?><?php if($acp['isfounder'] || $db->selectOne(array('field' => 'id', 'from' => __TAB_USER_TPWT__, 'where' => array('uid' => $Global['F_uid'])))) { ?> &nbsp; &nbsp;<a href="typewriter/" target="_blank">作品管理<\/a><?php } ?><?php if($acp) { ?> &nbsp; &nbsp;<a href="admin/" target="_blank">访问后台<\/a><?php } ?> &nbsp; &nbsp;<a href="login.php?action=logoff">安全退出<\/a>';
<?php
}else {
?>
$('rd8_userlogin').innerHTML = '<form action="login.php?action=login" method="post">用户名: <input maxlength="20" size="6" name="loginuser" \/> &nbsp;密 &nbsp;码: <input type="password" maxlength="30" size="6" name="loginpwd" \/><?php
if(_secode(1)) {
?> &nbsp;验证码: <input type="text" name="hash" size="6" title="请在该框中输入右图显示的字符" \/><img src="download.php?action=gethashimg" alt="请在左框中输入该图显示的字符" style="cursor: pointer;" onclick="javascript:this.src=\'download.php?action=gethashimg&amp;xx=\'+Math.random();" align="absmiddle" \/><?php
}
?> &nbsp;<!--label for="userlogin_hidden"><input type="checkbox" name="hidden" id="userlogin_hidden" value="1" \/> 隐身</label> &nbsp;-->记住我: <select name="cookie_time"><option value="3600">一小时<\/option><option value="86400">一&nbsp;&nbsp;天<\/option><option value="604800">一&nbsp;&nbsp;周<\/option><option value="2592000">一个月<\/option><option value="31536000">一&nbsp;&nbsp;年<\/option><\/select> &nbsp;<input type="submit" value="登录" \/> &nbsp;<a href="register.php">注册<\/a> &nbsp;<a href="profile.php?action=sendpwd">忘记密码<\/a><\/form>';
<?php
}

/*
</div>
			<!-- IF C_loghashenable -->
			<div style="padding-left: 3px;"></div>
			<!-- ENDIF -->
			<div style="padding-left: 3px;">状 &nbsp;态(<span style="text-decoration: underline;">S</span>): <label accesskey="S"><input type="checkbox" name="hidden" value="1" />隐身</label><!-- IF C_loghashenable --><img src="download.php?action=gethashimg" alt="请在左框中输入该图显示的字符" style="cursor: pointer;" onclick="javascript:this.src='download.php?action=gethashimg&amp;xx='+Math.random();" /><!-- ENDIF --></div>
			<div style="padding-left: 3px;">保 &nbsp;持(<span style="text-decoration: underline;">K</span>):
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
				<input type="hidden" name="forward" value="{forward}" />
			</div>
			<div style="width: 160px;text-align: center;margin: 10px 0;"><input type="submit" value="登录" /> <input type="reset" value="清除" /></div>
			<div style="width: 160px;text-align: center;"><a href="profile.php?action=register" rel="external">用户注册</a> &nbsp; <a href="profile.php?action=sendpwd">忘记密码</a></div>


			<!--登录框 -->

			用户名：<input name="loginuser" type="text" size="10" />
			密码：<input name="loginpwd" type="password" size="10" />
			状  态(S)：<input name="" type="checkbox" value="" />隐身
			保  持(K)：<select name="">
			  <option>一 小 时</option>
			  <option>一    天</option>
			  <option>一    周</option>
			  <option>一 个 月</option>
			  <option>一    年</option>
			</select>
			<input name="Submit" type="submit" value="登录" class="login" />
			&nbsp;<a href="#">用户注册</a>&nbsp;<a href="#">忘记密码</a>
			</form>
*/