<?php
/**
 * BugFree is free software under the terms of the FreeBSD License.
 *
 * export a file.
 *
 * @link        http://www.bugfree.org.cn
 * @package     BugFree
 */
/* Init BugFree system. */
require('Include/Init.inc.php');
$FileID = $_GET['FileID'];
$FileInfo = dbGetRow('TestFile','',"FileID = '{$FileID}'");
$FilePath = $_CFG['File']['UploadDirectory'] . '/' . $FileInfo['FileName'];
if(!preg_match("/" . $FileInfo['FileType'].'$/i',$FileInfo['FileTitle']))
{
    $FileName = $FileInfo['FileTitle'] . '.' . $FileInfo['FileType'];
}
else
{
    $FileName = $FileInfo['FileTitle'];
}

sysHeaderFile($FilePath,$FileName);

function sysHeaderFile($FilePath,$FileName = '')
{

    global $_CFG;

    if(!is_readable($FilePath))
    {
        die($FilePath . " is not readalble");
    }

    $FileTypeArray = array (
		'cdf' => 'application/x-cdf',
		'fif' => 'application/fractals',
		'spl' => 'application/futuresplash',
		'hta' => 'application/hta',
		'hqx' => 'application/mac-binhex40',
		'doc' => 'application/msword',
		'pdf' => 'application/pdf',
		'p10' => 'application/pkcs10',
		'p7m' => 'application/pkcs7-mime',
		'p7s' => 'application/pkcs7-signature',
		'cer' => 'application/x-x509-ca-cert',
		'crl' => 'application/pkix-crl',
		'ps' => 'application/postscript',
		'setpay' => 'application/set-payment-initiation',
		'setreg' => 'application/set-registration-initiation',
		'smi' => 'application/smil',
		'edn' => 'application/vnd.adobe.edn',
		'pdx' => 'application/vnd.adobe.pdx',
		'rmf' => 'application/vnd.adobe.rmf',
		'xdp' => 'application/vnd.adobe.xdp+xml',
		'xfd' => 'application/vnd.adobe.xfd+xml',
		'xfdf' => 'application/vnd.adobe.xfdf',
		'fdf' => 'application/vnd.fdf',
		'xls' => 'application/x-msexcel',
		'sst' => 'application/vnd.ms-pki.certstore',
		'pko' => 'application/vnd.ms-pki.pko',
		'cat' => 'application/vnd.ms-pki.seccat',
		'stl' => 'application/vnd.ms-pki.stl',
		'ppt' => 'application/x-mspowerpoint',
		'wpl' => 'application/vnd.ms-wpl',
		'rms' => 'video/vnd.rn-realvideo-secure',
		'rm' => 'application/vnd.rn-realmedia',
		'rmvb' => 'application/vnd.rn-realmedia-vbr',
		'rnx' => 'application/vnd.rn-realplayer',
		'rjs' => 'application/vnd.rn-realsystem-rjs',
		'rjt' => 'application/vnd.rn-realsystem-rjt',
		'rmj' => 'application/vnd.rn-realsystem-rmj',
		'rmx' => 'application/vnd.rn-realsystem-rmx',
		'rmp' => 'application/vnd.rn-rn_music_package',
		'rsml' => 'application/vnd.rn-rsml',
		'z' => 'application/x-compress',
		'tgz' => 'application/x-compressed',
		'etd' => 'application/x-ebx',
		'gz' => 'application/x-gzip',
		'ins' => 'application/x-internet-signup',
		'iii' => 'application/x-iphone',
		'jnlp' => 'application/x-java-jnlp-file',
		'latex' => 'application/x-latex',
		'nix' => 'application/x-mix-transfer',
		'mxp' => 'application/x-mmxp',
		'asx' => 'video/x-ms-asf-plugin',
		'wmd' => 'application/x-ms-wmd',
		'wmz' => 'application/x-ms-wmz',
		'p12' => 'application/x-pkcs12',
		'p7b' => 'application/x-pkcs7-certificates',
		'p7r' => 'application/x-pkcs7-certreqresp',
		'swf' => 'application/x-shockwave-flash',
		'sit' => 'application/x-stuffit',
		'tar' => 'application/x-tar',
		'man' => 'application/x-troff-man',
		'zip' => 'application/x-zip-compressed',
		'xml' => 'text/xml',
		'3gp' => 'video/3gpp-encrypted',
		'3g2' => 'video/3gpp2',
		'aiff' => 'audio/x-aiff',
		'au' => 'audio/basic',
		'mid' => 'midi/mid',
		'mp3' => 'audio/x-mpg',
		'm3u' => 'audio/x-mpegurl',
		'ra' => 'audio/x-realaudio',
		'wav' => 'audio/x-wav',
		'wax' => 'audio/x-ms-wax',
		'wma' => 'audio/x-ms-wma',
		'ram' => 'audio/x-pn-realaudio',
		'bmp' => 'image/bmp',
		'gif' => 'image/gif',
		'jpg' => 'image/pjpeg',
		'jpeg' => 'image/pjpeg',
		'png' => 'image/x-png',
		'tiff' => 'image/tiff',
		'rp' => 'image/vnd.rn-realpix',
		'ico' => 'image/x-icon',
		'xbm' => 'image/xbm',
		'css' => 'text/css',
		'323' => 'text/h323',
		'htm' => 'text/html',
		'html' => 'text/html',
		'uls' => 'text/iuls',
		'txt' => 'text/plain',
		'wsc' => 'text/scriptlet',
		'rt' => 'text/vnd.rn-realtext',
		'htt' => 'text/webviewhtml',
		'htc' => 'text/x-component',
		'iqy' => 'text/x-ms-iqy',
		'odc' => 'text/x-ms-odc',
		'rqy' => 'text/x-ms-rqy',
		'vcf' => 'text/x-vcard',
		'avi' => 'video/x-msvideo',
		'mpeg' => 'video/x-mpeg2a',
		'rv' => 'video/vnd.rn-realvideo',
		'wm' => 'video/x-ms-wm',
		'wmv' => 'video/x-ms-wmv',
		'wmx' => 'video/x-ms-wmx',
		'wvx' => 'video/x-ms-wvx',
                'mht' => 'message/rfc822');

	$Extence = sysGetFileType($FilePath);
	if(array_key_exists($Extence,$FileTypeArray))
	{
		$FileType = $FileTypeArray[$Extence];
	}
	else
	{
		$FileType = "application/force-download";
	}

	$HeaderName = $FileName=="" ? basename($FilePath) : $FileName;
    $RadomStr = md5($HeaderName);
    $HeaderName = str_replace(' ', $RadomStr, $HeaderName);
    if(preg_match('/MSIE/i',$_SERVER["HTTP_USER_AGENT"]))
    {
        $HeaderName = urlencode($HeaderName);
        $HeaderName = str_replace(urlencode($RadomStr), ' ', $HeaderName);
    }
    else
    {
        $HeaderName = str_replace($RadomStr, ' ', $HeaderName);
    }

    header("Content-type: {$FileType}");
    if(!in_array($Extence, $_CFG['NotForceDownloadFileType']))
    {
	    header("Content-Disposition: attachment; filename=\"{$HeaderName}\";");
    }
	header("Expires: 0");
	header("Cache-Control: must-revalidate, post-check=0,pre-check=0");
	header("Pragma: public");

    $file_content = file_get_contents($FilePath);
    echo $file_content;
    exit;
}
?>
