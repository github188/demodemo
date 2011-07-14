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
//����������



if(!defined('IN_Read8')) exit('Access Denied');

//////////////////////////////////////////////////////////////////////////////////////
//										�����б�									//
//						��������: �����������ô�д����ĸ����ʽ						//
//																					//


//------------------------����������------------------------------------------------//
//	rsubstr($fd,$start,$end)
//	csubstr($str,$start,$end)
//	cstrlen($string)	�ַ��������ж� ok
//	Page($total, $pagesize, $rewrite = false, $onlyoutput = true, $param = false, $styleMode = 0, $pvar = 'page', $current = false)	��ҳ���� ok
//	getad($position)							��ù������ ok
//	ipShow($ip)									ֻҪ��λip���� ok
//	readOfficial($methfile)						��ȡ�ٷ��������� ok
//	stopmark()									ѭ��ǿ��ֹͣ���� ok
//	listdir($path,$kind='file',$format='',$default='')	�ļ��б�������(kind= file Ϊ�ļ�, folder Ϊ�ļ���) ok
//	sizecount($filesize)						�ߴ������ ok
//	ReadURL($url)								Զ�̶�ȡ ok
//	writetofile($file_name,$data,$method='wb')	д���ļ� ok
//	PrintCodes($str,$kind=0)					�ı�ת������ʾ��ʽ ok
//	AddLog($logcon, $type = '', $logcon2 = '')	��־��¼���� ok
//	Banlist($str, $kind='word')					������ ok


//------------------------ʱ�亯����------------------------------------------------//
//	rdate($timestamp = false, $format = 'm-d')	date���� ok
//	rtodaytime($timestamp = false)				����ú��� ok

//------------------------��¼��غ���----------------------------------------------//
//	CheckLogin()			����Ƿ��¼ ok
//	CheckPass($name, $pwd)	��¼��� ok
//	ViewUserStatus()		��¼��Ϣ�鿴 ok

//------------------------COOKIE����------------------------------------------------//
//	GetCookie($name, $explode=false)			�õ�cookieֵ ok
//	Cookie($name, $value = '', $cookietime = 0)	cookie���溯�� ok
//	GetCookieName($name)						�õ�cookie���� ok

//------------------------Ȩ�޼��/��ȫ����-----------------------------------------//
//	HashCheck($hash)	��֤���� ok
//	IsGroup($groupid=false)	�û����� ok
//	HasPriv($privbit)	ǰ̨Ȩ�޼�� ok
//	VipPriv()			�Ƿ�ɷ���vip��Ʒ ok
//	IpAccess($ial, $allow = true, $explode = "\n")	�Ƿ�������ʵ�ip�� ok

//------------------------���ݺ���&��ǿ������---------------------------------------//
//	rrmdir($path, $del = 1)					ɾ���ļ��к��� ok
//	rrand($len, $type = 7, $limit = false)	����� ok
//	chtmlspecialchars($string)				htmlspecialchars���� ok
//	cjsspecialchars($string)				js����
//	caddslashes($string)					ת�庯��: ��ע�� ok
//	cstripslashes($string)					��ת�庯��: ���л�ǰ ok
//	convert($text, $from, $to)				����ת������ ok
//	var_export���� ok
//	ob_get_clean���� ok
//	file_get_contents���� ok
//	file_put_contents���� ok

//------------------------���԰�����------------------------------------------------//
//	Lang($msg, $prefix = '')			 ok
//	GetLang($istpl = false)				 ok
//	LanguagePack($tpl = false, $style = 'basic', $external = false)		 ok

//------------------------��ʾҳ�溯��----------------------------------------------//
//	back($msg, $external = 3)	������ʾ���� ok
//	jumpage($URL,$title)		��תҳ�溯�� ok
//	alert($con, $refresh=0)		������� ok

//////////////////////////////////
//            sn����            //
//////////////////////////////////
//	sn($module = false, $sn = false)	������кź��� ok
//	sn_decode($sn)						�������к� ok
//	getArrayCols($char,$n)				�ַ���ӳ����� ok
//	sn_check($domain, $module, $csn)	������к��Ƿ���� ok
//																					//
//////////////////////////////////////////////////////////////////////////////////////


//--------------------------------��׼����----------------------------------


function _login($forward = ''){
	global $_SYSTEM;

	header('Location: '.$_SYSTEM['SYSTEM']['SITE_ADDR'].'/login.php?forward='.rawurlencode($_SYSTEM['SYSTEM']['SITE_ADDR'].$forward));
	exit;
}

/*function csubstr($str,$len,$tell=1){
	$str = mb_convert_encoding($str,'GBK','utf-8');
	$maxlen=0;
	for($i=0; $i<$len; $i++){//i�ǵڼ�����
		if(strlen(mb_substr($str,$i,1,'GBK'))>1) {
			$maxlen+=2;
		}else {
			$maxlen++;
		}
		if($maxlen>$len) break;
	}
	$laststr=mb_substr($str,0,$i,'GBK');
	if($laststr<>$str && $tell==1) $laststr.="...";
	return mb_convert_encoding($laststr,'utf-8','GBK');
}*/
function csubstr($str,$start,$end){
	global $Global;
	$start = floor($start/2);
	$end = floor($end/2);
	return mb_substr($str,$start,$end,SYSCHARSET);
}


//��ҳ���� ok
function Page($total, $pagesize, $rewrite = false, $onlyoutput = true, $param = false, $styleMode = 0, $pvar = 'page', $current = false){
	include_once ROOT.'include/Page.cls.php' ;
	if(empty($pagesize) || $pagesize<1) $pagesize=20;
	$obj = new Page($total, $pagesize, $current,$pvar);
	$obj->rewrite = $rewrite ;
	$obj->styleMode = $styleMode ;
	if(is_array($param) && !empty($param)) $obj->setVar($param);

	if($onlyoutput) return $obj->output(1);
	$output = array();
	$output['output'] = $obj->output(1);
	$output['limit'] = $obj->limit();
	$output['between'] = $obj->between();
	return $output ;
}

//��ù������ ok
function getad($position){
	return $GLOBALS['_CACHE']['adverts'][$position];
}

//ֻҪ��λip���� ok
function ipShow($ip){
	$ip = explode('.', $ip);
	$ip[3] = '*';
	return implode('.', $ip);
}

//��ȡ�ٷ��������� ok
function readOfficial($methfile){
	global $Global;
	$url = "http://www.read8.net/$methfile&domain=".urlencode($_SYSTEM['SYSTEM']['SITE_ADDR']);
	return rforr($url);
}

//ѭ��ǿ��ֹͣ���� ok
function stopmark(){
	$file=ROOT.'data/cache/stop.txt';
	if(file_exists($file)) {
		if(time()-filemtime($file)>60) unlink($file);
		else exit;
	}
}


//��־��¼���� ok
function AddLog($logcon, $type = '', $logcon2 = ''){
	global $Global,$db;

	$typeArr=array("password", "money", "group", "action", "access", "systemerror", "sql" );
	if(defined('IN_Read8_ACP') && !$type) $type="action";//��̨Ĭ��Ϊaction
	if(!in_array($type,$typeArr)) $type="other";//����Ĭ��

	$logcon = Lang($logcon, 'log_');

	switch($type) {
		case 'password':
		case 'action':
		case 'sql':
			$logcon .= $Global['ImplodeChar'].$logcon2;
		default :
			$logcon = $Global['F_username'].$Global['ImplodeChar'].$logcon.$Global['ImplodeChar'].TIMESTAMP.$Global['ImplodeChar'].$Global['F_userip'];
			if(!is_file(ROOT."data/log/$type.log.php"))
				if(!rfow(ROOT."data/log/$type.log.php", "<?php exit('Access Denied'); ?>\n"))
					exit('Can\'t Create Log File, Check Write Permission (Floder)./data/log/');

			if(!rfoa(ROOT."data/log/$type.log.php", $logcon."\n"))
				exit('Can\'t Write Log, Check Write Permission (File),.data/log/'.$type.'.log.php');
			break;
	}
}

//������ ok
function Banlist($str, $kind='word'){
	if(!$str || empty($GLOBALS['_CACHE']['banlist'][$kind]) || !is_array($GLOBALS['_CACHE']['banlist'][$kind])) Return false;
	foreach($GLOBALS['_CACHE']['banlist'][$kind] as $val){
		if($kind=='word' && strstr($str,$val)) Return true;
		if($kind!='word' && $str==$val) Return true;
	}
	Return false;
}


//--------------------------------��¼��غ���----------------------------------

//����Ƿ��¼ ok
function CheckLogin($return = false){
	global $Global;
	if($return) return $Global['F_uid'];
	if(!$Global['F_uid']) back('system_login_expired',7);
}

//��¼��� ok
function CheckPass($name, $pwd){
	global $db;

	if(!$row = $db->selectOne(array(
		'from'  => __TAB_USER__,
		'where' => array(
			'username' => $name,
			'active'   => 1,
		),
	))) {
		$userarr = 'user_not_exists';
	}else {
		if($row['pwd'] != $pwd) {
			$userarr = 'login_password_error';
		}else {
			$userarr = array(
				'uid'      => $row['uid'],
				'username' => $row['username'],
				'email'    => $row['email'],
			);

			$userarr = caddslashes($userarr);
		}
	}

	return $userarr;
}

//��¼��Ϣ�鿴 ok
function ViewUserStatus(){
	$tpl = new AOKOTpl('login');
	$tpl->assign(array(
		'action' => 'viewstatus',
		'forward' => $GLOBALS['forward'],
		'C_loghashenable' => $GLOBALS['Global']['hashto']['login'],
	));
	return $tpl->output(1);
}


//--------------------------------COOKIE����----------------------------------

//�õ�cookieֵ ok
function _cookie_decode($name, $explode = false){
	$name = base64_encode($name);
	if(!empty($_COOKIE[GetCookieName($name)]) && !empty($GLOBALS['_SYSTEM']['SAFE']['code']) && $GLOBALS['Global']['F_userip']) {
		$cookieinfo = unserialize(base64_decode(stripslashes($_COOKIE[GetCookieName($name)])));
		if(md5($name.$cookieinfo['str'].$GLOBALS['_SYSTEM']['SAFE']['code'].ipShow($GLOBALS['Global']['F_userip'])) == $cookieinfo['hash']) {
			if($explode) return explode("|||", $cookieinfo['str']);//��������
			return $cookieinfo['str'];
		}
	}
	return '';
}

//cookie���溯�� ok
function _cookie_encode($name, $value = '', $cookietime = 0){
	if($cookietime == 0) $cookietime = -1000;
	$name = base64_encode($name);
	if(!empty($GLOBALS['_SYSTEM']['SAFE']['code']) && !empty($GLOBALS['Global']['F_userip']) && $cookietime > 0 && !empty($value)) {
		if(is_array($value)) $value = implode("|||", $value);//����Ҳ�ɴ���cookie
		$hash = md5($name.$value.$GLOBALS['_SYSTEM']['SAFE']['code'].ipShow($GLOBALS['Global']['F_userip']));
		$arr = array('str' => $value, 'hash' => $hash);
		setcookie(GetCookieName($name), base64_encode(serialize($arr)), TIMESTAMP + $cookietime);
	}else {
		setcookie(GetCookieName($name), '', TIMESTAMP + $cookietime);
	}
}

//�õ�cookie���� ok
function GetCookieName($name){
	return $GLOBALS['_SYSTEM']['SAFE']['prefix'].substr(md5($GLOBALS['_SYSTEM']['SAFE']['prefix'].$name), 0, 20);
}


function cookie($name, $value = '', $expire = 0, $path = '/', $domain = '', $secure = false, $httponly = false){
	global $_SYSTEM;

	$name = $_SYSTEM['SAFE']['prefix'].$name;
	if($expire > 0) $expire = TIMESTAMP + $expire;

	//if(!$path) $path = $_SYSTEM['COOKIE_PATH'];
	//if(!$domain) $domain = $_SYSTEM['COOKIE_DOMAIN'];
	setcookie($name, $value, $expire, $path);
}
function _cookie($name){
	global $_SYSTEM;

	return isset($_COOKIE[$_SYSTEM['SAFE']['prefix'].$name]) ? $_COOKIE[$_SYSTEM['SAFE']['prefix'].$name] : false;
}


//-----------------------------Ȩ�޼��/��ȫ����----------------------------





//ǰ̨Ȩ�޼�� ok

//�Ƿ�ɷ���vip��Ʒ ok
function VipPriv(){
	return isGroup($GLOBALS['Global']['Writergroupid']) || isGroup($GLOBALS['Global']['Writergroups']);
}

//�Ƿ�������ʵ�ip�� ok
function ipAccess($ial, $allow = true, $explode = "\n"){//ial(ip access list) as array, allow in allow,deny / deny,allow
	$uip = explode('.', $GLOBALS['Global']['F_userip']);
	if(empty($uip) || empty($uip[0]) || empty($uip[1]) || empty($uip[2]) || empty($uip[3])) return false;

	if(!is_array($ial)) $ial = explode($explode, $ial);
	foreach($ial as $key => $ip)
		$ial[$key] = explode('.', $ip);

	foreach($ial as $key => $ip)
		for($i = 0; $i <= 3; $i++)
			if(empty($ip[$i])) $ial[$key][$i] = $uip[$i];

	$result = $allow;
	foreach($ial as $ip) {
		if($allow) {
			$result = $result && ($ip == $uip ? !$allow : $allow);
		}else {
			$result = $result || ($ip == $uip ? !$allow : $allow);
		}
	}
	return $result;
}


//----------------------------���ݺ���&��ǿ������-----------------------------

//jsת��
function cjsspecialchars($string){
	if(is_array($string)) foreach($string as $key => $val) $string[$key] = chtmlspecialchars($val);
	else $string = str_replace(array('&', '<', '>', '"', "'", ' ', "\r\n", "\n", "\t"), array('&amp;', '&lt;', '&gt;', '&quot;', '\\\'', '&nbsp;', '<br />', '<br />', '&nbsp;&nbsp;&nbsp;&nbsp;'), $string);
	return $string;
}

//ת�庯��: ��ע�� ok
function caddslashes($string){
	if(is_array($string)) foreach($string as $key => $val) $string[$key] = caddslashes($val);
	else $string = addslashes($string);
	return $string;
}

//��ת�庯��: ���л�ǰ ok
function cstripslashes($string) {
	if(is_array($string)) foreach($string as $key => $val) $string[$key] = cstripslashes($val);
	else $string = stripslashes($string);
	return $string;
}

//����ת������ ok

//var_export���� ok
if(!function_exists('var_export')) {
	function var_export($array, $return = false){
		if($return) return arrayeval($array);
		else print_r($array);
	}
	function arrayeval($array, $level = 0){
		for($i = 0; $i <= $level; $i++) $space .= "\t";
		$evaluate = "Array\n$space(\n";
		$comma = "$space";
		foreach($array as $key => $val) {
			$key = is_string($key) ? "'".addcslashes($key, '\'\\')."'" : $key;
			$val = is_string($val) ? "'".addcslashes($val, '\'\\')."'" : $val;
			if(is_array($val)) $evaluate .= "$comma$key => ".arrayeval($val, $level + 1);
			else $evaluate .= "$comma$key => $val";
			$comma = ",\n$space";
		}
		$evaluate .= "\n$space)";
		return $evaluate;
	}
}

//ob_get_clean���� ok
if(!function_exists('ob_get_clean')) {
	function ob_get_clean(){
		$str = ob_get_contents();
		ob_end_clean();
		return $str;
	}
}


//------------------------���԰�����----------------------------- ok
function Lang($msg, $prefix = ''){
	/*if(empty($GLOBALS['Lang'])) */$Lang = array_merge(GetLang(), GetLang(1));
	//else global $Lang;
 	return isset($Lang[$prefix.$msg]) ? $Lang[$prefix.$msg] : $msg;
}
function GetLang($istpl = false){
	$Lang = LanguagePack($istpl);
	$style = empty($GLOBALS['Global']['oldstyle']) ? $GLOBALS['Global']['style'] : $GLOBALS['Global']['oldstyle'];
	if($style != 'basic') $Lang = array_merge($Lang, LanguagePack($istpl, $style, false));
	unset($style);
	return $Lang;
}
function LanguagePack($tpl = false, $style = 'basic', $external = false){
	$langfile = ROOT.'lang/'.$style.'.chs'.($tpl ? '.tpl' : '.msg').($external ? '.ext' : '').'.php';
	if(is_file($langfile)) {
		include $langfile;
	}else $Lang = array();
	if($external) return $Lang;
	else return array_merge($Lang, LanguagePack($tpl, $style, true));
}

//------------------------��ʾҳ�溯��-----------------------------

//������ʾ���� ok
function back($msg, $external = 3){//1 - header 2 - back 4 - login
	global $Global, $Lang, $_SYSTEM;

	$Lang = array();
	$msg = Lang($msg);

	$external = sprintf("%03b",$external);
	$header = $external[2] ? (defined('IN_Read8_ACP') ? 0 : 1) : 0;
	if($header) _header(array('error'));
	if(!empty($Global['oldstyle'])) $Global['style'] = $Global['oldstyle'];

	$forward = explode('/', substr($_SYSTEM['SYSTEM']['SITE_ADDR'], 7));
	$forward = 'http://'.$forward[0].$_SERVER['REQUEST_URI'];
	//��ʾ
	$tpl = new AOKOTpl('error');
	$tpl->assign(array(
		'forward' => $external[1] ? urlencode($forward) : '',
		'login' => $external[0] ? 'login' : '',
		'C_loghashenable' => _secode(1),
		'msg' => $msg,
	));
	$tpl->output();
	if($header) _footer();
	exit;
}

//��תҳ�溯�� ok
function jumpage($URL,$title){
	global $Global;
	//if(!empty($Global['oldstyle'])) $Global['style'] = $Global['oldstyle'];

	$title = Lang($title);

	$tpl = new AOKOTpl('jumpage');
	$tpl->assign(array(
		'URL' => $URL,
		'title' => $title,
	));
	$tpl->output();
	exit;
}

//������� ok
function alert($con, $refresh=0) {
	$con = Lang($con);

	$output = 'alert("'.str_replace('"','&quot;',$con).'");';
	if($refresh == 1) $output = 'parent.window.refresh();';
	elseif($refresh == -1) $output = 'window.close();';
	elseif($refresh) $output = "parent.window.location='$refresh';";

	exit('<script type="text/javascript">'.$output.'</script>');
}

//////////////////////////////////
//            sn����            //
//////////////////////////////////
function sn($module = false, $sn = false){
	global $Global, $_Modulecfg, $_SYSTEM;
	if(false === $module) {
		$module = 'footer';
		$sn = empty($Global['sitefootercode']) ? '' : $Global['sitefootercode'];
	}elseif(false === $sn) $sn = empty($_Modulecfg[$module]['sn']) ? '' : $_Modulecfg[$module]['sn'];
	return sn_check($_SYSTEM['SYSTEM']['SITE_ADDR'], $module, sn_decode($sn));
}

function sn_decode($sn){
	$chars = '0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ';
	$sn = strtoupper(str_replace('-', '', $sn));
	$sn_len = strlen($sn);
	$key = '';
	for($i = 0; $i < $sn_len; $i++) {
		$key .= strtolower($chars[getArrayCols($sn[$i], $i)]);
	}
	return $key;
}

function getArrayCols($char, $n){
	$chars = '0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ';
	$chars_len = strlen($chars);
	for($i = 0; $i < $chars_len; $i++) {
		if($chars[$i] == $char) {
			$rn = $i;
			break;
		}
	}
	$rn -= $n;
	if($rn < 0) $rn += $chars_len;
	return $rn;
}

function sn_check($domain, $module, $csn){
	if(!$csn) return false;
	if(!strstr($domain, 'http://'.$_SERVER['HTTP_HOST']) && !strstr($domain, 'http://www.'.$_SERVER['HTTP_HOST'])) return false;
	$hash = md5($module).'Read8'.md5($domain);
	$expired = hexdec(substr($csn, 17)) / 2;
	$chash = substr(md5($hash.$expired), 15);
	if($chash == substr($csn, 0, 17)) {
		if(time() < $expired) return true;
		else return false;
	}
	return false;
}