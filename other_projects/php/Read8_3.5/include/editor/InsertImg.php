<?php
send_result(4);
exit;
include_once "../init.php";
include_once ROOT."include/image.func.php";

if($Global['F_uid']==0) send_result(6); //是否登录
$Global['picturepath'] = ROOT."upload/picture/";
$Global['picturepathurl'] = "upload/picture/";
$month = date("Y-m-d",time())."/"; //获得目录名:格式年-月-日

$root = $Global['picturepath']; //存储文件基本路径

$baseUrl = $Global['picturepathurl']; //

$allowSize = 5*1024*1024; //允许上传文件最大值.单位:字节.


//------------判断&提交------------
$allowType = array("image/gif","image/pjpeg","image/x-png"); //允许上传附件类型
if(empty($_FILES) || $_FILES['NewFile']['name'] == '') send_result(3); //判断是否有文件提交
if(!in_array($_FILES['NewFile']['type'],$allowType)) send_result(1); //判断文件类型是否符合标准
if($_FILES['NewFile']['size'] > $allowSize) send_result(4); //判断文件大小是否属合标准
$newFileName = getname().".".get_type($_FILES['NewFile']['name']);
$uploadDir = $root.$month;


if (!file_exists($root)) mkdir($root);
if (!file_exists($uploadDir)) mkdir($uploadDir);

if(@move_uploaded_file($_FILES['NewFile']['tmp_name'],$uploadDir.$newFileName)) {
	AddMark($uploadDir.$newFileName);

	$filesize=getimagesize($uploadDir.$newFileName);
	$S_width=400;
	$S_height=316;
	if($filesize[0]>$S_width || $filesize[1]>$S_height)
	{
		$file = explode(".",$newFileName);
		$str = count($file);
		resizeimage($uploadDir.$newFileName, $S_width, $S_height, $uploadDir.$file[0]."_b.".$file[$str-1]);
		send_result(0, $Global['siteurl']."/".$baseUrl.$month, $file[0]."_b.".$file[$str-1], $newFileName);
	}else
	{
		send_result(0, $Global['siteurl']."/".$baseUrl.$month, $newFileName, $newFileName);
	}
}else {
	send_result(5) ;
}


//-----------函数定义------------
/**
 * Send Result to HTML
 *
 * @param int $N  Error Number:0,success.1,Invalid file type.3,file empty.4,file byte is too big.5 error
 * @param string $Url
 */
function send_result( $N , $Url = '', $previewpic = '', $basicpic = ''){
	echo "<script type=\"text/javascript\">" ;
	echo "window.parent.ErrorMsg(".$N.",'".$Url."','".$previewpic."','".$basicpic."');" ;
	echo "</script>" ;
	exit ;
}

function get_type($fileName){
	$path_parts = pathinfo($fileName);
	return $path_parts["extension"];
}
