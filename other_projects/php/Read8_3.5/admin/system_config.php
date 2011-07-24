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
if(!defined('IN_Read8') || !defined('IN_Read8_ACP') || !defined('IN_Read8_ACP_SYSTEM') || !isset($PHP_SELF)) {
	exit('Access Denied (system_config.php)');
}

$config = strtoupper($ps);

echo "---------$config--------------";

if(!in_array($config, array('SYSTEM', 'USER', 'AUTHOR', 'STORY', 'DISPLAY', 'READ', 'COMMENT', 'UPLOAD', 'FOPEN', 'SEO', 'SAFE', 'INFO', 'RANK', 'OFFICIAL', 'PAYMONTH', 'PAY', 'VIP', 'MOBILE'))) {
	back('未知分类');
}

$db->single(__TAB_SYSTEM__);

if($action == 'update') {
	$special = array(
		'safe' => array(
			'secode',
			'chmod_dir',
			'chmod_file',
		),
		'official' => array(
			'password',
		),
		'mobile' => array(
			'password',
		),
		'upload' => array(
			'format',
		),
	);

	$rows = $db->select(array(
		'field' => '`index`',
		'where' => array('config' => $config),
	));
	foreach($rows as $row) {
		if(!isset(${'p'.$row['index']}))
			continue;

		if(isset($special[$ps]) && in_array($row['index'], $special[$ps]))
			eval('${\'p\'.$row[\'index\']} = _system_config_save_'.$ps.'_'.$row['index'].'(${\'p\'.$row[\'index\']});');

		$db->update(array(
			'value'   => ${'p'.$row['index']},
		), array(
			'config'  => $config,
			'`index`' => $row['index'],
		));
	}
	_config_update($ps);
	jumpage("?ptype=$ptype&pfile=$pfile", '修改成功');
}


$tpl = new AOKOTpl('system_config_'.$ps);

$rows = $db->select(array(
	'field' => '`index`, value',
	'where' => array('config' => $config),
));
foreach($rows as $row)
	$cfg[$row['index']] = $row['value'];

$cfg = html_show($cfg, false);

$radios  = array(
	'system'  => array(
		'open',
	),
	'user'    => array(
		'open',
		'verify',
	),
	'story'   => array(
		'open',
		'verify',
	),
	'author'  => array(
		'verify',
	),
	'display' => array(
		'speedtrace',
	),
	'read'    => array(
		'fulltext',
		'fulltext_guest',
		'download_package',
		'download_package_guest',
		'download_text',
		'download_text_guest',
	),
	'comment' => array(
		'open',
		'chapter',
		'guest',
	),
	'upload'  => array(
		'open',
		'watermark_type',
	),
	'mobile'  => array(
		'novel_update',
		'novel_comment',
	),
);

$special = array(
	'system'  => array(
		'TIME_ZONE',
	),
	'author'  => array(
		'size',
	),
	'display' => array(
		'template',
	),
	'upload'  => array(
		'size',
		'format',
		'watermark',
		'watermark_color',
	),
	'fopen'   => array(
		'emulate',
		'curl',
	),
	'seo'     => array(
		'htmlsubdir',
	),
	'safe'    => array(
		'secode',
		'chmod_dir',
		'chmod_file',
	),
	'info'    => array(
	),
	'vip'     => array(
		'background',
		'color',
		'mark_color',
	),
);

foreach($cfg as $k => $v) {
	if(isset($radios[$ps]) && in_array($k, $radios[$ps])) {
		$tpl->assign(array(
			'C_'.$k.'_'.($v ? 'yes' : 'no') => ' checked="checked"',
			'C_'.$k.'_'.($v ? 'no' : 'yes') => '',
		));
	}elseif(isset($special[$ps]) && (in_array($k, $special[$ps]) || empty($special[$ps]))) {
		if(empty($special[$ps])) {
			eval('_system_config_'.$ps.'_($k, $v);');
		}elseif(in_array($k, $special[$ps])) {
			eval('_system_config_'.$ps.'_'.$k.'($k, $v);');
		}
	}else {
		$tpl->assign('C_'.$k, $v);
	}
}

$tpl->output();



function _system_config_system_TIME_ZONE($k, $v){
	global $tpl;

	$timezone = array(
		array('t' => 'GMT-12:00', 'v' => '-12',   'selected' => '-12'   == $v ? ' selected="selected"' : ''),
		array('t' => 'GMT-11:00', 'v' => '-11',   'selected' => '-11'   == $v ? ' selected="selected"' : ''),
		array('t' => 'GMT-10:00', 'v' => '-10',   'selected' => '-10'   == $v ? ' selected="selected"' : ''),
		array('t' => 'GMT-09:00', 'v' => '-9',    'selected' => '-9'    == $v ? ' selected="selected"' : ''),
		array('t' => 'GMT-08:00', 'v' => '-8',    'selected' => '-8'    == $v ? ' selected="selected"' : ''),
		array('t' => 'GMT-07:00', 'v' => '-7',    'selected' => '-7'    == $v ? ' selected="selected"' : ''),
		array('t' => 'GMT-06:00', 'v' => '-6',    'selected' => '-6'    == $v ? ' selected="selected"' : ''),
		array('t' => 'GMT-05:00', 'v' => '-5',    'selected' => '-5'    == $v ? ' selected="selected"' : ''),
		array('t' => 'GMT-04:00', 'v' => '-4',    'selected' => '-4'    == $v ? ' selected="selected"' : ''),
		array('t' => 'GMT-03:30', 'v' => '-3.5',  'selected' => '-3.5'  == $v ? ' selected="selected"' : ''),
		array('t' => 'GMT-03:00', 'v' => '-3',    'selected' => '-3'    == $v ? ' selected="selected"' : ''),
		array('t' => 'GMT-02:00', 'v' => '-2',    'selected' => '-2'    == $v ? ' selected="selected"' : ''),
		array('t' => 'GMT-01:00', 'v' => '-1',    'selected' => '-1'    == $v ? ' selected="selected"' : ''),
		array('t' => 'GMT',       'v' => '0',     'selected' => '0'     == $v ? ' selected="selected"' : ''),
		array('t' => 'GMT+01:00', 'v' => '+1',    'selected' => '+1'    == $v ? ' selected="selected"' : ''),
		array('t' => 'GMT+02:00', 'v' => '+2',    'selected' => '+2'    == $v ? ' selected="selected"' : ''),
		array('t' => 'GMT+03:00', 'v' => '+3',    'selected' => '+3'    == $v ? ' selected="selected"' : ''),
		array('t' => 'GMT+03:30', 'v' => '+3.5',  'selected' => '+3.5'  == $v ? ' selected="selected"' : ''),
		array('t' => 'GMT+04:00', 'v' => '+4',    'selected' => '+4'    == $v ? ' selected="selected"' : ''),
		array('t' => 'GMT+04:30', 'v' => '+4.5',  'selected' => '+4.5'  == $v ? ' selected="selected"' : ''),
		array('t' => 'GMT+05:00', 'v' => '+5',    'selected' => '+5'    == $v ? ' selected="selected"' : ''),
		array('t' => 'GMT+05:30', 'v' => '+5.5',  'selected' => '+5.5'  == $v ? ' selected="selected"' : ''),
		array('t' => 'GMT+05:45', 'v' => '+5.75', 'selected' => '+5.75' == $v ? ' selected="selected"' : ''),
		array('t' => 'GMT+06:00', 'v' => '+6',    'selected' => '+6'    == $v ? ' selected="selected"' : ''),
		array('t' => 'GMT+06:30', 'v' => '+6.5',  'selected' => '+6.5'  == $v ? ' selected="selected"' : ''),
		array('t' => 'GMT+07:00', 'v' => '+7',    'selected' => '+7'    == $v ? ' selected="selected"' : ''),
		array('t' => 'GMT+08:00', 'v' => '+8',    'selected' => '+8'    == $v ? ' selected="selected"' : ''),
		array('t' => 'GMT+09:00', 'v' => '+9',    'selected' => '+9'    == $v ? ' selected="selected"' : ''),
		array('t' => 'GMT+09:30', 'v' => '+9.5',  'selected' => '+9.5'  == $v ? ' selected="selected"' : ''),
		array('t' => 'GMT+10:00', 'v' => '+10',   'selected' => '+10'   == $v ? ' selected="selected"' : ''),
		array('t' => 'GMT+11:00', 'v' => '+11',   'selected' => '+11'   == $v ? ' selected="selected"' : ''),
		array('t' => 'GMT+12:00', 'v' => '+12',   'selected' => '+12'   == $v ? ' selected="selected"' : ''),
		array('t' => 'GMT+13:00', 'v' => '+13',   'selected' => '+13'   == $v ? ' selected="selected"' : ''),
	);

	$tpl->assign(array(
		'timezone'  => $timezone,
		'time_zone' => $v,
	));
}


function _system_config_author_size($k, $v){
	global $tpl;

	$tpl->assign(array(
		'C_'.$k               => $v,
		'size'                => array(
			array('v' => $v,        't' => '恢复'),
			array('v' => '20480',   't' => '20 K'),
			array('v' => '51200',   't' => '50 K'),
			array('v' => '102400',  't' => '100 K'),
			array('v' => '204800',  't' => '200 K'),
			array('v' => '512000',  't' => '500 K'),
			array('v' => '1048576', 't' => '1 M'),
			array('v' => '2097152', 't' => '2 M'),
		),
		'upload_max_filesize' => ini_get('upload_max_filesize'),
	));
}



function _system_config_display_template($k, $v){
	global $tpl;

	$templates = array();
	foreach(rlistdir(ROOT.'template') as $template) {
		$templates[] = array(
			'v'        => $template,
			't'        => $template,
			'selected' => $template == $v ? ' selected="selected"' : '',
		);
	}

	$tpl->assign('templates', $templates);
}



function _system_config_upload_size($k, $v){
	global $tpl;

	$tpl->assign(array(
		'C_'.$k               => $v,
		'size'                => array(
			array('v' => $v,        't' => '恢复'),
			array('v' => '20480',   't' => '20 K'),
			array('v' => '51200',   't' => '50 K'),
			array('v' => '102400',  't' => '100 K'),
			array('v' => '204800',  't' => '200 K'),
			array('v' => '512000',  't' => '500 K'),
			array('v' => '1048576', 't' => '1 M'),
			array('v' => '2097152', 't' => '2 M'),
		),
		'upload_max_filesize' => ini_get('upload_max_filesize'),
	));
}

function _system_config_upload_format($k, $v){// 1 - png(3)  2 - gif(2)  4 - jpeg(1)  8 - bmp(0)
	global $tpl;

	$formats = sprintf('%04b', $v);

	for($i = 0; $i < 4; $i++) {
		$tpl->assign('C_'.$k.'_'.(4 - $i), $formats[$i] ? ' checked="checked"' : '');
	}
}

function _system_config_upload_watermark($k, $v){
	global $tpl;

	for($i = 0; $i < 10; $i++) {
		$tpl->assign('C_'.$k.'_'.$i, $v == $i ? ' checked="checked"' : '');
	}
}

function _system_config_upload_watermark_color($k, $v){
	global $tpl;

	$tpl->assign(array(
		'C_'.$k => $v,
		'color' => array(
			array('C_color' => '255 0 0',     'C_hexcolor' => '#FF0000', 'C_colorname' => '红色',),
			array('C_color' => '0 255 0',     'C_hexcolor' => '#00FF00', 'C_colorname' => '绿色',),
			array('C_color' => '0 0 255',     'C_hexcolor' => '#0000FF', 'C_colorname' => '蓝色',),
			array('C_color' => '255 0 255',   'C_hexcolor' => '#FF00FF', 'C_colorname' => '牡丹红',),
			array('C_color' => '0 255 255',   'C_hexcolor' => '#00FFFF', 'C_colorname' => '青色',),
			array('C_color' => '255 255 0',   'C_hexcolor' => '#FFFF00', 'C_colorname' => '黃色',),
			array('C_color' => '112 219 147', 'C_hexcolor' => '#70DB93', 'C_colorname' => '海蓝',),
			array('C_color' => '255 255 255', 'C_hexcolor' => '#FFFFFF', 'C_colorname' => '白色',),
			array('C_color' => '0 0 0',       'C_hexcolor' => '#000000', 'C_colorname' => '黑色',),
			array('C_color' => '92 51 23',    'C_hexcolor' => '#5C3317', 'C_colorname' => '巧克力色',),
			array('C_color' => '159 95 159',  'C_hexcolor' => '#9F5F9F', 'C_colorname' => '蓝紫色',),
			array('C_color' => '181 166 66',  'C_hexcolor' => '#B5A642', 'C_colorname' => '黃铜色',),
			array('C_color' => '217 217 25',  'C_hexcolor' => '#D9D919', 'C_colorname' => '亮金色',),
			array('C_color' => '166 125 61',  'C_hexcolor' => '#A67D3D', 'C_colorname' => '棕色',),
			array('C_color' => '140 120 83',  'C_hexcolor' => '#8C7853', 'C_colorname' => '青铜色',),
			array('C_color' => '166 125 61',  'C_hexcolor' => '#A67D3D', 'C_colorname' => '青铜色二',),
			array('C_color' => '95 159 159',  'C_hexcolor' => '#5F9F9F', 'C_colorname' => '士官服蓝色',),
			array('C_color' => '217 135 25',  'C_hexcolor' => '#D98719', 'C_colorname' => '冷铜色',),
			array('C_color' => '184 115 51',  'C_hexcolor' => '#B87333', 'C_colorname' => '铜色',),
			array('C_color' => '255 127 0',   'C_hexcolor' => '#FF7F00', 'C_colorname' => '珊瑚红',),
			array('C_color' => '66 66 111',   'C_hexcolor' => '#42426F', 'C_colorname' => '紫蓝色',),
			array('C_color' => '92 64 51',    'C_hexcolor' => '#5C4033', 'C_colorname' => '深棕',),
			array('C_color' => '47 79 47',    'C_hexcolor' => '#2F4F2F', 'C_colorname' => '深绿',),
			array('C_color' => '74 118 110',  'C_hexcolor' => '#4A766E', 'C_colorname' => '深铜绿色',),
			array('C_color' => '79 79 47',    'C_hexcolor' => '#4F4F2F', 'C_colorname' => '深橄榄绿',),
			array('C_color' => '153 50 205',  'C_hexcolor' => '#9932CD', 'C_colorname' => '深兰花色',),
			array('C_color' => '135 31 120',  'C_hexcolor' => '#871F78', 'C_colorname' => '深紫色',),
			array('C_color' => '107 35 142',  'C_hexcolor' => '#6B238E', 'C_colorname' => '深石板蓝',),
			array('C_color' => '47 79 79',    'C_hexcolor' => '#2F4F4F', 'C_colorname' => '深铅灰色',),
			array('C_color' => '151 105 79',  'C_hexcolor' => '#97694F', 'C_colorname' => '深棕褐色',),
			array('C_color' => '112 147 219', 'C_hexcolor' => '#7093DB', 'C_colorname' => '深绿松石色',),
			array('C_color' => '133 94 66',   'C_hexcolor' => '#855E42', 'C_colorname' => '暗木色',),
			array('C_color' => '84 84 84',    'C_hexcolor' => '#545454', 'C_colorname' => '淡灰色',),
			array('C_color' => '133 99 99',   'C_hexcolor' => '#856363', 'C_colorname' => '土灰玫瑰红色',),
			array('C_color' => '209 146 117', 'C_hexcolor' => '#D19275', 'C_colorname' => '长石色',),
			array('C_color' => '142 35 35',   'C_hexcolor' => '#8E2323', 'C_colorname' => '火砖色',),
			array('C_color' => '35 142 35',   'C_hexcolor' => '#238E23', 'C_colorname' => '森林绿',),
			array('C_color' => '205 127 50',  'C_hexcolor' => '#CD7F32', 'C_colorname' => '金色',),
			array('C_color' => '219 219 112', 'C_hexcolor' => '#DBDB70', 'C_colorname' => '鲜黃色',),
			array('C_color' => '192 192 192', 'C_hexcolor' => '#C0C0C0', 'C_colorname' => '灰色',),
			array('C_color' => '82 127 118',  'C_hexcolor' => '#527F76', 'C_colorname' => '铜绿色',),
			array('C_color' => '147 219 112', 'C_hexcolor' => '#93DB70', 'C_colorname' => '青黃色',),
			array('C_color' => '33 94 33',    'C_hexcolor' => '#215E21', 'C_colorname' => '猎人绿',),
			array('C_color' => '78 47 47',    'C_hexcolor' => '#4E2F2F', 'C_colorname' => '印度红',),
			array('C_color' => '159 159 95',  'C_hexcolor' => '#9F9F5F', 'C_colorname' => '土黃色',),
			array('C_color' => '192 217 217', 'C_hexcolor' => '#C0D9D9', 'C_colorname' => '浅蓝色',),
			array('C_color' => '168 168 168', 'C_hexcolor' => '#A8A8A8', 'C_colorname' => '浅灰色',),
			array('C_color' => '143 143 189', 'C_hexcolor' => '#8F8FBD', 'C_colorname' => '浅钢蓝色',),
			array('C_color' => '233 194 166', 'C_hexcolor' => '#E9C2A6', 'C_colorname' => '浅木色',),
			array('C_color' => '50 205 50',   'C_hexcolor' => '#32CD32', 'C_colorname' => '石灰绿色',),
			array('C_color' => '228 120 51',  'C_hexcolor' => '#E47833', 'C_colorname' => '桔黃色',),
			array('C_color' => '142 35 107',  'C_hexcolor' => '#8E236B', 'C_colorname' => '褐红色',),
			array('C_color' => '50 205 153',  'C_hexcolor' => '#32CD99', 'C_colorname' => '中海蓝色',),
			array('C_color' => '50 50 205',   'C_hexcolor' => '#3232CD', 'C_colorname' => '中蓝色',),
			array('C_color' => '107 142 35',  'C_hexcolor' => '#6B8E23', 'C_colorname' => '中森林绿',),
			array('C_color' => '234 234 174', 'C_hexcolor' => '#EAEAAE', 'C_colorname' => '中鲜黃色',),
			array('C_color' => '147 112 219', 'C_hexcolor' => '#9370DB', 'C_colorname' => '中兰花色',),
			array('C_color' => '66 111 66',   'C_hexcolor' => '#426F42', 'C_colorname' => '中海绿色',),
			array('C_color' => '127 0 255',   'C_hexcolor' => '#7F00FF', 'C_colorname' => '中石板蓝色',),
			array('C_color' => '127 255 0',   'C_hexcolor' => '#7FFF00', 'C_colorname' => '中春绿色',),
			array('C_color' => '112 219 219', 'C_hexcolor' => '#70DBDB', 'C_colorname' => '中绿松石色',),
			array('C_color' => '219 112 147', 'C_hexcolor' => '#DB7093', 'C_colorname' => '中紫红色',),
			array('C_color' => '166 128 100', 'C_hexcolor' => '#A68064', 'C_colorname' => '中木色',),
			array('C_color' => '47 47 79',    'C_hexcolor' => '#2F2F4F', 'C_colorname' => '深藏青色',),
			array('C_color' => '35 35 142',   'C_hexcolor' => '#23238E', 'C_colorname' => '海军蓝',),
			array('C_color' => '77 77 255',   'C_hexcolor' => '#4D4DFF', 'C_colorname' => '霓虹蓝',),
			array('C_color' => '255 110 199', 'C_hexcolor' => '#FF6EC7', 'C_colorname' => '霓虹粉紅',),
			array('C_color' => '0 0 156',     'C_hexcolor' => '#00009C', 'C_colorname' => '新深藏青色',),
			array('C_color' => '235 199 158', 'C_hexcolor' => '#EBC79E', 'C_colorname' => '新棕褐色',),
			array('C_color' => '207 181 59',  'C_hexcolor' => '#CFB53B', 'C_colorname' => '暗金黃色',),
			array('C_color' => '255 127 0',   'C_hexcolor' => '#FF7F00', 'C_colorname' => '橙色',),
			array('C_color' => '255 36 0',    'C_hexcolor' => '#FF2400', 'C_colorname' => '橙红色',),
			array('C_color' => '219 112 219', 'C_hexcolor' => '#DB70DB', 'C_colorname' => '淡紫色',),
			array('C_color' => '143 188 143', 'C_hexcolor' => '#8FBC8F', 'C_colorname' => '浅绿色',),
			array('C_color' => '188 143 143', 'C_hexcolor' => '#BC8F8F', 'C_colorname' => '粉红色',),
			array('C_color' => '234 173 234', 'C_hexcolor' => '#EAADEA', 'C_colorname' => '李子色',),
			array('C_color' => '217 217 243', 'C_hexcolor' => '#D9D9F3', 'C_colorname' => '石英色',),
			array('C_color' => '89 89 171',   'C_hexcolor' => '#5959AB', 'C_colorname' => '艳蓝色',),
			array('C_color' => '111 66 66',   'C_hexcolor' => '#6F4242', 'C_colorname' => '鲑鱼色',),
			array('C_color' => '188 23 23',   'C_hexcolor' => '#BC1717', 'C_colorname' => '猩红色',),
			array('C_color' => '35 142 104',  'C_hexcolor' => '#238E68', 'C_colorname' => '海绿色',),
			array('C_color' => '107 66 38',   'C_hexcolor' => '#6B4226', 'C_colorname' => '半甜巧克力色',),
			array('C_color' => '142 107 35',  'C_hexcolor' => '#8E6B23', 'C_colorname' => '赭色',),
			array('C_color' => '230 232 250', 'C_hexcolor' => '#E6E8FA', 'C_colorname' => '银色',),
			array('C_color' => '50 153 204',  'C_hexcolor' => '#3299CC', 'C_colorname' => '天蓝',),
			array('C_color' => '0 127 255',   'C_hexcolor' => '#007FFF', 'C_colorname' => '石板蓝',),
			array('C_color' => '255 28 174',  'C_hexcolor' => '#FF1CAE', 'C_colorname' => '艳粉红色',),
			array('C_color' => '0 255 127',   'C_hexcolor' => '#00FF7F', 'C_colorname' => '春绿色',),
			array('C_color' => '35 107 142',  'C_hexcolor' => '#236B8E', 'C_colorname' => '钢蓝色',),
			array('C_color' => '56 176 222',  'C_hexcolor' => '#38B0DE', 'C_colorname' => '亮天蓝色',),
		),
	));
}

function _system_config_save_upload_format($v){
	return array_sum($v);
}


function _system_config_fopen_emulate($k, $v){
	global $tpl;

	$curl_disabled = $fsockopen_disabled = false;

	if(!function_exists('curl_init') || !function_exists('curl_setopt') || !function_exists('curl_exec'))
		$curl_disabled = true;

	if(!function_exists('fsockopen'))
		$fsockopen_disabled = true;

	$tpl->assign(array(
		'C_'.$k.'_'.($v ? 'yes' : 'no') => ' checked="checked"',
		'C_'.$k.'_'.($v ? 'no' : 'yes') => '',
	));

	if($curl_disabled && $fsockopen_disabled) {
		$tpl->assign(array(
			'C_'.$k.'_yes' => ' disabled="disabled"',
			'C_'.$k.'_no'  => ' checked="checked"',
		));
	}
}

function _system_config_fopen_curl($k, $v){
	global $tpl;

	$curl_disabled = $fsockopen_disabled = false;

	if(!function_exists('curl_init') || !function_exists('curl_setopt') || !function_exists('curl_exec'))
		$curl_disabled = true;

	if(!function_exists('fsockopen'))
		$fsockopen_disabled = true;

	$tpl->assign(array(
		'C_'.$k.'_'.($v ? 'yes' : 'no') => ' checked="checked"',
		'C_'.$k.'_'.($v ? 'no' : 'yes') => '',
	));

	if($curl_disabled && $fsockopen_disabled) {
		$tpl->assign(array(
			'C_'.$k.'_yes' => ' disabled="disabled"',
			'C_'.$k.'_no'  => ' disabled="disabled"',
		));
	}elseif($curl_disabled) {
		$tpl->assign(array(
			'C_'.$k.'_yes' => ' disabled="disabled"',
			'C_'.$k.'_no'  => ' checked="checked"',
		));
	}elseif($fsockopen_disabled) {
		$tpl->assign(array(
			'C_'.$k.'_yes' => ' checked="checked"',
			'C_'.$k.'_no'  => ' disabled="disabled"',
		));
	}
}



function _system_config_seo_htmlsubdir($k, $v){
	global $tpl;

	$htmlsubdiropts = array(
		'id' => '首字母/日期',
		'iw' => '首字母/字数',
		'it' => '首字母/题材',
		'di' => '日期/首字母',
		'dw' => '日期/字数',
		'dt' => '日期/题材',
		'wi' => '字数/首字母',
		'wd' => '字数/日期',
		'wt' => '字数/题材',
		'ti' => '题材/首字母',
		'td' => '题材/日期',
		'tw' => '题材/字数',
	);
	$htmlsubdirs = array();
	foreach($htmlsubdiropts as $key => $val) {
		$htmlsubdirs[] = array(
			'v' => $key,
			't' => $val,
			's' => $v == $key ? ' selected="selected"' : '',
		);
	}
	$tpl->assign('htmlsubdirs', $htmlsubdirs);
}



function _system_config_safe_secode($k, $v){
	global $tpl;

	$b = sprintf('%03b', $v);

	$tpl->assign(array(
		'C_secode_4' => $b[0] ? ' checked="checked"' : '',
		'C_secode_2' => $b[1] ? ' checked="checked"' : '',
		'C_secode_1' => $b[2] ? ' checked="checked"' : '',
	));
}

function _system_config_safe_chmod_dir($k, $v){
	global $tpl;

	$v = decoct($v);

	$u = sprintf('%03b', $v[0]);
	$g = sprintf('%03b', $v[1]);
	$o = sprintf('%03b', $v[2]);

	$tpl->assign(array(
		'C_'.$k => $v,

		'dur' => $u[0] ? ' checked="checked"' : '',
		'dgr' => $g[0] ? ' checked="checked"' : '',
		'dor' => $o[0] ? ' checked="checked"' : '',

		'duw' => $u[1] ? ' checked="checked"' : '',
		'dgw' => $g[1] ? ' checked="checked"' : '',
		'dow' => $o[1] ? ' checked="checked"' : '',

		'dux' => $u[2] ? ' checked="checked"' : '',
		'dgx' => $g[2] ? ' checked="checked"' : '',
		'dox' => $o[2] ? ' checked="checked"' : '',
	));
}

function _system_config_safe_chmod_file($k, $v){
	global $tpl;

	$v = decoct($v);

	$u = sprintf('%03b', $v[0]);
	$g = sprintf('%03b', $v[1]);
	$o = sprintf('%03b', $v[2]);

	$tpl->assign(array(
		'C_'.$k => $v,

		'fur' => $u[0] ? ' checked="checked"' : '',
		'fgr' => $g[0] ? ' checked="checked"' : '',
		'for' => $o[0] ? ' checked="checked"' : '',

		'fuw' => $u[1] ? ' checked="checked"' : '',
		'fgw' => $g[1] ? ' checked="checked"' : '',
		'fow' => $o[1] ? ' checked="checked"' : '',

		'fux' => $u[2] ? ' checked="checked"' : '',
		'fgx' => $g[2] ? ' checked="checked"' : '',
		'fox' => $o[2] ? ' checked="checked"' : '',
	));
}


function _system_config_save_safe_secode($v){
	return array_sum($v);
}

function _system_config_save_safe_chmod_dir($v){
	return octdec($v);
}

function _system_config_save_safe_chmod_file($v){
	return octdec($v);
}




function _system_config_info_($k, $v){
	global $tpl;

	if('html' == $k) {
		foreach(explode(',', $v) as $k => $html) {
			$tpl->assign('C_info'.$k.'_html', $html ? ' checked="checked"' : '');
		}
	}else {
		$tpl->assign('C_'.$k, html_show(html_show_decode($v), false));
	}
}


function _system_config_save_official_password($v){
	if(strlen($v) == 32) {
		return $v;
	}else {
		return md5($v);
	}
}

function _system_config_save_mobile_password($v){
	if(strlen($v) == 32) {
		return $v;
	}else {
		return md5($v);
	}
}


function _system_config_vip_background($k, $v){
	global $tpl;

	include _config('pay');
	$tpl->assign('C_moneytitle', $_SYSTEM['PAY']['unit']);
	$tpl->assign(array(
		'C_'.$k => $v,
		'color' => array(
			array('C_color' => '255 0 0',     'C_hexcolor' => '#FF0000', 'C_colorname' => '红色',),
			array('C_color' => '0 255 0',     'C_hexcolor' => '#00FF00', 'C_colorname' => '绿色',),
			array('C_color' => '0 0 255',     'C_hexcolor' => '#0000FF', 'C_colorname' => '蓝色',),
			array('C_color' => '255 0 255',   'C_hexcolor' => '#FF00FF', 'C_colorname' => '牡丹红',),
			array('C_color' => '0 255 255',   'C_hexcolor' => '#00FFFF', 'C_colorname' => '青色',),
			array('C_color' => '255 255 0',   'C_hexcolor' => '#FFFF00', 'C_colorname' => '黃色',),
			array('C_color' => '112 219 147', 'C_hexcolor' => '#70DB93', 'C_colorname' => '海蓝',),
			array('C_color' => '255 255 255', 'C_hexcolor' => '#FFFFFF', 'C_colorname' => '白色',),
			array('C_color' => '0 0 0',       'C_hexcolor' => '#000000', 'C_colorname' => '黑色',),
			array('C_color' => '92 51 23',    'C_hexcolor' => '#5C3317', 'C_colorname' => '巧克力色',),
			array('C_color' => '159 95 159',  'C_hexcolor' => '#9F5F9F', 'C_colorname' => '蓝紫色',),
			array('C_color' => '181 166 66',  'C_hexcolor' => '#B5A642', 'C_colorname' => '黃铜色',),
			array('C_color' => '217 217 25',  'C_hexcolor' => '#D9D919', 'C_colorname' => '亮金色',),
			array('C_color' => '166 125 61',  'C_hexcolor' => '#A67D3D', 'C_colorname' => '棕色',),
			array('C_color' => '140 120 83',  'C_hexcolor' => '#8C7853', 'C_colorname' => '青铜色',),
			array('C_color' => '166 125 61',  'C_hexcolor' => '#A67D3D', 'C_colorname' => '青铜色二',),
			array('C_color' => '95 159 159',  'C_hexcolor' => '#5F9F9F', 'C_colorname' => '士官服蓝色',),
			array('C_color' => '217 135 25',  'C_hexcolor' => '#D98719', 'C_colorname' => '冷铜色',),
			array('C_color' => '184 115 51',  'C_hexcolor' => '#B87333', 'C_colorname' => '铜色',),
			array('C_color' => '255 127 0',   'C_hexcolor' => '#FF7F00', 'C_colorname' => '珊瑚红',),
			array('C_color' => '66 66 111',   'C_hexcolor' => '#42426F', 'C_colorname' => '紫蓝色',),
			array('C_color' => '92 64 51',    'C_hexcolor' => '#5C4033', 'C_colorname' => '深棕',),
			array('C_color' => '47 79 47',    'C_hexcolor' => '#2F4F2F', 'C_colorname' => '深绿',),
			array('C_color' => '74 118 110',  'C_hexcolor' => '#4A766E', 'C_colorname' => '深铜绿色',),
			array('C_color' => '79 79 47',    'C_hexcolor' => '#4F4F2F', 'C_colorname' => '深橄榄绿',),
			array('C_color' => '153 50 205',  'C_hexcolor' => '#9932CD', 'C_colorname' => '深兰花色',),
			array('C_color' => '135 31 120',  'C_hexcolor' => '#871F78', 'C_colorname' => '深紫色',),
			array('C_color' => '107 35 142',  'C_hexcolor' => '#6B238E', 'C_colorname' => '深石板蓝',),
			array('C_color' => '47 79 79',    'C_hexcolor' => '#2F4F4F', 'C_colorname' => '深铅灰色',),
			array('C_color' => '151 105 79',  'C_hexcolor' => '#97694F', 'C_colorname' => '深棕褐色',),
			array('C_color' => '112 147 219', 'C_hexcolor' => '#7093DB', 'C_colorname' => '深绿松石色',),
			array('C_color' => '133 94 66',   'C_hexcolor' => '#855E42', 'C_colorname' => '暗木色',),
			array('C_color' => '84 84 84',    'C_hexcolor' => '#545454', 'C_colorname' => '淡灰色',),
			array('C_color' => '133 99 99',   'C_hexcolor' => '#856363', 'C_colorname' => '土灰玫瑰红色',),
			array('C_color' => '209 146 117', 'C_hexcolor' => '#D19275', 'C_colorname' => '长石色',),
			array('C_color' => '142 35 35',   'C_hexcolor' => '#8E2323', 'C_colorname' => '火砖色',),
			array('C_color' => '35 142 35',   'C_hexcolor' => '#238E23', 'C_colorname' => '森林绿',),
			array('C_color' => '205 127 50',  'C_hexcolor' => '#CD7F32', 'C_colorname' => '金色',),
			array('C_color' => '219 219 112', 'C_hexcolor' => '#DBDB70', 'C_colorname' => '鲜黃色',),
			array('C_color' => '192 192 192', 'C_hexcolor' => '#C0C0C0', 'C_colorname' => '灰色',),
			array('C_color' => '82 127 118',  'C_hexcolor' => '#527F76', 'C_colorname' => '铜绿色',),
			array('C_color' => '147 219 112', 'C_hexcolor' => '#93DB70', 'C_colorname' => '青黃色',),
			array('C_color' => '33 94 33',    'C_hexcolor' => '#215E21', 'C_colorname' => '猎人绿',),
			array('C_color' => '78 47 47',    'C_hexcolor' => '#4E2F2F', 'C_colorname' => '印度红',),
			array('C_color' => '159 159 95',  'C_hexcolor' => '#9F9F5F', 'C_colorname' => '土黃色',),
			array('C_color' => '192 217 217', 'C_hexcolor' => '#C0D9D9', 'C_colorname' => '浅蓝色',),
			array('C_color' => '168 168 168', 'C_hexcolor' => '#A8A8A8', 'C_colorname' => '浅灰色',),
			array('C_color' => '143 143 189', 'C_hexcolor' => '#8F8FBD', 'C_colorname' => '浅钢蓝色',),
			array('C_color' => '233 194 166', 'C_hexcolor' => '#E9C2A6', 'C_colorname' => '浅木色',),
			array('C_color' => '50 205 50',   'C_hexcolor' => '#32CD32', 'C_colorname' => '石灰绿色',),
			array('C_color' => '228 120 51',  'C_hexcolor' => '#E47833', 'C_colorname' => '桔黃色',),
			array('C_color' => '142 35 107',  'C_hexcolor' => '#8E236B', 'C_colorname' => '褐红色',),
			array('C_color' => '50 205 153',  'C_hexcolor' => '#32CD99', 'C_colorname' => '中海蓝色',),
			array('C_color' => '50 50 205',   'C_hexcolor' => '#3232CD', 'C_colorname' => '中蓝色',),
			array('C_color' => '107 142 35',  'C_hexcolor' => '#6B8E23', 'C_colorname' => '中森林绿',),
			array('C_color' => '234 234 174', 'C_hexcolor' => '#EAEAAE', 'C_colorname' => '中鲜黃色',),
			array('C_color' => '147 112 219', 'C_hexcolor' => '#9370DB', 'C_colorname' => '中兰花色',),
			array('C_color' => '66 111 66',   'C_hexcolor' => '#426F42', 'C_colorname' => '中海绿色',),
			array('C_color' => '127 0 255',   'C_hexcolor' => '#7F00FF', 'C_colorname' => '中石板蓝色',),
			array('C_color' => '127 255 0',   'C_hexcolor' => '#7FFF00', 'C_colorname' => '中春绿色',),
			array('C_color' => '112 219 219', 'C_hexcolor' => '#70DBDB', 'C_colorname' => '中绿松石色',),
			array('C_color' => '219 112 147', 'C_hexcolor' => '#DB7093', 'C_colorname' => '中紫红色',),
			array('C_color' => '166 128 100', 'C_hexcolor' => '#A68064', 'C_colorname' => '中木色',),
			array('C_color' => '47 47 79',    'C_hexcolor' => '#2F2F4F', 'C_colorname' => '深藏青色',),
			array('C_color' => '35 35 142',   'C_hexcolor' => '#23238E', 'C_colorname' => '海军蓝',),
			array('C_color' => '77 77 255',   'C_hexcolor' => '#4D4DFF', 'C_colorname' => '霓虹蓝',),
			array('C_color' => '255 110 199', 'C_hexcolor' => '#FF6EC7', 'C_colorname' => '霓虹粉紅',),
			array('C_color' => '0 0 156',     'C_hexcolor' => '#00009C', 'C_colorname' => '新深藏青色',),
			array('C_color' => '235 199 158', 'C_hexcolor' => '#EBC79E', 'C_colorname' => '新棕褐色',),
			array('C_color' => '207 181 59',  'C_hexcolor' => '#CFB53B', 'C_colorname' => '暗金黃色',),
			array('C_color' => '255 127 0',   'C_hexcolor' => '#FF7F00', 'C_colorname' => '橙色',),
			array('C_color' => '255 36 0',    'C_hexcolor' => '#FF2400', 'C_colorname' => '橙红色',),
			array('C_color' => '219 112 219', 'C_hexcolor' => '#DB70DB', 'C_colorname' => '淡紫色',),
			array('C_color' => '143 188 143', 'C_hexcolor' => '#8FBC8F', 'C_colorname' => '浅绿色',),
			array('C_color' => '188 143 143', 'C_hexcolor' => '#BC8F8F', 'C_colorname' => '粉红色',),
			array('C_color' => '234 173 234', 'C_hexcolor' => '#EAADEA', 'C_colorname' => '李子色',),
			array('C_color' => '217 217 243', 'C_hexcolor' => '#D9D9F3', 'C_colorname' => '石英色',),
			array('C_color' => '89 89 171',   'C_hexcolor' => '#5959AB', 'C_colorname' => '艳蓝色',),
			array('C_color' => '111 66 66',   'C_hexcolor' => '#6F4242', 'C_colorname' => '鲑鱼色',),
			array('C_color' => '188 23 23',   'C_hexcolor' => '#BC1717', 'C_colorname' => '猩红色',),
			array('C_color' => '35 142 104',  'C_hexcolor' => '#238E68', 'C_colorname' => '海绿色',),
			array('C_color' => '107 66 38',   'C_hexcolor' => '#6B4226', 'C_colorname' => '半甜巧克力色',),
			array('C_color' => '142 107 35',  'C_hexcolor' => '#8E6B23', 'C_colorname' => '赭色',),
			array('C_color' => '230 232 250', 'C_hexcolor' => '#E6E8FA', 'C_colorname' => '银色',),
			array('C_color' => '50 153 204',  'C_hexcolor' => '#3299CC', 'C_colorname' => '天蓝',),
			array('C_color' => '0 127 255',   'C_hexcolor' => '#007FFF', 'C_colorname' => '石板蓝',),
			array('C_color' => '255 28 174',  'C_hexcolor' => '#FF1CAE', 'C_colorname' => '艳粉红色',),
			array('C_color' => '0 255 127',   'C_hexcolor' => '#00FF7F', 'C_colorname' => '春绿色',),
			array('C_color' => '35 107 142',  'C_hexcolor' => '#236B8E', 'C_colorname' => '钢蓝色',),
			array('C_color' => '56 176 222',  'C_hexcolor' => '#38B0DE', 'C_colorname' => '亮天蓝色',),
		),
	));
}
function _system_config_vip_color($k, $v){
	global $tpl;

	$tpl->assign(array(
		'C_'.$k => $v,
		'color' => array(
			array('C_color' => '255 0 0',     'C_hexcolor' => '#FF0000', 'C_colorname' => '红色',),
			array('C_color' => '0 255 0',     'C_hexcolor' => '#00FF00', 'C_colorname' => '绿色',),
			array('C_color' => '0 0 255',     'C_hexcolor' => '#0000FF', 'C_colorname' => '蓝色',),
			array('C_color' => '255 0 255',   'C_hexcolor' => '#FF00FF', 'C_colorname' => '牡丹红',),
			array('C_color' => '0 255 255',   'C_hexcolor' => '#00FFFF', 'C_colorname' => '青色',),
			array('C_color' => '255 255 0',   'C_hexcolor' => '#FFFF00', 'C_colorname' => '黃色',),
			array('C_color' => '112 219 147', 'C_hexcolor' => '#70DB93', 'C_colorname' => '海蓝',),
			array('C_color' => '255 255 255', 'C_hexcolor' => '#FFFFFF', 'C_colorname' => '白色',),
			array('C_color' => '0 0 0',       'C_hexcolor' => '#000000', 'C_colorname' => '黑色',),
			array('C_color' => '92 51 23',    'C_hexcolor' => '#5C3317', 'C_colorname' => '巧克力色',),
			array('C_color' => '159 95 159',  'C_hexcolor' => '#9F5F9F', 'C_colorname' => '蓝紫色',),
			array('C_color' => '181 166 66',  'C_hexcolor' => '#B5A642', 'C_colorname' => '黃铜色',),
			array('C_color' => '217 217 25',  'C_hexcolor' => '#D9D919', 'C_colorname' => '亮金色',),
			array('C_color' => '166 125 61',  'C_hexcolor' => '#A67D3D', 'C_colorname' => '棕色',),
			array('C_color' => '140 120 83',  'C_hexcolor' => '#8C7853', 'C_colorname' => '青铜色',),
			array('C_color' => '166 125 61',  'C_hexcolor' => '#A67D3D', 'C_colorname' => '青铜色二',),
			array('C_color' => '95 159 159',  'C_hexcolor' => '#5F9F9F', 'C_colorname' => '士官服蓝色',),
			array('C_color' => '217 135 25',  'C_hexcolor' => '#D98719', 'C_colorname' => '冷铜色',),
			array('C_color' => '184 115 51',  'C_hexcolor' => '#B87333', 'C_colorname' => '铜色',),
			array('C_color' => '255 127 0',   'C_hexcolor' => '#FF7F00', 'C_colorname' => '珊瑚红',),
			array('C_color' => '66 66 111',   'C_hexcolor' => '#42426F', 'C_colorname' => '紫蓝色',),
			array('C_color' => '92 64 51',    'C_hexcolor' => '#5C4033', 'C_colorname' => '深棕',),
			array('C_color' => '47 79 47',    'C_hexcolor' => '#2F4F2F', 'C_colorname' => '深绿',),
			array('C_color' => '74 118 110',  'C_hexcolor' => '#4A766E', 'C_colorname' => '深铜绿色',),
			array('C_color' => '79 79 47',    'C_hexcolor' => '#4F4F2F', 'C_colorname' => '深橄榄绿',),
			array('C_color' => '153 50 205',  'C_hexcolor' => '#9932CD', 'C_colorname' => '深兰花色',),
			array('C_color' => '135 31 120',  'C_hexcolor' => '#871F78', 'C_colorname' => '深紫色',),
			array('C_color' => '107 35 142',  'C_hexcolor' => '#6B238E', 'C_colorname' => '深石板蓝',),
			array('C_color' => '47 79 79',    'C_hexcolor' => '#2F4F4F', 'C_colorname' => '深铅灰色',),
			array('C_color' => '151 105 79',  'C_hexcolor' => '#97694F', 'C_colorname' => '深棕褐色',),
			array('C_color' => '112 147 219', 'C_hexcolor' => '#7093DB', 'C_colorname' => '深绿松石色',),
			array('C_color' => '133 94 66',   'C_hexcolor' => '#855E42', 'C_colorname' => '暗木色',),
			array('C_color' => '84 84 84',    'C_hexcolor' => '#545454', 'C_colorname' => '淡灰色',),
			array('C_color' => '133 99 99',   'C_hexcolor' => '#856363', 'C_colorname' => '土灰玫瑰红色',),
			array('C_color' => '209 146 117', 'C_hexcolor' => '#D19275', 'C_colorname' => '长石色',),
			array('C_color' => '142 35 35',   'C_hexcolor' => '#8E2323', 'C_colorname' => '火砖色',),
			array('C_color' => '35 142 35',   'C_hexcolor' => '#238E23', 'C_colorname' => '森林绿',),
			array('C_color' => '205 127 50',  'C_hexcolor' => '#CD7F32', 'C_colorname' => '金色',),
			array('C_color' => '219 219 112', 'C_hexcolor' => '#DBDB70', 'C_colorname' => '鲜黃色',),
			array('C_color' => '192 192 192', 'C_hexcolor' => '#C0C0C0', 'C_colorname' => '灰色',),
			array('C_color' => '82 127 118',  'C_hexcolor' => '#527F76', 'C_colorname' => '铜绿色',),
			array('C_color' => '147 219 112', 'C_hexcolor' => '#93DB70', 'C_colorname' => '青黃色',),
			array('C_color' => '33 94 33',    'C_hexcolor' => '#215E21', 'C_colorname' => '猎人绿',),
			array('C_color' => '78 47 47',    'C_hexcolor' => '#4E2F2F', 'C_colorname' => '印度红',),
			array('C_color' => '159 159 95',  'C_hexcolor' => '#9F9F5F', 'C_colorname' => '土黃色',),
			array('C_color' => '192 217 217', 'C_hexcolor' => '#C0D9D9', 'C_colorname' => '浅蓝色',),
			array('C_color' => '168 168 168', 'C_hexcolor' => '#A8A8A8', 'C_colorname' => '浅灰色',),
			array('C_color' => '143 143 189', 'C_hexcolor' => '#8F8FBD', 'C_colorname' => '浅钢蓝色',),
			array('C_color' => '233 194 166', 'C_hexcolor' => '#E9C2A6', 'C_colorname' => '浅木色',),
			array('C_color' => '50 205 50',   'C_hexcolor' => '#32CD32', 'C_colorname' => '石灰绿色',),
			array('C_color' => '228 120 51',  'C_hexcolor' => '#E47833', 'C_colorname' => '桔黃色',),
			array('C_color' => '142 35 107',  'C_hexcolor' => '#8E236B', 'C_colorname' => '褐红色',),
			array('C_color' => '50 205 153',  'C_hexcolor' => '#32CD99', 'C_colorname' => '中海蓝色',),
			array('C_color' => '50 50 205',   'C_hexcolor' => '#3232CD', 'C_colorname' => '中蓝色',),
			array('C_color' => '107 142 35',  'C_hexcolor' => '#6B8E23', 'C_colorname' => '中森林绿',),
			array('C_color' => '234 234 174', 'C_hexcolor' => '#EAEAAE', 'C_colorname' => '中鲜黃色',),
			array('C_color' => '147 112 219', 'C_hexcolor' => '#9370DB', 'C_colorname' => '中兰花色',),
			array('C_color' => '66 111 66',   'C_hexcolor' => '#426F42', 'C_colorname' => '中海绿色',),
			array('C_color' => '127 0 255',   'C_hexcolor' => '#7F00FF', 'C_colorname' => '中石板蓝色',),
			array('C_color' => '127 255 0',   'C_hexcolor' => '#7FFF00', 'C_colorname' => '中春绿色',),
			array('C_color' => '112 219 219', 'C_hexcolor' => '#70DBDB', 'C_colorname' => '中绿松石色',),
			array('C_color' => '219 112 147', 'C_hexcolor' => '#DB7093', 'C_colorname' => '中紫红色',),
			array('C_color' => '166 128 100', 'C_hexcolor' => '#A68064', 'C_colorname' => '中木色',),
			array('C_color' => '47 47 79',    'C_hexcolor' => '#2F2F4F', 'C_colorname' => '深藏青色',),
			array('C_color' => '35 35 142',   'C_hexcolor' => '#23238E', 'C_colorname' => '海军蓝',),
			array('C_color' => '77 77 255',   'C_hexcolor' => '#4D4DFF', 'C_colorname' => '霓虹蓝',),
			array('C_color' => '255 110 199', 'C_hexcolor' => '#FF6EC7', 'C_colorname' => '霓虹粉紅',),
			array('C_color' => '0 0 156',     'C_hexcolor' => '#00009C', 'C_colorname' => '新深藏青色',),
			array('C_color' => '235 199 158', 'C_hexcolor' => '#EBC79E', 'C_colorname' => '新棕褐色',),
			array('C_color' => '207 181 59',  'C_hexcolor' => '#CFB53B', 'C_colorname' => '暗金黃色',),
			array('C_color' => '255 127 0',   'C_hexcolor' => '#FF7F00', 'C_colorname' => '橙色',),
			array('C_color' => '255 36 0',    'C_hexcolor' => '#FF2400', 'C_colorname' => '橙红色',),
			array('C_color' => '219 112 219', 'C_hexcolor' => '#DB70DB', 'C_colorname' => '淡紫色',),
			array('C_color' => '143 188 143', 'C_hexcolor' => '#8FBC8F', 'C_colorname' => '浅绿色',),
			array('C_color' => '188 143 143', 'C_hexcolor' => '#BC8F8F', 'C_colorname' => '粉红色',),
			array('C_color' => '234 173 234', 'C_hexcolor' => '#EAADEA', 'C_colorname' => '李子色',),
			array('C_color' => '217 217 243', 'C_hexcolor' => '#D9D9F3', 'C_colorname' => '石英色',),
			array('C_color' => '89 89 171',   'C_hexcolor' => '#5959AB', 'C_colorname' => '艳蓝色',),
			array('C_color' => '111 66 66',   'C_hexcolor' => '#6F4242', 'C_colorname' => '鲑鱼色',),
			array('C_color' => '188 23 23',   'C_hexcolor' => '#BC1717', 'C_colorname' => '猩红色',),
			array('C_color' => '35 142 104',  'C_hexcolor' => '#238E68', 'C_colorname' => '海绿色',),
			array('C_color' => '107 66 38',   'C_hexcolor' => '#6B4226', 'C_colorname' => '半甜巧克力色',),
			array('C_color' => '142 107 35',  'C_hexcolor' => '#8E6B23', 'C_colorname' => '赭色',),
			array('C_color' => '230 232 250', 'C_hexcolor' => '#E6E8FA', 'C_colorname' => '银色',),
			array('C_color' => '50 153 204',  'C_hexcolor' => '#3299CC', 'C_colorname' => '天蓝',),
			array('C_color' => '0 127 255',   'C_hexcolor' => '#007FFF', 'C_colorname' => '石板蓝',),
			array('C_color' => '255 28 174',  'C_hexcolor' => '#FF1CAE', 'C_colorname' => '艳粉红色',),
			array('C_color' => '0 255 127',   'C_hexcolor' => '#00FF7F', 'C_colorname' => '春绿色',),
			array('C_color' => '35 107 142',  'C_hexcolor' => '#236B8E', 'C_colorname' => '钢蓝色',),
			array('C_color' => '56 176 222',  'C_hexcolor' => '#38B0DE', 'C_colorname' => '亮天蓝色',),
		),
	));
}
function _system_config_vip_mark_color($k, $v){
	global $tpl;

	$tpl->assign(array(
		'C_'.$k => $v,
		'color' => array(
			array('C_color' => '255 0 0',     'C_hexcolor' => '#FF0000', 'C_colorname' => '红色',),
			array('C_color' => '0 255 0',     'C_hexcolor' => '#00FF00', 'C_colorname' => '绿色',),
			array('C_color' => '0 0 255',     'C_hexcolor' => '#0000FF', 'C_colorname' => '蓝色',),
			array('C_color' => '255 0 255',   'C_hexcolor' => '#FF00FF', 'C_colorname' => '牡丹红',),
			array('C_color' => '0 255 255',   'C_hexcolor' => '#00FFFF', 'C_colorname' => '青色',),
			array('C_color' => '255 255 0',   'C_hexcolor' => '#FFFF00', 'C_colorname' => '黃色',),
			array('C_color' => '112 219 147', 'C_hexcolor' => '#70DB93', 'C_colorname' => '海蓝',),
			array('C_color' => '255 255 255', 'C_hexcolor' => '#FFFFFF', 'C_colorname' => '白色',),
			array('C_color' => '0 0 0',       'C_hexcolor' => '#000000', 'C_colorname' => '黑色',),
			array('C_color' => '92 51 23',    'C_hexcolor' => '#5C3317', 'C_colorname' => '巧克力色',),
			array('C_color' => '159 95 159',  'C_hexcolor' => '#9F5F9F', 'C_colorname' => '蓝紫色',),
			array('C_color' => '181 166 66',  'C_hexcolor' => '#B5A642', 'C_colorname' => '黃铜色',),
			array('C_color' => '217 217 25',  'C_hexcolor' => '#D9D919', 'C_colorname' => '亮金色',),
			array('C_color' => '166 125 61',  'C_hexcolor' => '#A67D3D', 'C_colorname' => '棕色',),
			array('C_color' => '140 120 83',  'C_hexcolor' => '#8C7853', 'C_colorname' => '青铜色',),
			array('C_color' => '166 125 61',  'C_hexcolor' => '#A67D3D', 'C_colorname' => '青铜色二',),
			array('C_color' => '95 159 159',  'C_hexcolor' => '#5F9F9F', 'C_colorname' => '士官服蓝色',),
			array('C_color' => '217 135 25',  'C_hexcolor' => '#D98719', 'C_colorname' => '冷铜色',),
			array('C_color' => '184 115 51',  'C_hexcolor' => '#B87333', 'C_colorname' => '铜色',),
			array('C_color' => '255 127 0',   'C_hexcolor' => '#FF7F00', 'C_colorname' => '珊瑚红',),
			array('C_color' => '66 66 111',   'C_hexcolor' => '#42426F', 'C_colorname' => '紫蓝色',),
			array('C_color' => '92 64 51',    'C_hexcolor' => '#5C4033', 'C_colorname' => '深棕',),
			array('C_color' => '47 79 47',    'C_hexcolor' => '#2F4F2F', 'C_colorname' => '深绿',),
			array('C_color' => '74 118 110',  'C_hexcolor' => '#4A766E', 'C_colorname' => '深铜绿色',),
			array('C_color' => '79 79 47',    'C_hexcolor' => '#4F4F2F', 'C_colorname' => '深橄榄绿',),
			array('C_color' => '153 50 205',  'C_hexcolor' => '#9932CD', 'C_colorname' => '深兰花色',),
			array('C_color' => '135 31 120',  'C_hexcolor' => '#871F78', 'C_colorname' => '深紫色',),
			array('C_color' => '107 35 142',  'C_hexcolor' => '#6B238E', 'C_colorname' => '深石板蓝',),
			array('C_color' => '47 79 79',    'C_hexcolor' => '#2F4F4F', 'C_colorname' => '深铅灰色',),
			array('C_color' => '151 105 79',  'C_hexcolor' => '#97694F', 'C_colorname' => '深棕褐色',),
			array('C_color' => '112 147 219', 'C_hexcolor' => '#7093DB', 'C_colorname' => '深绿松石色',),
			array('C_color' => '133 94 66',   'C_hexcolor' => '#855E42', 'C_colorname' => '暗木色',),
			array('C_color' => '84 84 84',    'C_hexcolor' => '#545454', 'C_colorname' => '淡灰色',),
			array('C_color' => '133 99 99',   'C_hexcolor' => '#856363', 'C_colorname' => '土灰玫瑰红色',),
			array('C_color' => '209 146 117', 'C_hexcolor' => '#D19275', 'C_colorname' => '长石色',),
			array('C_color' => '142 35 35',   'C_hexcolor' => '#8E2323', 'C_colorname' => '火砖色',),
			array('C_color' => '35 142 35',   'C_hexcolor' => '#238E23', 'C_colorname' => '森林绿',),
			array('C_color' => '205 127 50',  'C_hexcolor' => '#CD7F32', 'C_colorname' => '金色',),
			array('C_color' => '219 219 112', 'C_hexcolor' => '#DBDB70', 'C_colorname' => '鲜黃色',),
			array('C_color' => '192 192 192', 'C_hexcolor' => '#C0C0C0', 'C_colorname' => '灰色',),
			array('C_color' => '82 127 118',  'C_hexcolor' => '#527F76', 'C_colorname' => '铜绿色',),
			array('C_color' => '147 219 112', 'C_hexcolor' => '#93DB70', 'C_colorname' => '青黃色',),
			array('C_color' => '33 94 33',    'C_hexcolor' => '#215E21', 'C_colorname' => '猎人绿',),
			array('C_color' => '78 47 47',    'C_hexcolor' => '#4E2F2F', 'C_colorname' => '印度红',),
			array('C_color' => '159 159 95',  'C_hexcolor' => '#9F9F5F', 'C_colorname' => '土黃色',),
			array('C_color' => '192 217 217', 'C_hexcolor' => '#C0D9D9', 'C_colorname' => '浅蓝色',),
			array('C_color' => '168 168 168', 'C_hexcolor' => '#A8A8A8', 'C_colorname' => '浅灰色',),
			array('C_color' => '143 143 189', 'C_hexcolor' => '#8F8FBD', 'C_colorname' => '浅钢蓝色',),
			array('C_color' => '233 194 166', 'C_hexcolor' => '#E9C2A6', 'C_colorname' => '浅木色',),
			array('C_color' => '50 205 50',   'C_hexcolor' => '#32CD32', 'C_colorname' => '石灰绿色',),
			array('C_color' => '228 120 51',  'C_hexcolor' => '#E47833', 'C_colorname' => '桔黃色',),
			array('C_color' => '142 35 107',  'C_hexcolor' => '#8E236B', 'C_colorname' => '褐红色',),
			array('C_color' => '50 205 153',  'C_hexcolor' => '#32CD99', 'C_colorname' => '中海蓝色',),
			array('C_color' => '50 50 205',   'C_hexcolor' => '#3232CD', 'C_colorname' => '中蓝色',),
			array('C_color' => '107 142 35',  'C_hexcolor' => '#6B8E23', 'C_colorname' => '中森林绿',),
			array('C_color' => '234 234 174', 'C_hexcolor' => '#EAEAAE', 'C_colorname' => '中鲜黃色',),
			array('C_color' => '147 112 219', 'C_hexcolor' => '#9370DB', 'C_colorname' => '中兰花色',),
			array('C_color' => '66 111 66',   'C_hexcolor' => '#426F42', 'C_colorname' => '中海绿色',),
			array('C_color' => '127 0 255',   'C_hexcolor' => '#7F00FF', 'C_colorname' => '中石板蓝色',),
			array('C_color' => '127 255 0',   'C_hexcolor' => '#7FFF00', 'C_colorname' => '中春绿色',),
			array('C_color' => '112 219 219', 'C_hexcolor' => '#70DBDB', 'C_colorname' => '中绿松石色',),
			array('C_color' => '219 112 147', 'C_hexcolor' => '#DB7093', 'C_colorname' => '中紫红色',),
			array('C_color' => '166 128 100', 'C_hexcolor' => '#A68064', 'C_colorname' => '中木色',),
			array('C_color' => '47 47 79',    'C_hexcolor' => '#2F2F4F', 'C_colorname' => '深藏青色',),
			array('C_color' => '35 35 142',   'C_hexcolor' => '#23238E', 'C_colorname' => '海军蓝',),
			array('C_color' => '77 77 255',   'C_hexcolor' => '#4D4DFF', 'C_colorname' => '霓虹蓝',),
			array('C_color' => '255 110 199', 'C_hexcolor' => '#FF6EC7', 'C_colorname' => '霓虹粉紅',),
			array('C_color' => '0 0 156',     'C_hexcolor' => '#00009C', 'C_colorname' => '新深藏青色',),
			array('C_color' => '235 199 158', 'C_hexcolor' => '#EBC79E', 'C_colorname' => '新棕褐色',),
			array('C_color' => '207 181 59',  'C_hexcolor' => '#CFB53B', 'C_colorname' => '暗金黃色',),
			array('C_color' => '255 127 0',   'C_hexcolor' => '#FF7F00', 'C_colorname' => '橙色',),
			array('C_color' => '255 36 0',    'C_hexcolor' => '#FF2400', 'C_colorname' => '橙红色',),
			array('C_color' => '219 112 219', 'C_hexcolor' => '#DB70DB', 'C_colorname' => '淡紫色',),
			array('C_color' => '143 188 143', 'C_hexcolor' => '#8FBC8F', 'C_colorname' => '浅绿色',),
			array('C_color' => '188 143 143', 'C_hexcolor' => '#BC8F8F', 'C_colorname' => '粉红色',),
			array('C_color' => '234 173 234', 'C_hexcolor' => '#EAADEA', 'C_colorname' => '李子色',),
			array('C_color' => '217 217 243', 'C_hexcolor' => '#D9D9F3', 'C_colorname' => '石英色',),
			array('C_color' => '89 89 171',   'C_hexcolor' => '#5959AB', 'C_colorname' => '艳蓝色',),
			array('C_color' => '111 66 66',   'C_hexcolor' => '#6F4242', 'C_colorname' => '鲑鱼色',),
			array('C_color' => '188 23 23',   'C_hexcolor' => '#BC1717', 'C_colorname' => '猩红色',),
			array('C_color' => '35 142 104',  'C_hexcolor' => '#238E68', 'C_colorname' => '海绿色',),
			array('C_color' => '107 66 38',   'C_hexcolor' => '#6B4226', 'C_colorname' => '半甜巧克力色',),
			array('C_color' => '142 107 35',  'C_hexcolor' => '#8E6B23', 'C_colorname' => '赭色',),
			array('C_color' => '230 232 250', 'C_hexcolor' => '#E6E8FA', 'C_colorname' => '银色',),
			array('C_color' => '50 153 204',  'C_hexcolor' => '#3299CC', 'C_colorname' => '天蓝',),
			array('C_color' => '0 127 255',   'C_hexcolor' => '#007FFF', 'C_colorname' => '石板蓝',),
			array('C_color' => '255 28 174',  'C_hexcolor' => '#FF1CAE', 'C_colorname' => '艳粉红色',),
			array('C_color' => '0 255 127',   'C_hexcolor' => '#00FF7F', 'C_colorname' => '春绿色',),
			array('C_color' => '35 107 142',  'C_hexcolor' => '#236B8E', 'C_colorname' => '钢蓝色',),
			array('C_color' => '56 176 222',  'C_hexcolor' => '#38B0DE', 'C_colorname' => '亮天蓝色',),
		),
	));
}
