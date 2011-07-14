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
	exit('Access Denied (focus.php)');
}



$db->single(__TAB_FOCUS__);

if(isset($_POST['submit']) || isset($_GET['submit'])) {
	if(isset($_GET['delete']) && ris_int($_GET['delete'])) {
		$db->update(array(
			'active' => 0,
		), array(
			'id'     => $_GET['delete'],
		));
	}else {
		if(empty($_FILES['upload']['name'])) {
			back('请选择文件');
		}elseif($_FILES['upload']['error']) {
			back('上传出错');
		}elseif('jpg' != substr($_FILES['upload']['name'], strrpos($_FILES['upload']['name'], '.') + 1)) {
			back('只能使用jpg格式图片');
		}else {
			$folder = 'upload/focus';
			rmkdir($folder);//创建目录

			list($usec, $sec) = explode(' ', microtime());
			$file = date('YmdHis').floor($usec * 100000).'.jpg';//生成文件名

			if(rmove_uploaded_file($_FILES['upload']['tmp_name'], ROOT.$folder.'/'.$file)) {
				$db->insert(array(
					'url'         => $folder.'/'.$file,
					'description' => $description,
					'link'        => $link,
					'dateline'    => TIMESTAMP,
				));
			}else {
				back('上传出错');
			}
		}
	}
	_cache_update('focus');
	jumpage("?ptype=$ptype&pfile=$pfile", '修改成功');
}

$pics = $db->select(array(
	'where' => array(
		'active' => 1,
	),
	'order' => 'dateline DESC',
	'limit' => 20,
));


$tpl = new AOKOTpl('focus');
$tpl->assign('booklist', $pics);
$tpl->output();