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
	exit('Access Denied (collect_single_rule.php)');
}

require_once ROOT.'include/collect.func.php';

$db->single(__TAB_COLLECT_SINGLE_RULE__);


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

		$import = _collect_rule_import($import_text, false);

		$rule_exists = false;
		foreach(_collect_site() as $k => $v) {
			if($v['mark'] == $import['mark']) {
				$rule_exists = true;
				break;
			}
		}

		unset($import['id']);
		if($rule_exists) {
			$db->update($import, array(
				'mark' => $import['mark'],
			));
		}else {
			$db->insert($import);
		}

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

		_collect_rule_export($rule, false);
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

		$cv_pos = array();
		for($i = 1; $i <= 5; $i++) {
			$cv_pos[] = array(
				'v' => $i,
				'c' => $i == $rule['chapter_title_pos'] ? ' selected="selected"' : '',
				't' => '{read8cv'.$i.'}',
			);
		}

		$replaces   = html_show(unserialize(base64_decode($rule['replacements'])), false);
		$subjectpos = html_show(unserialize(base64_decode($rule['subject_pos'])),  false);
		$contentpos = html_show(unserialize(base64_decode($rule['content_pos'])),  false);

		$tpl = new AOKOTpl($pf.'_'.$ps.'_'.$action);
		$tpl->assign(html_show($rule, false));
		$tpl->assign(array(
			'subjects'    => $_CATEGORY['NOVEL']['subject'],
			'subject_pos' => $subjectpos,
			'contents'    => $_CATEGORY['NOVEL']['content'],
			'content_pos' => $contentpos,
			'cv_pos'      => $cv_pos,
			'replaces'    => $replaces,

			'image_open'  => $rule['image'] ? ' checked="checked"' : '',
			'image_close' => $rule['image'] ? '' : ' checked="checked"',

			'compression_auto'    => $rule['compression']      ? ''                   : ' checked="checked"',
			'compression_none'    => $rule['compression'] == 1 ? ' checked="checked"' : '',
			'compression_gzip'    => $rule['compression'] == 2 ? ' checked="checked"' : '',
			'compression_deflate' => $rule['compression'] == 3 ? ' checked="checked"' : '',

			'charsets_auto'   => $rule['charsets']      ? ''                   : ' checked="checked"',
			'charsets_utf8'   => $rule['charsets'] == 1 ? ' checked="checked"' : '',
			'charsets_gb2312' => $rule['charsets'] == 2 ? ' checked="checked"' : '',
			'charsets_big5'   => $rule['charsets'] == 3 ? ' checked="checked"' : '',
		));
		$tpl->output();
		break;
	case 'update':
		if(!$id || !ris_int($id))
			back('未知规则');

		if(empty($mark))
			back('站点标识不能为空');

		if(!preg_match('/^[A-Za-z0-9]+$/', $mark))
			back('站点标识只能使用字母和数字');

		if(_collect_mark($mark) != $id)
			back('站点标识不能重复');

		$columns = array(
			'mark'                => '',
			'title'               => '',
			'address'             => '',
			'page_info'           => '',
			'general_regexp_1'    => '',
			'title_start'         => '',
			'title_end'           => '',
			'title_regexp'        => '',
			'author_start'        => '',
			'author_end'          => '',
			'author_regexp'       => '',
			'subject_start'       => '',
			'subject_end'         => '',
			'subject_regexp'      => '',
			//'subject_pos'         => '',
			'content_start'       => '',
			'content_end'         => '',
			'content_regexp'      => '',
			//'content_pos'         => '',
			'intro_start'         => '',
			'intro_end'           => '',
			'intro_regexp'        => '',
			'cover_start'         => '',
			'cover_end'           => '',
			'cover_prefix'        => '',
			'cover_regexp'        => '',
			'page_list'           => '',
			'general_regexp_2'    => '',
			'volume_start'        => '',
			'volume_end'          => '',
			'volume_regexp'       => '',
			'chapter_link_start'  => '',
			'chapter_link_end'    => '',
			'chapter_title_start' => '',
			'chapter_title_end'   => '',
			'chapter_regexp'      => '',
			'chapter_title_pos'   => '',
			'page_read'           => '',
			'general_regexp_3'    => '',
			'page_final'          => '',
			'text_start'          => '',
			'text_end'            => '',
			'text_regexp'         => '',
			//'replacements'        => '',
			'image'               => '',
			'image_prefix'        => '',
			'compression'         => '',
			'charsets'            => '',
			//'lastupdate'          => '',
		);
		foreach($columns as $k => $v) {
			if(isset($$k)) {
				$columns[$k] = $$k;
			}
		}

		$subjectpos = array();
		if(!empty($subject_pos['from'])) {
			foreach($subject_pos['from'] as $k => $v) {
				if(!$v)
					continue;

				$subjectpos[] = array(
					'f' => stripslashes($v),
					't' => $subject_pos['to'][$k],
				);
			}
		}
		$columns['subject_pos']  = base64_encode(serialize($subjectpos));

		$contentpos = array();
		if(!empty($content_pos['from'])) {
			foreach($content_pos['from'] as $k => $v) {
				if(!$v)
					continue;

				$contentpos[] = array(
					'f' => stripslashes($v),
					't' => $content_pos['to'][$k],
				);
			}
		}
		$columns['content_pos']  = base64_encode(serialize($contentpos));

		$replaces = array();
		if(!empty($replacements['from'])) {
			foreach($replacements['from'] as $k => $v) {
				if(!$v)
					continue;

				$replaces[] = array(
					'f'    => stripslashes($v),
					't'    => stripslashes($replacements['to'][$k]),
					's'    => intval(isset($replacements['regexp'][$k]) ? $replacements['regexp'][$k] : 0),
					'func' => empty($replacements['regexp'][$k]) ? 'str_replace' : 'preg_replace',
				);
			}
		}
		$columns['replacements'] = base64_encode(serialize($replaces));

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
		if(empty($mark))
			back('站点标识不能为空');

		if(!preg_match('/^[A-Za-z0-9]+$/', $mark))
			back('站点标识只能使用字母和数字');

		if(_collect_mark($mark))
			back('站点标识不能重复');

		$columns = array(
			'mark'         => $mark,
			'title'        => empty($title) ? '' : $title,
			'subject_pos'  => '',
			'content_pos'  => '',
			'replacements' => '',
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

		$tpl->assign('rules', html_show(_collect_site()));

		$tpl->output();
}
