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
if(!defined('IN_Read8') || !defined('IN_Read8_ACP') || !defined('IN_Read8_ACP_COLLECT') || !isset($PHP_SELF)) {
	exit('Access Denied (collect_multi_rule.php)');
}

require_once ROOT.'include/collect.func.php';

$db->single(__TAB_COLLECT_MULTI_RULE__);


switch($action) {
	case 'import':
		if(!empty($import_method)) {
			if(isset($_FILES['import_file']['error'])) {
				$import_error = upload_status($_FILES['import_file']['error'], true);
				if($import_error)
					back($import_error);

				$import_text = rfor($_FILES['import_file']['tmp_name']);
			}
		}else {
			$import_method = 0;
		}

		if(empty($import_text)) {
			if($import_method) {
				back('读取上传文件数据失败');
			}else {
				back('请将规则数据复制到文本框中');
			}
		}

		$import = _collect_rule_import($import_text, true);

		$parent_exists = false;
		foreach(_collect_site() as $k => $v) {
			if($v['mark'] == $import['parent']) {
				$parent_exists = true;
				break;
			}
		}

		unset($import['id']);
		if(empty($ignore_parent) && !$parent_exists)
			back('未找到基础规则 标识: '.$import['parent']);

		$db->insert($import);

		_clear_cache('collect_rule_');
		_clear_cache('collect_site_');
		_clear_cache('collect_mark_');

		jumpage("?ptype=$ptype&pfile=$pfile", '导入成功');
		break;
	case 'export':
		if(!$id || !ris_int($id))
			back('未知规则');

		$rule = $db->selectOne(array(
			'where' => array(
				'id' => $id,
			),
		));

		_collect_rule_export($rule, true);
		break;
	case 'delete':
		if(!$id || !ris_int($id))
			back('未知规则');

		$db->delete(array(
			'id' => $id,
		));

		_clear_cache('collect_rule_');
		_clear_cache('collect_site_');
		_clear_cache('collect_mark_');

		jumpage("?ptype=$ptype&pfile=$pfile", '删除成功');
		break;
	case 'edit':
		if(!$id || !ris_int($id))
			back('未知规则');

		$rule = $db->selectOne(array(
			'where' => array(
				'id' => $id,
			),
		));

		$sites = _collect_site();
		foreach($sites as $k => $v) {
			$sites[$k]['selected'] = $v['mark'] == $rule['parent'] ? ' selected="selected"' : '';
		}

		$tpl = new AOKOTpl($pf.'_'.$ps.'_'.$action);
		$tpl->assign(html_show($rule, false));
		$tpl->assign(array(
			'sites' => $sites,
/*
			'compression_auto'    => $rule['compression']      ? ''                   : ' checked="checked"',
			'compression_none'    => $rule['compression'] == 1 ? ' checked="checked"' : '',
			'compression_gzip'    => $rule['compression'] == 2 ? ' checked="checked"' : '',
			'compression_deflate' => $rule['compression'] == 3 ? ' checked="checked"' : '',

			'charsets_auto'   => $rule['charsets']      ? ''                   : ' checked="checked"',
			'charsets_utf8'   => $rule['charsets'] == 1 ? ' checked="checked"' : '',
			'charsets_gb2312' => $rule['charsets'] == 2 ? ' checked="checked"' : '',
			'charsets_big5'   => $rule['charsets'] == 3 ? ' checked="checked"' : '',
*/
		));
		$tpl->output();
		break;
	case 'update':
		if(!$id || !ris_int($id))
			back('未知规则');

		$columns = array(
			'title'      => '',
			'parent'     => '',
			'page_list'  => '',
			'page_start' => '',
			'page_end'   => '',
			'bid_start'  => '',
			'bid_end'    => '',
			'bid_regexp' => '',
			//'lastupdate' => '',
		);
		foreach($columns as $k => $v) {
			if(isset($$k)) {
				$columns[$k] = $$k;
			}
		}

		$columns['lastupdate']   = TIMESTAMP;

		$db->update($columns, array(
			'id' => $id,
		));

		_clear_cache('collect_rule_');
		_clear_cache('collect_site_');
		_clear_cache('collect_mark_');

		jumpage("?ptype=$ptype&pfile=$pfile", '修改成功');
		break;
	/*case 'add':
		$tpl = new AOKOTpl($pf.'_'.$ps.'_'.$action);
		$tpl->output();
		break;*/
	case 'insert':
		$columns = array(
			'title'        => empty($title) ? '' : $title,
			'lastupdate'   => TIMESTAMP,
		);

		$id = $db->insert($columns);

		_clear_cache('collect_rule_');
		_clear_cache('collect_site_');
		_clear_cache('collect_mark_');

		header('Location: ?ptype='.$ptype.'&pfile='.$pfile.'&action=edit&id='.$id);
		exit;
	default :
		$tpl = new AOKOTpl($pf.'_'.$ps);

		$tpl->assign('rules', html_show(_collect_site(true)));

		$tpl->output();
}
