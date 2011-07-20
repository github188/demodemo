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
if(!defined('IN_Read8') || !defined('IN_Read8_ACP') || !isset($PHP_SELF)) {
	exit('Access Denied (menu.cfg.php)');
}



function acpcategorys(){
	return array(
		'default',
		'config',
		'bookbase',
		'story',
		//'database',
		'user',
                'financial',
		//'style',
		'tools',
		'external',
		'official',
	);
}

function acpmenu(){
	return array(
		'default' => array(
			'shortcut' => array(
			),
			'sysfunc' => array(
				'announce',
				'focus',
				'help',
			),
		),
		'config' => array(
			'config' => array(
				'config_system',
				'config_user',
				//'config_author',
				//'config_story',
				'config_read',
				'config_comment',
			),
			'config_advanced' => array(
				'config_upload',
				'config_fopen',
				'config_seo',
				'config_safe',
				'config_info',
				'config_rank',
				//'config_official',
			),
			'config_gui' => array(
				'config_display',
				'style_choose',
				'style_cache',
			),
			'config_com' => array(
				'config_pay',
				'config_vip',
				'config_mobile',
			),
		),
		'bookbase' => array(
			'collect' => array(
				'collect_single',
				'collect_multi',
				'collect_single_rule',
				'collect_multi_rule',
				'collect_blacklist',
			),
			'bookbase' => array(
				'setworks',
				//'setcommend',
				'category_novel',
				'comment_novel',
				'setbookupdate',
				'setzips',
				/*'bookrecovery',
				'bookbackup',*/
				'checkbook',
			),
			'commend' => array(
				'novel_commend_face',
				'novel_commend_subject',
				'novel_commend_strong',
				'novel_commend_editor',
				'novel_commend_vip',
			),
		),
		'story' => array(
			'story' => array(
				'config_story',
				'manage_story',
				'category_story',
				'comment_story',
				'verify_story',
			),
			'commend' => array(
				'story_commend_face',
				'story_commend_subject',
				'story_commend_strong',
				'story_commend_editor',
				'story_commend_vip',
			),
		),
		'database' => array(
			'database' => array(
				'db_export',
				'db_import',
				'db_update',
				'db_optimize',
			),
		),
		'user' => array(
			'user' => array(
				'config_user',
				'user_add',
				'user_edit',
				'user_ban',
				'user_verify',
			),
			'author' => array(
				'config_author',
				//'config_vip',
				'author_add',
				'author_edit',
				'author_verify',
				'author_interview',
			),
			'typewriter' => array(
				//'config_typewriter',
				'typewriter_add',
				'typewriter_edit',
			),
			'admin' => array(
				'admin_manage',
			),
		),
		/*'style' => array(
			'style' => array(
				'setstyle',
				'setemplate',
				//'getstyles',
			),
		),*/
		'tools' => array(
			'tools' => array(
				'cache',
				'setbans',
			),
			'database' => array(
				'db_export',
				'db_import',
				'db_update',
				'db_optimize',
			),
			/*'count' => array(
				'count_global',
				'count_book',
				'count_writer',
			),*/
			'log' => array(
				'log_acp',
				'log_php',
				'log_mysql',
			),
		),
		'external' => array(
			'external' => array(
				'api',
				'js',
				'friend_link',
				'advertise',
			),
		),
		'official' => array(
			'official' => array(
				'config_official',
				'official_ask',
				'official_bug',
				'official_site',
				'official_manual',
				'official_info',
				'official_join',
				'download_rule',
			),
		),
		'financial' => array(
			'income' => array(
				'income_error_fix',
				'income_pay_list',
				'income_user_pay',
			),
                       'spend' => array(
				'author_charge',
				'author_balance',
			),

		),
);
}

function acpdefaultmenu(){
	return array(
		'default' => 'home',
		'config' => 'config_system',
		'bookbase' => 'setworks',
		'story' => 'manage_story',
		'user' => 'user_edit',
		'database' => 'db_export',
		'tools' => 'cache',
		'external' => 'js',
		'official' => 'official_site',
                'financial' => 'income_pay_list',
	);
}

/*function acpc_insert($menu, $data, $pos){
	$tmp = array();
	foreach($menu as $key => $val) {
		$tmp[] = $val;
		if($val == $pos) $tmp[] = $data;
	}
	return $tmp;
}

function acpm_insert($menu, $data, $pos, $list){
	$tmp = array();
	foreach($menu as $key => $val) {
		$tmp[$key] = $val;
		if($key == $pos) $tmp[$data] = $list;
	}
	return $tmp;
}*/
