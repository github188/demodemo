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
if(!defined('IN_Read8') || !defined('IN_Read8_ACP') || !defined('IN_Read8_ACP_STYLE') || !isset($PHP_SELF)) {
	exit('Access Denied (style_choose.php)');
}


$style_name = $db->selectOne(array(
	'field' =>'value',
	'from'  =>__TAB_SYSTEM__,
	'where' => array(
		'config'	=>	'DISPLAY',
		'`index`'		=>	'template',
	),
));

	$TemplateInfo=_data_import(rfor(ROOT.'template/'.$style_name['value'].'/_template_data.php'));
	if(!is_file(ROOT.'template/'.$style_name['value'].'/'.$TemplateInfo['pic']))
		$TemplateInfo['pic'] = '../basic/no_thumb.png';


if($action == 'export') {
	$TemplateInfo['cache'] = $db->select(array(
		'from'  => __TAB_CACHE__,
	));
	_data_export($TemplateInfo);
}elseif($action == 'change') {
	if(false !== strpos($id, '..'))
		back('非法操作');

	$f = ROOT.'template/'.$id.'/_template_data.php';
	if(!is_file($f))
		back('模板信息不存在');

	$tpldata = _data_import(rfor($f));
	if(!is_array($tpldata))
		back('模板信息非法');

	$db->update(array(
		'value' => $id,
	), array(
		'config' => 'DISPLAY',
		'`index`' => 'template',
	), __TAB_SYSTEM__);
	_config_update('display');
	$db->execUpBySql('TRUNCATE TABLE '.__TAB_CACHE__);
	foreach($tpldata['cache'] as $vc) {
		$db->insert($vc, __TAB_CACHE__);
	}
	jumpage("?ptype=$ptype&pfile=$pfile", '启用成功');
}
	/*array(
		'title'		=>	'默认风格',
		'author'	=>	'澳柯网信科技发展有限公司',
		'pic'		=>	'thumb.gif',
		'info'		=>	'有aokosft提供读吧官方，，。，。的默认风格',
	)*/
$errArr=array(
	1 => '分割格式错误',
	2 => '不存在rem或import的数据',
	3 => '不存在版本信息',
	4 => '该规则数据不适用于当前版本',
	5 => '所含数据内容类型错误',
	);

	$TplDir=rlistdir(ROOT.'template/');
	$activeTpl=array();//可用模板信息
	$errTplArr=array();//不可用模板信息

	foreach($TplDir as $k=>$v2){
		$v=ROOT.'template/'.$v2.'/_template_data.php';
		if(!is_file($v)) {
			 $errTplArr[]=array('title'=>$v2);
			continue;
		}
		$TplInfo=_data_import(rfor($v));
		if(is_array($TplInfo)) {
			if(!is_file(ROOT.'template/'.$v2.'/'.$TplInfo['pic'])) {
				$TplInfo['pic'] = '../basic/no_thumb.png';
			}
			$TplInfo['tplname']=$v2;
			$activeTpl[]=html_show($TplInfo);
		}else {
			 $errTplArr[]=array('title'=>$v2);
		}

	}



	$tpl = new AOKOTpl($pfile);
	$tpl->assign($TemplateInfo);
	$tpl->assign('tplname',$style_name['value']);
	$tpl->assign('tplArr', $activeTpl);
	$tpl->assign('errTplArr', $errTplArr);
	$tpl->output();




function _data_export($rule){
	global $_SYSTEM;

	$data = '<'.'?php exit; ?'.'>'.LF.
			'# AOKOSOFT Read8.Net '.SYS_VERSION.' '.SYS_BUILD.' '.LF.
			'# Identify:    '.base64_encode(SYS_VERSION).LF.
			'# From:        '.$_SYSTEM['SYSTEM']['SITE_ADDR'].LF.
			'# Last Update: '.rdate($rule['lastupdate']).LF.
			'# Export Date: '.rdate(TIMESTAMP).LF.
			'#'.LF.
			'# http://www.read8.net'.LF.
			'# Please visit our website for the latest information'.LF.
			'# ----------------------------------------------------------'.LF.
			LF.
			LF.
			LF.
			wordwrap(base64_encode(serialize($rule)), 80, LF, true);

	ob_end_clean();
	header('Cache-Control: no-cache, must-revalidate');
	header('Pragma: no-cache');
	header('Content-Encoding: none');
	header('Content-Length: '.strlen($data));
	header('Content-Disposition: attachment; filename='.$style_name['value'].'_'.rdate(TIMESTAMP, 'Y_m_d_H_i_s').'.php');
	header('Content-Type: '.(strpos($_SERVER['HTTP_USER_AGENT'], 'MSIE') ? 'application/octetstream' : 'application/octet-stream'));
	echo $data;
	exit;
}

function _data_import($data, $complex = false){
	list($rem, $import) = explode(LF.LF.LF.LF, str_replace(CR, '', $data));

	$rems = explode(LF, trim($rem));
	$i_ver = base64_decode(substr($rems[2], 15));

	if($i_ver != SYS_VERSION)
		return false;

	$rule = unserialize(base64_decode($import));
	if(!is_array($rule))
		return false;

	return caddslashes($rule);
}

