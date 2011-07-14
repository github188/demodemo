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


header('Content-Type: text/html; charset=utf-8');


if(is_file('../data/install.close')) {
	exit('如需重新安装，请删除 ./data/install.close 文件');
}

if(!function_exists('file_put_contents')) {
	function file_put_contents($path, $contents){
		$fp = fopen($path, 'wb');
		if($fp === false)
			return false;

		$len = fwrite($fp, $contents);
		if($len === false)
			return false;

		fclose($fp);
		return $len;
	}
}


if(!get_magic_quotes_gpc()) {
	$_GET  = superaddslash($_GET);
	$_POST = superaddslash($_POST);
}

$version = '3.5';

$install_steps = array(
	1 => '欢迎',
	2 => '许可协议',
	3 => '检查环境',
	4 => '设置数据库',
	5 => '初始化数据',
	6 => '设置基本信息',
	7 => '完成',
);

$install_step_description = array(
	1 => ' &nbsp; &nbsp;欢迎使用读吧书网安装程序。',
	2 => ' &nbsp; &nbsp;接受许可协议方可安装。',
	3 => ' &nbsp; &nbsp;安装程序将检查当前服务器的环境，所有基本配置符合后，才能继续安装，若有推荐配置不符合要求，书网的部分功能可能无法正常使用。',
	4 => ' &nbsp; &nbsp;请填写书网所使用的数据库相关信息，通常由您自己或您的服务提供商设置，如果您是独立服务器用户，请自行安装并设置好数据库。',
	5 => ' &nbsp; &nbsp;安装程序将写入基本数据并初始化网站。',
	6 => ' &nbsp; &nbsp;请填写网站的基本信息和站长的前后台用户名及密码，网站信息可以在安装完成后登录后台再次修改。此用户将是全站最高权限用户，请牢记您的帐号密码。',
	7 => ' &nbsp; &nbsp;安装程序会自动删除 install 文件夹，若未能自动删除，请通过 ftp 或直接从服务器手动删除该文件夹。',
);

if(empty($_GET['step']))
	$_GET['step'] = 0;

$step = $_GET['step'];
if(!$step || !ris_int($step) || $step < 1 || $step > 7) {
	$step = 1;
}



if($step == 1) {
	step_1();
}elseif($step == 2) {
	step_2();
}elseif($step == 3) {
	step_3();
}elseif($step == 4) {
	step_4();
}elseif($step == 5) {
	step_5();
}elseif($step == 6) {
	step_6();
}elseif($step == 7) {
	step_7();
}

function step_7(){
	file_put_contents('../data/install.close', time());
	insheader();
?>
				<form action=".." method="get">
					<div class="part_head">恭喜</div>
					<div class="part_body">
						<div class="part_text">
							<table width="100%">
								<tr align="left">
									<td>您已成功安装读吧书网。</td>
								</tr>
								<tr align="left">
									<td><span style="color: Red;">安装程序将自动删除，如果安装程序未能自动删除，请手动删除 install 文件夹。</span></td>
								</tr>
							</table>
						</div>
					</div>

					<div id="goto">
						<input class="goto" type="submit" onfocus="this.blur();" value="  登录网站  " />
					</div>
				</form>
<?php
	insfooter();
}

function step_6(){

	global $check;

	$check = 0;
	get_post_var('check');

	global $sitename, $front_user, $front_password1, $front_password2, $front_mail, $acp_user, $acp_password1, $acp_password2, $chmod_dir, $chmod_file;
	$sitename = $front_user = $front_password1 = $front_password2 = $front_mail = $acp_user = $acp_password1 = $acp_password2 = '';

	if($check) {
		get_post_var('sitename');
		get_post_var('front_user');
		get_post_var('front_password1');
		get_post_var('front_password2');
		get_post_var('front_mail');
		get_post_var('acp_user');
		get_post_var('acp_password1');
		get_post_var('acp_password2');
		get_post_var('chmod_dir');
		get_post_var('chmod_file');

		if(preg_match("/\\|%|&| |\'|\"|\/|\*|,|<|>|\r|\t|\n|#/im", $front_user)) {
			$halt['front_user'] = '用户名中包含非法字符';
		}
		if(!$front_user) {
			$halt['front_user'] = '请输入用户名';
		}
		if($front_password1 != $front_password2) {
			$halt['front_password'] = '两次输入的密码不一致';
		}
		if(!$front_password1) {
			$halt['front_password'] = '请输入密码';
		}
		if(!$front_mail) {
			$halt['front_mail'] = '请输入您的邮箱';
		}

		if(preg_match("/\\|%|&| |\'|\"|\/|\*|,|<|>|\r|\t|\n|#/im", $acp_user)) {
			$halt['acp_user'] = '用户名中包含非法字符';
		}
		if(!$acp_user) {
			$halt['acp_user'] = '请输入用户名';
		}
		if($acp_password1 != $acp_password2) {
			$halt['acp_password'] = '两次输入的密码不一致';
		}
		if(!$acp_password1) {
			$halt['acp_password'] = '请输入密码';
		}

		if(empty($halt)) {
			include '../config.inc.php';
			extract($C_MySQL);

			global $db;
			$db = mysql_connect($hostname, $username, $password);
			mysql_select_db($database);

			mysql_query('SET NAMES \'utf8\'');
			mysql_query('SET character_set_connection=utf8, character_set_results=utf8, character_set_client=binary');
			mysql_query('SET sql_mode = \'\'');

			mysql_query('UPDATE `'.$_prefix_.'read8_system` SET `value` = \''.$sitename.'\' WHERE `config` = \'SYSTEM\' AND `index` = \'SITE_NAME\'');
			mysql_query('REPLACE INTO `'.$_prefix_.'read8_user` (`uid`, `username`, `pwd`, `email`, `dateline`) VALUES (1, \''.$front_user.'\', \''.md5($front_password1).'\', \''.$front_mail.'\', '.time().')');
			mysql_query('REPLACE INTO `'.$_prefix_.'read8_user_acp` (`id`, `uid`, `name`, `pswd`, `isfounder`, `privilege`, `accessip`, `history`) VALUES (1, 1, \''.$acp_user.'\', \''.md5($acp_password1).'\', 1, \'\', \'\', \'\')');

			if(PHP_OS != 'WINNT') {
				mysql_query('UPDATE `'.$_prefix_.'read8_system` SET `value` = \''.octdec($chmod_dir).'\' WHERE `config` = \'SAFE\' AND `index` = \'chmod_dir\'');
				mysql_query('UPDATE `'.$_prefix_.'read8_system` SET `value` = \''.octdec($chmod_file).'\' WHERE `config` = \'SAFE\' AND `index` = \'chmod_file\'');
			}

			header('Location: ?step=7');
			exit;
		}
	}

	insheader();
?>
				<form action="?step=6" method="post">
					<div class="part_head">网站信息</div>
					<div class="part_body">
						<div class="part_text">
							<table width="100%">
								<tr align="left">
									<td width="18%">网站名称</td>
									<td width="32%"><input type="text" name="sitename" value="<?php echo html_show($sitename, false); ?>" /></td>
									<td>此处以后可通过后台修改</td>
								</tr>
							</table>
						</div>
					</div>

					<div class="part_head">用户信息</div>
					<div class="part_body">
						<div class="part_text">
							<table width="100%">
								<tr align="left">
									<td width="18%">用 户 名</td>
									<td width="32%"><input type="text" name="front_user" value="<?php echo html_show($front_user, false); ?>" /></td>
									<td>登录书网的用户名</td>
								</tr>
<?php
	if(isset($halt['front_user'])) {
?>
								<tr align="left">
									<td>&nbsp;</td>
									<td colspan="2"><span style="color: Red;"><?php echo $halt['front_user']; ?></span></td>
								</tr>
<?php
	}
?>
								<tr align="left">
									<td>密 &nbsp; &nbsp;码</td>
									<td><input type="password" name="front_password1" value="" /></td>
									<td>登录书网的密码</td>
								</tr>
<?php
	if(isset($halt['front_password'])) {
?>
								<tr align="left">
									<td>&nbsp;</td>
									<td colspan="2"><span style="color: Red;"><?php echo $halt['front_password']; ?></span></td>
								</tr>
<?php
	}
?>
								<tr align="left">
									<td>重复密码</td>
									<td><input type="password" name="front_password2" value="" /></td>
									<td>请重复一次密码</td>
								</tr>
								<tr align="left">
									<td>电子邮件</td>
									<td><input type="text" name="front_mail" value="<?php echo html_show($front_mail, false); ?>" /></td>
									<td>请输入您的邮箱</td>
								</tr>
<?php
	if(isset($halt['front_mail'])) {
?>
								<tr align="left">
									<td>&nbsp;</td>
									<td colspan="2"><span style="color: Red;"><?php echo $halt['front_mail']; ?></span></td>
								</tr>
<?php
	}
?>
							</table>
						</div>
					</div>

					<div class="part_head">管理员信息</div>
					<div class="part_body">
						<div class="part_text">
							<table width="100%">
								<tr align="left">
									<td width="18%">用 户 名</td>
									<td width="32%"><input type="text" name="acp_user" value="<?php echo html_show($acp_user, false); ?>" /></td>
									<td>登录后台的用户名，可以和前台用户名不同</td>
								</tr>
<?php
	if(isset($halt['acp_user'])) {
?>
								<tr align="left">
									<td>&nbsp;</td>
									<td colspan="2"><span style="color: Red;"><?php echo $halt['acp_user']; ?></span></td>
								</tr>
<?php
	}
?>
								<tr align="left">
									<td>密 &nbsp; &nbsp;码</td>
									<td><input type="password" name="acp_password1" value="" /></td>
									<td>登录后台的密码，可以和前台密码不同</td>
								</tr>
<?php
	if(isset($halt['acp_password'])) {
?>
								<tr align="left">
									<td>&nbsp;</td>
									<td colspan="2"><span style="color: Red;"><?php echo $halt['acp_password']; ?></span></td>
								</tr>
<?php
	}
?>
								<tr align="left">
									<td>重复密码</td>
									<td><input type="password" name="acp_password2" value="" /></td>
									<td>请重复一次密码</td>
								</tr>
							</table>
						</div>
					</div>
<?php
	if(PHP_OS != 'WINNT') {
?>
					<script type="text/javascript"><!--//--><![CDATA[//><!--
						function $(objectId){
							return document.getElementById(objectId);
						}
						function calcmod(prefix){
							var user = 0;
							var group = 0;
							var other = 0;
							for(var i = 1; i <= 3; i++) {
								if(i == 1) v = 4;
								else if(i == 2) v = 2;
								else v = 1;
								if($(prefix + '_user'+i).checked) user = user + v;
								if($(prefix + '_group'+i).checked) group = group + v;
								if($(prefix + '_other'+i).checked) other = other + v;
							}
							$(prefix + '_mod').value = '' + user + group + other;
						}
					//-><!]]></script>

					<div class="part_head">权限设置</div>
					<div class="part_body">
						<div class="part_text">
							<table width="100%">
								<tr align="left">
									<td colspan="3">您正在使用 UNIX/Linux 类服务器，请设置程序创建文件的权限。</td>
								</tr>
								<tr align="left">
									<td colspan="3">您也可以在安装后到“系统设置 => 安全设置”中修改。</td>
								</tr>
								<tr align="left">
									<td>目 &nbsp;录</td>
									<td>
										<input type="text" name="chmod_dir" id="d_mod" value="777" readonly="readonly" />
										<table>
											<tr><td>所有者</td><td>组</td><td>其他人</td></tr>
											<tr>
												<td><label for="d_user1"><input type="checkbox" id="d_user1" value="4" onclick="javascript:calcmod('d');" checked="checked" />读取</label></td>
												<td><label for="d_group1"><input type="checkbox" id="d_group1" value="4" onclick="javascript:calcmod('d');" checked="checked" />读取</label></td>
												<td><label for="d_other1"><input type="checkbox" id="d_other1" value="4" onclick="javascript:calcmod('d');" checked="checked" />读取</label></td>
											</tr>
											<tr>
												<td><label for="d_user2"><input type="checkbox" id="d_user2" value="2" onclick="javascript:calcmod('d');" checked="checked" />写入</label></td>
												<td><label for="d_group2"><input type="checkbox" id="d_group2" value="2" onclick="javascript:calcmod('d');" checked="checked" />写入</label></td>
												<td><label for="d_other2"><input type="checkbox" id="d_other2" value="2" onclick="javascript:calcmod('d');" checked="checked" />写入</label></td>
											</tr>
											<tr>
												<td><label for="d_user3"><input type="checkbox" id="d_user3" value="1" onclick="javascript:calcmod('d');" checked="checked" />执行</label></td>
												<td><label for="d_group3"><input type="checkbox" id="d_group3" value="1" onclick="javascript:calcmod('d');" checked="checked" />执行</label></td>
												<td><label for="d_other3"><input type="checkbox" id="d_other3" value="1" onclick="javascript:calcmod('d');" checked="checked" />执行</label></td>
											</tr>
										</table>
									</td>
									<td>若您对此不熟悉，请设为 777。</td>
								</tr>
								<tr align="left">
									<td>文 &nbsp;件</td>
									<td>
										<input type="text" name="chmod_file" id="f_mod" value="777" readonly="readonly" />
										<table>
											<tr><td>所有者</td><td>组</td><td>其他人</td></tr>
											<tr>
												<td><label for="f_user1"><input type="checkbox" id="f_user1" value="4" onclick="javascript:calcmod('f');" checked="checked" />读取</label></td>
												<td><label for="f_group1"><input type="checkbox" id="f_group1" value="4" onclick="javascript:calcmod('f');" checked="checked" />读取</label></td>
												<td><label for="f_other1"><input type="checkbox" id="f_other1" value="4" onclick="javascript:calcmod('f');" checked="checked" />读取</label></td>
											</tr>
											<tr>
												<td><label for="f_user2"><input type="checkbox" id="f_user2" value="2" onclick="javascript:calcmod('f');" checked="checked" />写入</label></td>
												<td><label for="f_group2"><input type="checkbox" id="f_group2" value="2" onclick="javascript:calcmod('f');" checked="checked" />写入</label></td>
												<td><label for="f_other2"><input type="checkbox" id="f_other2" value="2" onclick="javascript:calcmod('f');" checked="checked" />写入</label></td>
											</tr>
											<tr>
												<td><label for="f_user3"><input type="checkbox" id="f_user3" value="1" onclick="javascript:calcmod('f');" checked="checked" />执行</label></td>
												<td><label for="f_group3"><input type="checkbox" id="f_group3" value="1" onclick="javascript:calcmod('f');" checked="checked" />执行</label></td>
												<td><label for="f_other3"><input type="checkbox" id="f_other3" value="1" onclick="javascript:calcmod('f');" checked="checked" />执行</label></td>
											</tr>
										</table>
									</td>
									<td>若您对此不熟悉，请设为 777。</td>
								</tr>
							</table>
						</div>
					</div>
<?php
	}
?>

					<div id="goto">
						<input type="hidden" name="check" value="1" />
						<input class="goto_next" type="submit" onfocus="this.blur();" value="下一步    " />
					</div>
				</form>
<?php
	insfooter();
}

function step_5(){
	include '../config.inc.php';
	extract($C_MySQL);

	insheader();

	global $db;
	$db = mysql_connect($hostname, $username, $password);
	mysql_select_db($database);

	mysql_query('SET NAMES \'utf8\'');
	mysql_query('SET character_set_connection=utf8, character_set_results=utf8, character_set_client=binary');
	mysql_query('SET sql_mode = \'\'');

	$sqls = array();
	foreach(explode(';'."\n", str_replace("\r", '', file_get_contents('read8.sql'))) as $query) {
		$sql = explode("\n", $query);
		foreach($sql as $k => $v) {
			$v = trim($v);
			if(!$v || $v[0] == '#' || $v[0] == '-')
				unset($sql[$k]);
		}
		$query = implode(' ', $sql);
		if(!$query)
			continue;

		$sqls[] = str_replace(array('\r', '\n', '`rd8_'), array("\r", "\n", '`'.$_prefix_), $query);
	}
?>
				<form action="" method="get">
					<div class="part_head">初始化数据</div>
					<div class="part_body">
						<div class="part_text">
							<table width="100%">
<?php
	$server_version = mysql_get_client_info();
	foreach($sqls as $sql) {
		if(preg_match('/^CREATE TABLE IF NOT EXISTS `(\w+)`/iU', $sql, $match)) {
?>
								<tr align="left">
									<td colspan="3">创建 <?php echo $match[1]; ?> ... 完成</td>
								</tr>
<?php
			if(version_compare($server_version, '5.0', '<')) {
				$sql = str_replace('ENGINE', 'TYPE', $sql);
				if(version_compare($server_version, '4.1', '<')) {
					$sql = str_replace('DEFAULT CHARSET=utf8', '', $sql);
				}
			}
		}
		mysql_query($sql);
	}
	unset($server_version);

	$SITE_ADDR = 'http://'.$_SERVER['HTTP_HOST'].str_replace('/install/index.php', '', $_SERVER['SCRIPT_NAME']);
	mysql_query('UPDATE `'.$_prefix_.'read8_system` SET `value` = \''.$SITE_ADDR.'\' WHERE `config` = \'SYSTEM\' AND `index` = \'SITE_ADDR\'');
?>
							</table>
						</div>
					</div>

					<div id="goto">
						<input type="hidden" name="step" value="6" />
						<input class="goto_next" type="submit" onfocus="this.blur();" value="下一步    " />
					</div>
				</form>
<?php
	insfooter();
}

function step_4(){
	include '../config.inc.php';
	$perm = check_perm('config.inc.php');

	global $hostname, $username, $password, $database, $_prefix_;

	$hostname = $username = $password = $database = '';
	$_prefix_ = 'rd8_';

	global $check, $cover;

	$check = $cover = 0;
	get_post_var('check');
	get_post_var('cover');

	if($perm && $check) {
		get_post_var('hostname');
		get_post_var('username');
		get_post_var('password');
		get_post_var('database');
		get_post_var('_prefix_');
	}else {
		if(isset($C_MySQL) && is_array($C_MySQL)) {
			extract($C_MySQL);
		}
	}

	$halt = '';

	if($check) {
		if($perm) {
			file_put_contents('../config.inc.php',  '<'.'?php'."\n".
													'// 此文件使用 UTF-8 编码，请勿使用记事本编辑'."\n".
													"\n".
													'$C_MySQL = array('."\n".
													'	\'hostname\' => \''.$hostname.'\','."\n".
													'	\'username\' => \''.$username.'\','."\n".
													'	\'password\' => \''.$password.'\','."\n".
													'	\'database\' => \''.$database.'\','."\n".
													'	\'_prefix_\' => \''.$_prefix_.'\','."\n".
													');');
			$hostname = $username = $password = $database = '';
			$_prefix_ = 'rd8_';
			include '../config.inc.php';
			if(isset($C_MySQL) && is_array($C_MySQL)) {
				extract($C_MySQL);
			}
		}

		global $db;
		$db = mysql_connect($hostname, $username, $password);
		if(!$db) {
			$halt = '无法连接到数据库服务器，请检查用户名和密码是否正确。';
		}else {
			if(!mysql_select_db($database)) {
				if(!mysql_query('CREATE DATABASE IF NOT EXISTS '.$database)) {
					$halt = '找不到指定的数据库，尝试创建失败，请检查该数据库是否存在。';
				}
			}
		}

		if(!$halt) {
			if($cover) {
				$rs = mysql_query('SHOW TABLES LIKE \''.$_prefix_.'%\'');
				while($tbl = mysql_fetch_row($rs)) {
					if($cover == 1) {
						mysql_query('DROP TABLE '.$tbl[0]);
					}else {
						mysql_query('ALTER TABLE '.$tbl[0].' RENAME bak_'.$tbl[0]);
					}
				}
			}

			header('Location: ?step=5');
			exit;
		}
	}

	insheader();
?>
				<form action="?step=4" method="post">
					<div class="part_head">数据库信息</div>
					<div class="part_body">
						<div class="part_text">
							<table width="100%">
<?php
	if(!$perm) {
?>
								<tr align="left">
									<td colspan="3"><span style="color: Red;">配置文件 ./config.inc.php 不可写，您需要手动修改文件设置数据库并在此复查设置是否正确。</span></td>
								</tr>
<?php
	}
?>
<?php
	if($halt) {
?>
								<tr align="left">
									<td colspan="3"><span style="color: Red;"><?php echo $halt; ?></span></td>
								</tr>
<?php
	}
?>
								<tr align="left">
									<td>主 &nbsp;机</td>
									<td><input type="text" name="hostname" value="<?php echo html_show($hostname, false); ?>"<?php if(!$perm) { ?> readonly="readonly"<?php } ?> /></td>
									<td>通常为 localhost 或数据库服务器的 IP</td>
								</tr>
								<tr align="left">
									<td>用户名</td>
									<td><input type="text" name="username" value="<?php echo html_show($username, false); ?>"<?php if(!$perm) { ?> readonly="readonly"<?php } ?> /></td>
									<td>登录数据库服务器所使用的用户名</td>
								</tr>
								<tr align="left">
									<td>密 &nbsp;码</td>
									<td><input type="password" name="password" value="<?php echo html_show($password, false); ?>"<?php if(!$perm) { ?> readonly="readonly"<?php } ?> /></td>
									<td>对应登录数据库服务器所使用用户的密码</td>
								</tr>
								<tr align="left">
									<td>数据库</td>
									<td><input type="text" name="database" value="<?php echo html_show($database, false); ?>"<?php if(!$perm) { ?> readonly="readonly"<?php } ?> /></td>
									<td>本程序所使用数据库的名字</td>
								</tr>
								<tr align="left">
									<td>表前缀</td>
									<td><input type="text" name="_prefix_" value="<?php echo html_show($_prefix_, false); ?>"<?php if(!$perm) { ?> readonly="readonly"<?php } ?> /></td>
									<td>数据表名前缀，如果在同一数据库安装多个书网，请修改此项</td>
								</tr>
								<tr align="left">
									<td colspan="3"><label for="backup"><input type="radio" name="cover" id="backup" value="-1"<?php if($cover == -1) { ?> checked="checked"<?php } ?> /> 备份已经存在的数据表</label></td>
								</tr>
								<tr align="left">
									<td colspan="3"><label for="cover"><input type="radio" name="cover" id="cover" value="1"<?php if($cover == 1) { ?> checked="checked"<?php } ?> /> 覆盖已经存在的数据表</label></td>
								</tr>
							</table>
						</div>
					</div>

					<div id="goto">
						<input type="hidden" name="check" value="1" />
<?php
	if(!$perm) {
?>
						<input class="goto_reload" type="button" onclick="window.location.reload();" onfocus="this.blur();" value="刷新本页    " />
<?php
	}
?>
						<input class="goto_next" type="submit" onfocus="this.blur();" value="下一步    " />
					</div>
				</form>
<?php
	insfooter();
}

function step_3(){
	insheader();
?>
				<form action="" method="get">
					<div class="part_head">服务器环境</div>
					<div class="part_body">
						<div class="part_text">
							<table width="100%">
								<tr align="center">
									<td>配置项</td>
									<td>基本配置</td>
									<td>推荐配置</td>
									<td>当前配置</td>
								</tr>
								<tr align="center">
									<td>PHP 版本</td>
									<td>4.3+</td>
									<td>5.2.x</td>
									<td><span style="color: <?php echo check_php(true); ?>;font-weight: bold;"><?php echo PHP_VERSION; ?></span></td>
								</tr>
								<tr align="center">
									<td>MySQL 扩展</td>
									<td>3.23+</td>
									<td>4.1+</td>
									<td><span style="color: <?php echo check_mysql(true); ?>;font-weight: bold;"><?php echo mysql_get_client_info(); ?></span></td>
								</tr>
								<tr align="center">
									<td>mbstring 扩展</td>
									<td>支持</td>
									<td>支持</td>
									<td><span style="color: <?php echo check_mbstring(true); ?>;font-weight: bold;"><?php if(check_mbstring()) { ?>支持<?php }else { ?>不支持<?php } ?></span></td>
								</tr>
								<tr align="center">
									<td>远程文件读取</td>
									<td>不限</td>
									<td>支持</td>
									<td><span style="color: <?php echo check_allow_url_fopen(true); ?>;font-weight: bold;"><?php if(check_allow_url_fopen()) { ?>支持<?php }else { ?>不支持<?php } ?></span></td>
								</tr>
								<tr align="center">
									<td>fsockopen 函数</td>
									<td>不限</td>
									<td>支持</td>
									<td><span style="color: <?php echo check_fsockopen(true); ?>;font-weight: bold;"><?php if(check_fsockopen()) { ?>支持<?php }else { ?>不支持<?php } ?></span></td>
								</tr>
								<tr align="center">
									<td>curl 扩展</td>
									<td>不限</td>
									<td>支持</td>
									<td><span style="color: <?php echo check_curl(true); ?>;font-weight: bold;"><?php if(check_curl()) { ?>支持<?php }else { ?>不支持<?php } ?></span></td>
								</tr>
								<tr align="center">
									<td>mkdir 函数</td>
									<td>不限</td>
									<td>支持</td>
									<td><span style="color: <?php echo check_mkdir(true); ?>;font-weight: bold;"><?php if(check_mkdir()) { ?>支持<?php }else { ?>不支持<?php } ?></span></td>
								</tr>
								<tr align="center">
									<td>chmod 函数</td>
									<td>不限</td>
									<td>支持</td>
									<td><span style="color: <?php echo check_chmod(true); ?>;font-weight: bold;"><?php if(check_chmod()) { ?>支持<?php }else { ?>不支持<?php } ?></span></td>
								</tr>
								<tr align="center">
									<td>ini_set 函数</td>
									<td>不限</td>
									<td>支持</td>
									<td><span style="color: <?php echo check_ini_set(true); ?>;font-weight: bold;"><?php if(check_ini_set()) { ?>支持<?php }else { ?>不支持<?php } ?></span></td>
								</tr>
								<tr align="center">
									<td>安全模式</td>
									<td>不限</td>
									<td>关闭</td>
									<td><span style="color: <?php echo check_safe_mode(true); ?>;font-weight: bold;"><?php if(check_safe_mode()) { ?>关闭<?php }else { ?>开启<?php } ?></span></td>
								</tr>
								<tr align="center">
									<td>set_time_limit 函数</td>
									<td>不限</td>
									<td>支持</td>
									<td><span style="color: <?php echo check_set_time_limit(true); ?>;font-weight: bold;"><?php if(check_set_time_limit()) { ?>支持<?php }else { ?>不支持<?php } ?></span></td>
								</tr>
							</table>
						</div>
					</div>

					<div class="part_head">文件权限</div>
					<div class="part_body">
						<div class="part_text">
							<table width="100%">
								<tr align="center">
									<td>文件或目录名</td>
									<td>基本配置</td>
									<td>推荐配置</td>
									<td>当前配置</td>
								</tr>
								<tr align="center">
									<td>./config.inc.php</td>
									<td>不限</td>
									<td>可写</td>
									<td><span style="color: <?php echo check_perm('config.inc.php', true); ?>;font-weight: bold;"><?php if(check_perm('config.inc.php')) { ?>可写<?php }else { ?>不可写<?php } ?></span></span></td>
								</tr>
								<tr align="center">
									<td>./install</td>
									<td>不限</td>
									<td>可写</td>
									<td><span style="color: <?php echo check_perm('install', true); ?>;font-weight: bold;"><?php if(check_perm('install')) { ?>可写<?php }else { ?>不可写<?php } ?></span></span></td>
								</tr>
<?php
	$pass = true;

	foreach(array(
		'data',
		'data/backup',
		'data/cache',
		'data/cache/template',
		'data/logfiles',
		'data/task',
		'data/txt',
		'data/zip',
		'html',
		'html/map',
		'upload',
		'upload/focus',
	) as $v) {
		$pass = $pass && check_perm($v);
?>
								<tr align="center">
									<td>./<?php echo $v; ?></td>
									<td>可写</td>
									<td>可写</td>
									<td><span style="color: <?php echo check_perm($v, true); ?>;font-weight: bold;"><?php if(check_perm($v)) { ?>可写<?php }else { ?>不可写<?php } ?></span></span></td>
								</tr>
<?php
	}
?>
							</table>
						</div>
					</div>

					<div id="goto">
<?php
	if($pass && check_php() && check_mysql() && check_mbstring()) {
?>
						<input type="hidden" name="step" value="4" />
						<input class="goto_next" type="submit" onfocus="this.blur();" value="下一步    " />
<?php
	}else {
?>
						<input class="goto_reload" type="button" onclick="window.location.reload();" onfocus="this.blur();" value="重新检查    " />
<?php
	}
?>
					</div>
				</form>
<?php
	insfooter();
}

function step_2(){
	insheader();
?>
				<form action="" method="get">
					<div class="part_head">许可协议</div>
					<div class="part_body">
						<div class="part_text">
							<p>本程序为澳柯网信版权所有。任何个人或单位不允许私自更改版权信息。澳柯网信保留所有版权权利。对任何修改本程序并出售和发布来赢得的行为，将追究法律责任。</p>
							<p>出于版权和功能的商业策略，我们对部分程序实行了加密。对恶意泄露本站商业程序的用户，我们将停止已有服务，并追究法律责任。</p>
							<p>对于本站程序中所附带的采集工具，不是针对某个站而写的。仅供作为一项技术测试。对使用此工具而发生的任何版权及法律纠纷，本站概不负责。</p>
							<p><label for="accept"><input type="checkbox" id="accept" onclick="javascript:document.getElementById('next').disabled = !this.checked;" /> 我已阅读并同意以上内容</label></p>
						</div>
					</div>

					<div id="goto">
						<input type="hidden" name="step" value="3" />
						<input class="goto_next" type="submit" id="next" disabled="disabled" onfocus="this.blur();" value="下一步    " />
					</div>
				</form>
<?php
	insfooter();
}

function step_1(){
	insheader();
?>
				<form action="" method="get">
					<div class="part_head">欢迎</div>
					<div class="part_body">
						<div class="part_text">
							欢迎使用读吧书网，点击下一步开始安装。
						</div>
					</div>

					<div id="goto">
						<input type="hidden" name="step" value="2" />
						<input class="goto_next" type="submit" onfocus="this.blur();" value="下一步    " />
					</div>
				</form>
<?php
	insfooter();
}


function insheader(){
	global $version, $install_steps, $step, $install_step_description;
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml11.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
	<head>
		<title>读吧书网 <?php echo $version; ?> 安装程序</title>
		<link href="install.css" type="text/css" rel="stylesheet" />
	</head>

	<body>
		<div id="header"></div>

		<div id="body">
			<div id="left">
				<div class="navigation_title">安装步骤</div>

<?php
	foreach($install_steps as $k => $v) {
?>
				<div class="navigation_steps<?php if($k == $step) { ?> navigation_current<?php } ?>"><div id="step_<?php echo $k; ?>"></div><?php echo $v; ?></div>
<?php
		if($k == $step) {
?>
				<div id="step_description"><?php echo $install_step_description[$step]; ?></div>
<?php
		}
	}
?>
			</div>

			<div id="right">
				<div id="body_title"><div id="install_disk"></div><?php echo $install_steps[$step]; ?></div>

<?php
}


function insfooter(){
?>
			</div>
		</div>
	</body>
</html>
<?php
}


function ris_int($var){
	return is_numeric($var) && floor($var) == $var;
}


function check_php($getcolor = false){
	if(version_compare(PHP_VERSION, '4.3', '>')) {
		if(version_compare(PHP_VERSION, '5.2', '>')) {
			$color = 'Green';
		}else {
			$color = 'Gray';
		}
	}else {
		$color = 'Red';
	}
	if($getcolor) {
		return $color;
	}else {
		return $color != 'Red';
	}
}

function check_mysql($getcolor = false){
	if(extension_loaded('mysql')) {
		if(version_compare(mysql_get_client_info(), '3.23', '>=')) {
			if(version_compare(mysql_get_client_info(), '4.1', '>=')) {
				$color = 'Green';
			}else {
				$color = 'Gray';
			}
		}else {
			$color = 'Red';
		}
	}else {
		$color = 'Red';
	}
	if($getcolor) {
		return $color;
	}else {
		return $color != 'Red';
	}
}

function check_mbstring($getcolor = false){
	if(extension_loaded('mbstring')) {
		$color = 'Green';
	}else {
		$color = 'Red';
	}
	if($getcolor) {
		return $color;
	}else {
		return $color != 'Red';
	}
}

function check_allow_url_fopen($getcolor = false){
	if(ini_get('allow_url_fopen')) {
		$color = 'Green';
	}else {
		$color = 'Gray';
	}
	if($getcolor) {
		return $color;
	}else {
		return $color != 'Gray';
	}
}

function check_fsockopen($getcolor = false){
	if(function_exists('fsockopen')) {
		$color = 'Green';
	}else {
		$color = 'Gray';
	}
	if($getcolor) {
		return $color;
	}else {
		return $color != 'Gray';
	}
}

function check_curl($getcolor = false){
	if(extension_loaded('curl') && function_exists('curl_init') && function_exists('curl_setopt') && function_exists('curl_exec')) {
		$color = 'Green';
	}else {
		$color = 'Gray';
	}
	if($getcolor) {
		return $color;
	}else {
		return $color != 'Gray';
	}
}

function check_mkdir($getcolor = false){
	if(function_exists('mkdir')) {
		$color = 'Green';
	}else {
		$color = 'Gray';
	}
	if($getcolor) {
		return $color;
	}else {
		return $color != 'Gray';
	}
}

function check_chmod($getcolor = false){
	if(function_exists('chmod')) {
		$color = 'Green';
	}else {
		$color = 'Gray';
	}
	if($getcolor) {
		return $color;
	}else {
		return $color != 'Gray';
	}
}

function check_ini_set($getcolor = false){
	if(function_exists('ini_set')) {
		$color = 'Green';
	}else {
		$color = 'Gray';
	}
	if($getcolor) {
		return $color;
	}else {
		return $color != 'Gray';
	}
}

function check_safe_mode($getcolor = false){
	if(!ini_get('safe_mode')) {
		$color = 'Green';
	}else {
		$color = 'Gray';
	}
	if($getcolor) {
		return $color;
	}else {
		return $color != 'Gray';
	}
}

function check_set_time_limit($getcolor = false){
	if(!ini_get('safe_mode') && function_exists('set_time_limit')) {
		$color = 'Green';
	}else {
		$color = 'Gray';
	}
	if($getcolor) {
		return $color;
	}else {
		return $color != 'Gray';
	}
}

function check_perm($path, $getcolor = false){
	if(is_file('../'.$path)) {
		return check_perm_file('../'.$path, $getcolor);
	}elseif(is_dir('../'.$path)) {
		return check_perm_dir('../'.$path, $getcolor);
	}

	if($getcolor) {
		return 'Red';
	}else {
		return false;
	}
}

function check_perm_file($file, $getcolor = false){
	$color = 'Green';

	$fp = fopen($file, 'ab');
	if(false === $fp)
		$color = 'Gray';

	$length = fwrite($fp, ' ');
	if(false === $length)
		$color = 'Gray';

	fclose($fp);

	if($getcolor) {
		return $color;
	}else {
		return $color != 'Gray';
	}
}

function check_perm_dir($dir, $getcolor = false){
	if(!file_put_contents($dir.'/check.perm', 'check perm')) {
		$color = 'Red';
	}elseif(!unlink($dir.'/check.perm')) {
		$color = 'Red';
	}else {
		$color = 'Green';
	}

	if($getcolor) {
		return $color;
	}else {
		return $color != 'Red';
	}
}

function superaddslash($string){
	if(is_array($string)) {
		foreach($string as $k => $v) {
			$string[$k] = superaddslash($v);
		}
	}else {
		$string = addslashes($string);
	}

	return $string;
}

function html_show_crlf($string){
	if(is_array($string))
		foreach($string as $k => $v)
			$string[$k] = html_show_crlf($v);
	else
		$string = str_replace(array(' ', CR.LF, LF, HT), array('&nbsp;', '<br />', '<br />', '&nbsp;&nbsp;&nbsp;&nbsp;'), $string);

	return $string;
}
function html_show_crlf_decode($string){
	if(is_array($string))
		foreach($string as $k => $v)
			$string[$k] = html_show_crlf_decode($v);
	else
		$string = str_replace(array('&nbsp;&nbsp;&nbsp;&nbsp;', '<br />', '&nbsp;'), array(HT, LF, ' '), $string);

	return $string;
}
function html_show($string, $showcrlf = true){
	if(is_array($string))
		foreach($string as $k => $v)
			$string[$k] = html_show($v, $showcrlf);
	else
		$string = str_replace(array('&', '<', '>', '"', "'", '\\'), array('&amp;', '&lt;', '&gt;', '&quot;', '&#039;', '&#092;'), $string);

	if(!is_array($string) && $showcrlf)
		return html_show_crlf($string);

	return $string;
}
function html_show_decode($string){
	if(is_array($string))
		foreach($string as $k => $v)
			$string[$k] = html_show_decode($v);
	else
		$string = str_replace(array('&#092;', '&#039;', '&quot;', '&gt;', '&lt;', '&amp;'), array('\\', "'", '"', '>', '<', '&'), html_show_crlf_decode($string));

	return $string;
}

function get_post_var($var){
	global $$var;

	$$var = isset($_POST[$var]) ? $_POST[$var] : '';
}
