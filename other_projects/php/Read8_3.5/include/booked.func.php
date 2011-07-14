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
//书籍操作定义

if(!defined('IN_Read8')) exit('Access Denied');

//ubb代码转换为html显示内容(只转换img和attach标签)
function UbbDecode($content){
	global $Global, $_SYSTEM;

	$content = str_replace(array("\r\n", "\n", '\r\n', '\n'), array('<br />', '<br />', '<br />', '<br />'), $content);
	$content = preg_replace("/\[img\](.*)\[\/img\]/isU", "<img src=\"$1\" alt=\"\" />", $content);
	return preg_replace("/\[attach\](\d+)\[\/attach\]/isU", "<img src=\"".$_SYSTEM['SYSTEM']['SITE_ADDR']."/download.php?aid=$1\" alt=\"\" />", $content);
}



//更新书籍的最新章节id
function UpdateBookcid($bid){
	global $db;
	//$sql="SELECT id FROM ".__TAB_CHAPTER__." WHERE book_id='$bid' ORDER BY orderid DESC LIMIT 1";
	//$row=$db->fetch_one($sql);
	$row = $db->selectOne(array(
		'field' => 'id',
		'from'  => __TAB_NOVEL_CHAPTER__,
		'where' => array('book_id' => $bid),
		'order' => 'dateline DESC',
	));
	$lcid=$row['id'];//上一章节绝对id
	//更新书籍最新章节记录
	//$sql="UPDATE ".__TAB_NOVEL__." SET lastchapter='$lcid' WHERE id='$bid'";
	//$db->query($sql);
	$db->update(array(
		'lastchapter' => $lcid,
	), array(
		'id'          => $bid,
	), __TAB_NOVEL__);
}

//更新书籍的大小
function UpdateBooksize($bid){
	global $db;
	//$sql="SELECT SUM(words) AS wordsnum FROM ".__TAB_CHAPTER__." WHERE book_id='$bid'";
	//$row=$db->fetch_one($sql);
	$row = $db->selectOne(array(
		'field' => 'SUM(words) AS wordsnum',
		'from'  => __TAB_CHAPTER__,
		'where' => array('book_id' => $bid),
	));
	//$sql="UPDATE ".__TAB_BOOK__." SET size=".$row['wordsnum']." WHERE id='$bid'";
	//$db->query($sql);
	$db->update(array(
		'size' => $row['wordsnum'],
	), array(
		'id'   => $bid,
	));
}



//跳转提示栏 ok
function JsTick($url, $timer = 3){
	echo "<div class='adminborder'>
			<div class='adminhead'>跳转状态栏</div>
			<div class='b0' style='width: 99%;padding-left: 1%;'><span id='jstickdiv'></span></div>
		</div>
<script type=\"text/javascript\">
	var second = $timer;
	function tick(){
		if(second > 0) {
			second--;
			document.getElementById('jstickdiv').innerHTML='等待 <span style=\"color: Red;\">' + second + '</span> 秒后继续...... &nbsp; &nbsp;不想等待: <a href=\"$url\">点击这里继续</a>';
			setTimeout(\"tick()\",1000);
		}else {
			self.location='$url';
		}
	}
	tick();
</script>";
	exit;
}



//附件名称生成  $extonly = 0只返回文件名 1只返回扩展名 2返回文件名和扩展名 ok
function attachname($filename,$extonly=0,$checksafe=0,$safexts=array('php','phtml','php3','jsp','asp','aspx','asa','cgi','fcgi','pl','exe','dll','htaccess',)){
	$fextname = strtolower(substr($filename,strrpos($filename,'.')+1));//扩展名

	if($checksafe && !$extonly)
		return in_array($fextname,$safexts);//检查

	if($extonly == 1)
		return $fextname;//只返回扩展名

	in_array($fextname,$safexts) && $fextname = '_'.$fextname;//保证扩展名安全
	$filename = str_replace(array('/','+','=',),array('','','',),base64_encode($filename));//去掉base64_encode后可能产生的/ + =有的主机限制+ =
	$fnamelen = strlen($filename);
	if($fnamelen > 10) {//长度控制
		$filename = substr($filename,0,10);
	}else {
		$filename .= rrand(10-$fnamelen);
	}
	$filename .= '_'.rrand(10).'.'.$fextname;//加入随机数 文件名
	if(!$extonly) return $filename;
	return array($filename,$fextname);
}

//更新附件函数 ok
function UpdateAttach($attachs,$saveastemp,$aid=0){
	if(!is_array($attachs)) back('bad_argument');

	global $db,$Global,$attachalt,$newattachalt,$cid,$pbid,$abschapterid,$chaptercontent,$addattachcon;

	if(!$aid) {
		foreach($attachs['tmp_name'] as $key => $val){
			if(empty($val)) continue;
			$filename = $attachs['name'][$key];
			$filesize = $attachs['size'][$key];
			$filemime = $attachs['type'][$key];

			list($filepath,$fextname) = attachname($filename,2);

			if($Global['attach_save_method']) {
				$path = ROOT.$Global['attach_dir'].'/'.rdate(TIMESTAMP, $Global['attach_save_method']);
				if(!is_dir($path)) {
					mkdir($path, 0777);
					$filepath = rdate(TIMESTAMP, $Global['attach_save_method']).'/'.$filepath;
				}
			}
			if($saveastemp) {
				$attachments[] = array(
					'alt'     => $newattachalt[$key],
					'file'    => $filepath,
					'name'    => $filename,
					'type'    => $filetype,
					'mime'    => $filemime,
					'size'    => $filesize,
					'content' => rfor($val),
				);
			}else {
				$sql = "INSERT INTO ".__TAB_ATTACH__." VALUES(NULL, '$pbid', '$abschapterid', '".TIMESTAMP."', '$filepath', '$newattachalt[$key]', '$filename', '$filemime', '$filesize', '0')";
				$attach_nextid = $db->execInBySql($sql);
				move_uploaded_file($val,ROOT."$Global[attach_dir]/$filepath");

				if(in_array($fextname,array('jpg','jpeg')) && $Global['imgmark'])
					AddMark(ROOT."$Global[attach_dir]/$filepath");

				if(!strstr($chaptercontent,"[attach]{$attach_nextid}[/attach]"))
					$addattachcon .= "\n[attach]{$attach_nextid}[/attach]";
			}
		}
	}else {
		$tmpattach = $attachs['tmp_name'][$aid];
		$filename  = $attachs['name'][$aid];
		$filesize  = $attachs['size'][$aid];
		$filemime  = $attachs['type'][$aid];

		list($filepath,$fextname) = attachname($filename,2);

		if($Global['attach_save_method']) {
			$path = ROOT.$Global['attach_dir'].'/'.rdate(TIMESTAMP, $Global['attach_save_method']);
			if(!is_dir($path)) {
				mkdir($path, 0777);
				$filepath = rdate(TIMESTAMP, $Global['attach_save_method']).'/'.$filepath;
			}
		}
		if($saveastemp) {
			$attachments[$aid] = array(
				'alt'     => $attachalt[$aid],
				'file'    => $filepath,
				'name'    => $filename,
				'type'    => $filetype,
				'mime'    => $filemime,
				'size'    => $filesize,
				'content' => rfor($tmpattach),
			);
		}else {
			//$sql = "SELECT file,hits FROM ".__TAB_ATTACH__." WHERE id='$aid'";
			//$row = $db->fetch_one($sql);
			$row = $db->selectOne(array(
				'field' => 'file, hits',
				'from'  => __TAB_ATTACH__,
				'where' => array('id' => $aid),
			));
			unlink(ROOT."$Global[attach_dir]/$row[file]");
			move_uploaded_file($tmpattach,ROOT."$Global[attach_dir]/$filepath");

			$sql = "REPLACE INTO ".__TAB_ATTACH__." VALUES('$aid', '$pbid', '$cid', '".TIMESTAMP."', '$filepath', '$attachalt[$aid]', '$filename', '$filemime', '$filesize', '$row[hits]')";
			$db->execUpBySql($sql);

			if(in_array($fextname,array('jpg','jpeg')) && $Global['imgmark'])
				AddMark(ROOT."$Global[attach_dir]/$filepath");
			//增加内容中的附件链接
			if(!strstr($chaptercontent,"[attach]{$aid}[/attach]"))
				$addattachcon .= "\n[attach]{$aid}[/attach]";
		}
	}
}
