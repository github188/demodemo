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
//ע���顢�������롢�����ȵ�ؼ��� ��ajax����

define('IN_Read8', true);
require_once str_replace(array('\\', '/'.basename(__FILE__)), array('/', ''), __FILE__).'/include/init.php';

switch($action) {
	case 'checkreg':
		if(!$_SYSTEM['USER']['open'] || $Global['F_uid'])
			exit;

		$hid = 0;

		if($row=$db->selectOne(array(
			'field' => 'uid',
			'from'  => __TAB_USER__,
			'where' => array('username' => $tpl_user),
		))) $hid = 2;//�����û���

		if(Banlist($tpl_user, 'name')) $hid = 3;//��ֹ�û���

		echo $hid;

		exit;
	case 'gethot':
		$fd = rforr('http://top.baidu.com/', false);
		$startpos = strpos($fd, 'ʮ��С˵���ư�') + strlen('ʮ��С˵���ư�');
		$endpos = strpos($fd, '</table>', $startpos);//��ȡ���
		$book_intro = trim(substr($fd, $startpos, $endpos - $startpos));
		preg_match_all("/href[^>]+> ([".chr(0xa1)."-".chr(0xff)."]+)<\/a>/", $book_intro, $c);
		$carr = array_slice($c[1], 0, 4);//̫��ؼ������ױ���Ϊ����
		$t = convert(implode('-',$carr), 'gbk', SYSCHARSET);
		$m = convert(implode(',',$carr), 'gbk', SYSCHARSET);
		exit("document.getElementById('ins$pos').disabled=false;document.getElementById('tmpseo$pos').value='${$pos}';");
}