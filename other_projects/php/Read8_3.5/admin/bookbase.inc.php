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
//后台书籍导入导出

if(!defined('IN_Read8') || !defined('IN_Read8_ACP') || !isset($PHP_SELF)) {
	exit('Access Denied');
}
if(!acppriv(11)) back('admin_no_privileges_bookbackup');
require_once ROOT.'include/booked.func.php';
function_exists('set_time_limit') && set_time_limit(300);
back('暂不提供');
$TH_version = SYS_VERSION;

$typenamearr=array(
	'full' => '全部备份',
	'bytype' => '单一类别',
	'manual' => '指定范围',
	'single' => '单本备份',
);

require_once ROOT.'include/zip.cls.php';
require_once ROOT.'include/unzip.cls.php';

$db->single(__TAB_BOOK__);

if($pfile=="bookbackup") {//---------------导出书籍-------------//只备份book/chapter/volumn,bookhit表另建
	if($action=="start") {//开始备份
		if(empty($type) || empty($datafile) || preg_match("/(\.)(exe|jsp|asp|aspx|cgi|fcgi|pl)(\.|$)/i", $datafile)) back('admin_module_bookbase_error');

		if($type=='full') {
			$sql="SELECT * FROM ".__TAB_BOOK__;
		}elseif($type=='bytype') {
			$sql="SELECT * FROM ".__TAB_BOOK__." WHERE type_id='$typeid'";
		}elseif($type=='manual') {
			$bookidarea=explode("-",$bookidarea);
			$sql="SELECT * FROM ".__TAB_BOOK__." WHERE id BETWEEN '$bookidarea[0]' AND '$bookidarea[1]'";
		}else {
			$sql="SELECT * FROM ".__TAB_BOOK__." WHERE id='$bookid'";
		}
		$rows = $db->selectBySql($sql);
		//先取得本次内容的全部sql语句
		$idstring = base64_encode("".TIMESTAMP.",$TH_version,$type");
		$timestamp = rdate(TIMESTAMP, 'Y-m-d H:i');
		$sqldump = $filecomm =
					"# Identify: $idstring\n".
					"# Read8.net BookBase Dump\n".
					"# Version: Read8.net $TH_version\n".
					"# Time: $timestamp\n".
					"# Type: $typenamearr[$type]\n".
					"#\n".
					"# Read8.net Home: http://www.Read8.net\n".
					"# Please visit our website for newest information about Read8\n".
					"# --------------------------------------------------------\n\n\n";
		$bookarr=array();

		//显示
		$tpl = new AOKOTpl("bookbase_backup");
		$tpl->assign("action",$action);
		$tpl->output();

		//--即时显示函数
		ob_end_flush();
		ob_implicit_flush();
		for($i = 1; $i <= 300; $i++ ) print(" ");

		//书籍内容打包
		foreach($rows as $row) {
			stopmark();
			$bookarr[$row['id']] = array($row['type_id'], $row['title']);
			$sqldump .= $row['id']."||".base64_encode($row['title'])."||".base64_encode($row['author'])."||".
						GetInitial($_CACHE['booktypebytid'][$row['type_id']]).
						"||".str_replace("\n", "<br />", $row['intro'])."||".$row['size']."\n";
			//下面导入章节和分卷
			$rows2 = $db->select(array(
				'field' => 'c.title, c.orderid, c.words, v.name, v.chapterorderid',
				'from'  => $db->table(__TAB_CHAPTER__, 'c'),
				'join'  => array('table' => $db->table(__TAB_VOLUME__, 'v'), 'on' => 'v.id = c.volume_id AND v.book_id = '.$row['id']),
				'where' => array('c.book_id' => $row['id']),
				'order' => 'c.orderid',
			));
			foreach($rows2 as $row2) {
				if($row2['orderid'] == $row2['chapterorderid']) $tmp = base64_encode($row2['name']);
				else $tmp = '';
				$sqldump .= base64_encode($row2['title'])."||".$row2['orderid']."||".$row2['words']."||$tmp\n";
			}
			$sqldump .= "\n\n";
		}
		echo '<script type="text/javascript">dl_info.innerHTML="<font class=\'down\'>完成</font>"+dl_info.innerHTML;</script>';

		//写入引导sql
		rfow(ROOT."data/backup/index.txt", "# Identify: $idstring\n时间: ".rdate(TIMESTAMP, 'Y-m-d H:i')."\n类型: ".$typenamearr[$type]);
		rfow(ROOT."data/backup/index.sql", $sqldump);
		unset($sqldump);

		$zip = new zip(ROOT."data/backup/$datafile");
		$zip -> addFile(ROOT."data/backup/index.txt",'index.txt',$idstring);
		$zip -> addFile(ROOT."data/backup/index.sql",'index.sql');
		foreach($bookarr as $key=>$val){
			stopmark();
			echo '<script type="text/javascript">dl_info.innerHTML="正在打包 <font class=\'down\'>'.htmlspecialchars($val[1]).' </font> ...<br />"+dl_info.innerHTML;</script>';

			$zip ->addDir($key);
			//列表html文件并加入
			$path=ROOT."html/$val[0]/$key";
			$htmlarr = rlistdir($path);
			foreach($htmlarr as $thishtml){
				$zip -> addFile("$path/$thishtml","$key/$thishtml");
			}
			unset($htmlarr);
			echo '<script type="text/javascript">dl_info.innerHTML="<font class=\'down\'>完成</font>"+dl_info.innerHTML;</script>';
		}
		$zip->save(convert($filecomm."# 本次打包书籍量为: ".count($bookarr), SYSCHARSET, 'gbk'));
		unlink(ROOT."data/backup/index.txt");
		unlink(ROOT."data/backup/index.sql");

		//日志记录
		$logcon="书籍备份<".$typenamearr[$type]." - $datafile>";
		AddLog($logcon);
		jumpage("?ptype=$ptype&pfile=$pfile",'admin_module_bookbase_success_backup');

	}

	//-------------显示主体-----------
	$booktypelist=array();
	foreach($_CACHE['booktypebytid'] as $key=>$val){
		$booktypelist[]=array(
			'C_key' => $key,
			'C_val' => $val,
		);
	}
	extract($db->selectOne(array(
		'field' => 'MAX(id) AS maxbookid',
	)));
	$tmpname = rdate(TIMESTAMP, 'md').'_'.rrand(8, 7).'.zip';
	//统计当前书籍数目
	extract($db->selectOne(array(
		'field' => 'COUNT(*) AS allbooknum',
	)));

	//显示
	$tpl = new AOKOTpl("bookbase_backup");
	foreach($typenamearr as $key => $val)
		$tpl->assign("C_type_$key", $val);

	$tpl->assign("action",       $action);
	$tpl->assign("C_allbooknum", $allbooknum);
	$tpl->assign("C_maxbookid",  $maxbookid);
	$tpl->assign("booktypelist", $booktypelist);
	$tpl->assign("C_tmpname",    $tmpname);
	$tpl->output();


}

if($pfile=="bookrecovery") {//---------------导入书籍-------------//
	if($action=='start') {//导入
		require_once ROOT.'include/booked.func.php';
		if($from == 'server') {
			$datafile=ROOT."data/backup/$datafile_server";
			if(!file_exists($datafile)) back('admin_module_bookbase_file_not_exists');
			$filename=$datafile;
		}else {
			$filename=$_FILES['datafile']['tmp_name'];
		}

		//显示
		$tpl = new AOKOTpl("bookbase_recovery");
		$tpl->assign("action",$action);
		$tpl->output();

		//--即时显示函数
		ob_end_flush();
		ob_implicit_flush();
		for($i = 1; $i <= 300; $i++ ) print(" ");

		//下面开始导入操作
		//类别索引
		foreach($_CACHE['booktypebytid'] as $key=>$val){
			$_CACHE['btbyinitial'][getinitial($val)]=$key;
		}
		$zip = new dunzip($filename);
		$zip->unzipall(ROOT."data/backup/importtemp");
		$zip->close();
		echo '<script type="text/javascript">dl_info.innerHTML="<font class=\'down\'>完成</font>"+dl_info.innerHTML;</script>';
		$sqlarr=explode("\n\n\n", rfor(ROOT."data/backup/importtemp/index.sql"));
		unset($sqlarr[0]);
		foreach($sqlarr as $thisbookinfo){
			stopmark();
			$binfoarr=explode("\n",$thisbookinfo);
			$bookinfo=explode("||",$binfoarr[0]);
			if(empty($bookinfo[0]) || empty($bookinfo[1])) continue;
			$bookname=base64_decode($bookinfo[1]);
			$bookauthor=base64_decode($bookinfo[2]);
			echo '<script type="text/javascript">dl_info.innerHTML="正在导入 <font class=\'bname\'>'.htmlspecialchars($bookname).'</font> ...<br />"+dl_info.innerHTML;</script>';

			$row = $db->selectOne(array(
				'field' => 'COUNT(c.id) AS maxnum',
				'from'  => $db->table(__TAB_BOOK__, 'b'),
				'join'  => array('table' => $db->table(__TAB_CHAPTER__, 'c'), 'on' => 'c.book_id = b.id'),
				'where' => array('b.title' => $bookname),
			));
			if($row['maxnum']>0) {//此书已有
				if(count($binfoarr)-1<=$row['maxnum']) {
					echo '<script type="text/javascript">dl_info.innerHTML="<font class=\'cacel\'>跳过</font>"+dl_info.innerHTML;</script>';
					continue;//跳过此书
				}
				echo '<script type="text/javascript">dl_info.innerHTML="<font class=\'cover\'>覆盖</font>"+dl_info.innerHTML;</script>';
				//删除已有书籍
				$row = $db->selectOne(array(
					'field' => 'type_id, id',
					'where' => array('title' => $bookname),
				));
				rrmdir(ROOT."html/".$row['type_id']."/".$row['id']);
				//删除数据库
				$sql=array(__TAB_CHAPTER__, __TAB_VOLUME__, __TAB_BOOKREVIEW__, __TAB_BOOKCASE__, __TAB_BOOKHITS__);
				foreach($sql as $val) {
					$db->delete(array(
						'book_id' => $row['id'],
					), $val);
				}
				$db->delete(array('id' => $row['id']));
			}
			//导入此书
			//新增书籍处理
			$booktypeid=$_CACHE['btbyinitial'][$bookinfo[3]];
			$booktypeid=is_numeric($booktypeid) ? $booktypeid : 1;
			$tbookinfoarr=array(
					'title'    => $bookname,
					'uid'      => $Global['F_uid'],
					'username' => $Global['F_username'],
					'author'   => $bookauthor,
					'type_id'  => $booktypeid,
					'intro'    => str_replace("<br />","\n",$bookinfo[4]),
					'attrib'   => '公开作品',
					'size'     => $bookinfo[5],
			);
			$bid=newbook($tbookinfoarr);
			//导入章节
			$tmp = rlistdir(ROOT."data/backup/importtemp/".$bookinfo[0]);
			//检查安全性
			foreach($tmp as $val){
				if(substr($val,-5)<>".html" && substr($val,-4)<>".jpg") back(
					"安全警告:要导入的书籍数据中含有非法文件:<br />书名: $bookname <br>编号:$bookinfo[0]<br>临时地址:data/backup/importtemp/$bookinfo[0]/$val");
			}
			//导入静态数据
			rrmdir(ROOT."html/$booktypeid/$bid");
			rename(ROOT."data/backup/importtemp/".$bookinfo[0], ROOT."html/$booktypeid/$bid");
			//导入章节mysql
			unset($binfoarr[0]);
			$vid=0;
			foreach($binfoarr as $thischapter){
				$chapterinfo=explode("||",$thischapter);
				if(empty($chapterinfo[0]) || empty($chapterinfo[1])) continue;
				//先插入卷
				if(!empty($chapterinfo[3])) {
					$vid = $db->insert(array(
						'book_id'        => $bid,
						'name'           => addslashes(base64_decode($chapterinfo[3])),
						'chapterorderid' => addslashes($chapterinfo[1]),
					), __TAB_VOLUME__);
				}
				$db->insert(array(
					'title'      => addslashes(base64_decode($chapterinfo[0])),
					'book_id'    => $bid,
					'orderid'    => addslashes($chapterinfo[1]),
					'dateline'   => TIMESTAMP,
					'updatetime' => TIMESTAMP,
					'volume_id'  => $vid,
					'isnew'      => 0,
					'md5'        => md5(base64_decode($chapterinfo[0])),
				), __TAB_CHAPTER__);
				if($sqlerror = $db->error()) break;
			}
			if($sqlerror) back("MySQL错误，错误信息如下:<br /> $sqlerror");
			reindex($bid);
			UpdateBookcid($bid);
			echo '<script type="text/javascript">dl_info.innerHTML="<font class=\'down\'>完成</font>"+dl_info.innerHTML;</script>';
		}
		//导入完毕，清除剩余的文件
		rrmdir(ROOT."data/backup/importtemp");

		//日志记录
		jumpage("?ptype=$ptype&pfile=$pfile",'admin_module_bookbase_success_recovery');

	}
	if($action=="delete" && is_array($delfilearr)) {//批量删除
		foreach($delfilearr as $val){
			stopmark();
			unlink(ROOT."data/backup/$val");
		}
		//日志记录
		$logcon="删除书籍备份<共".count($delfilearr)."个>";
		jumpage("?ptype=$ptype&pfile=$pfile",'admin_module_bookbase_success_backup_del');
	}


	//列表显示所有zip备份文件
	$sqldir = rlistdir(ROOT."data/backup");
	$exportlog = array();
	foreach($sqldir as $thiszip){
		stopmark();
		$thiszipurl=ROOT."data/backup/$thiszip";
		if(!preg_match("/\.zip$/i", $thiszipurl)) continue;
		$filesize = filesize($thiszipurl);
		//得到indentify识别内容
		$zip = new dunzip($thiszipurl);
		$zip->getList('index.txt');
		$con=$zip->unzip('index.txt');
		$zip->close();
		//显示操作
		$identify = explode(',', base64_decode(preg_replace("/^# Identify:\s*(\w+).*/s", "\\1", $con)));
		$exportlog[$identify[0]] = array(
			'version' => $identify[1],
			'type' => $identify[2],
			'filename' => $thiszip,
			'size' => sizecount($filesize)
		);
	}
	krsort($exportlog);
	reset($exportlog);
	$backuplist = array();
	$i= 0;
	foreach($exportlog as $key=>$val){
		stopmark();
		$val['dateline']=is_int($key)?rdate($key, 'Y-m-d H:i'):"未知";
		$val['type']=$typenamearr[$val['type']];
		$backuplist[]=array(
			'ptype' => $ptype,
			'pfile' => $pfile,
			'C_filename' => $val['filename'],
			'C_version' => $val['version'],
			'C_dateline' => $val['dateline'],
			'C_type' => $val['type'],
			'C_size' => $val['size'],
		);
	}

	//显示
	$tpl = new AOKOTpl("bookbase_recovery");
	$tpl->assign("action",$action);
	$tpl->assign("backuplist",$backuplist);
	$tpl->output();

}

