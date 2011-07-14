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

if(!defined('IN_Read8')) exit('Access Denied');


class dunzip{
	var $fileName;
	var $compressedList; // You will problably use only this one!
	var $endOfCentral;   // End of central dir, contains ZIP Comments
	var $fh;
	var $zipSignature = "\x50\x4b\x03\x04"; // local file header signature
	var $dirSignature = "\x50\x4b\x01\x02"; // central dir header signature
	var $dirSignatureE= "\x50\x4b\x05\x06"; // end of central dir signature
	Function dunzip($fileName){
		$this->fileName       = $fileName;
		$this->compressedList =
		$this->endOfCentral   = Array();
	}
	Function getList($stopOnFile=false){
		if(sizeof($this->compressedList)){
			return $this->compressedList;
		}
		$fh = fopen($this->fileName, "r");
		$this->fh = &$fh;
		if(!$fh) return false;
		$ddTry = false;
		fseek($fh, 0);
		for(;;){
			$signature = fread($fh, 4);
			if(feof($fh)) break;
			if($signature == 'PK00')continue;
			if($signature == $this->zipSignature){
				$file['version_needed']     = unpack("v", fread($fh, 2)); // version needed to extract
				$file['general_bit_flag']   = unpack("v", fread($fh, 2)); // general purpose bit flag
				$file['compression_method'] = unpack("v", fread($fh, 2)); // compression method
				$file['lastmod_time']       = unpack("v", fread($fh, 2)); // last mod file time
				$file['lastmod_date']       = unpack("v", fread($fh, 2));  // last mod file date
				$file['crc-32']             = fread($fh, 4);              // crc-32
				$file['compressed_size']    = unpack("V", fread($fh, 4)); // compressed size
				$file['uncompressed_size']  = unpack("V", fread($fh, 4)); // uncompressed size
				$fileNameLength             = unpack("v", fread($fh, 2)); // filename length
				$extraFieldLength           = unpack("v", fread($fh, 2)); // extra field length
				$file['file_name']          = fread($fh, $fileNameLength[1]); // filename
				$file['extra_field']        = $extraFieldLength[1]?fread($fh, $extraFieldLength[1]):''; // extra field
				$file['contents-startOffset']= ftell($fh);
				fseek($fh, $file['compressed_size'][1], SEEK_CUR);
				$BINlastmod_date = str_pad(decbin($file['lastmod_date'][1]), 16, '0', STR_PAD_LEFT);
				$BINlastmod_time = str_pad(decbin($file['lastmod_time'][1]), 16, '0', STR_PAD_LEFT);
				$lastmod_dateY = bindec(substr($BINlastmod_date,  0, 7))+1980;
				$lastmod_dateM = bindec(substr($BINlastmod_date,  7, 4));
				$lastmod_dateD = bindec(substr($BINlastmod_date, 11, 5));
				$lastmod_timeH = bindec(substr($BINlastmod_time,   0, 5));
				$lastmod_timeM = bindec(substr($BINlastmod_time,   5, 6));
				$lastmod_timeS = bindec(substr($BINlastmod_time,  11, 5));
				$this->compressedList[$file['file_name']] = Array(
					'file_name'         =>$file['file_name'],
					'compression_method'=>$file['compression_method'][1],
					'version_needed'    =>$file['version_needed'][1],
					'lastmod_datetime'  =>mktime($lastmod_timeH, $lastmod_timeM, $lastmod_timeS, $lastmod_dateM, $lastmod_dateD, $lastmod_dateY),
					'crc-32'            =>str_pad(dechex(ord($file['crc-32'][3])), 2, '0', STR_PAD_LEFT).
										  str_pad(dechex(ord($file['crc-32'][2])), 2, '0', STR_PAD_LEFT).
										  str_pad(dechex(ord($file['crc-32'][1])), 2, '0', STR_PAD_LEFT).
										  str_pad(dechex(ord($file['crc-32'][0])), 2, '0', STR_PAD_LEFT),
					'compressed_size'   =>$file['compressed_size'][1],
					'uncompressed_size' =>$file['uncompressed_size'][1],
					'extra_field'       =>$file['extra_field'],
					'general_bit_flag'  =>str_pad(decbin($file['general_bit_flag'][1]), 8, '0', STR_PAD_LEFT),
					'contents-startOffset'=>$file['contents-startOffset']
				);
				if($stopOnFile) if($file['file_name'] == $stopOnFile) break;
			}
			elseif($signature == $this->dirSignature){
				$dir['version_madeby']      = unpack("v", fread($fh, 2)); // version made by
				$dir['version_needed']      = unpack("v", fread($fh, 2)); // version needed to extract
				$dir['general_bit_flag']    = unpack("v", fread($fh, 2)); // general purpose bit flag
				$dir['compression_method']  = unpack("v", fread($fh, 2)); // compression method
				$dir['lastmod_time']        = unpack("v", fread($fh, 2)); // last mod file time
				$dir['lastmod_date']        = unpack("v", fread($fh, 2)); // last mod file date
				$dir['crc-32']              = fread($fh, 4);              // crc-32
				$dir['compressed_size']     = unpack("V", fread($fh, 4)); // compressed size
				$dir['uncompressed_size']   = unpack("V", fread($fh, 4)); // uncompressed size
				$fileNameLength             = unpack("v", fread($fh, 2)); // filename length
				$extraFieldLength           = unpack("v", fread($fh, 2)); // extra field length
				$fileCommentLength          = unpack("v", fread($fh, 2)); // file comment length
				$dir['disk_number_start']   = unpack("v", fread($fh, 2)); // disk number start
				$dir['internal_attributes'] = unpack("v", fread($fh, 2)); // internal file attributes-byte1
				$dir['external_attributes1']= unpack("v", fread($fh, 2)); // external file attributes-byte2
				$dir['external_attributes2']= unpack("v", fread($fh, 2)); // external file attributes
				$dir['relative_offset']     = unpack("V", fread($fh, 4)); // relative offset of local header
				$dir['file_name']           = fread($fh, $fileNameLength[1]);                             // filename
				$dir['extra_field']         = $extraFieldLength[1] ?fread($fh, $extraFieldLength[1]) :'';   // extra field
				$dir['file_comment']        = $fileCommentLength[1]?fread($fh, $fileCommentLength[1]):''; // file comment
				$BINlastmod_date = str_pad(decbin($file['lastmod_date'][1]), 16, '0', STR_PAD_LEFT);
				$BINlastmod_time = str_pad(decbin($file['lastmod_time'][1]), 16, '0', STR_PAD_LEFT);
				$lastmod_dateY = bindec(substr($BINlastmod_date,  0, 7))+1980;
				$lastmod_dateM = bindec(substr($BINlastmod_date,  7, 4));
				$lastmod_dateD = bindec(substr($BINlastmod_date, 11, 5));
				$lastmod_timeH = bindec(substr($BINlastmod_time,   0, 5));
				$lastmod_timeM = bindec(substr($BINlastmod_time,   5, 6));
				$lastmod_timeS = bindec(substr($BINlastmod_time,  11, 5));
			}
			elseif($signature == $this->dirSignatureE){
				$eodir['disk_number_this']   = unpack("v", fread($fh, 2)); // number of this disk
				$eodir['disk_number']        = unpack("v", fread($fh, 2)); // number of the disk with the start of the central directory
				$eodir['total_entries_this'] = unpack("v", fread($fh, 2)); // total number of entries in the central dir on this disk
				$eodir['total_entries']      = unpack("v", fread($fh, 2)); // total number of entries in
				$eodir['size_of_cd']         = unpack("V", fread($fh, 4)); // size of the central directory
				$eodir['offset_start_cd']    = unpack("V", fread($fh, 4)); // offset of start of central directory with respect to the starting disk number
				$zipFileCommentLenght        = unpack("v", fread($fh, 2)); // zipfile comment length
				$eodir['zipfile_comment']    = $zipFileCommentLenght[1]?fread($fh, $zipFileCommentLenght[1]):''; // zipfile comment
				$this->endOfCentral = Array(
					'disk_number_this'=>$eodir['disk_number_this'][1],
					'disk_number'=>$eodir['disk_number'][1],
					'total_entries_this'=>$eodir['total_entries_this'][1],
					'total_entries'=>$eodir['total_entries'][1],
					'size_of_cd'=>$eodir['size_of_cd'][1],
					'offset_start_cd'=>$eodir['offset_start_cd'][1],
					'zipfile_comment'=>$eodir['zipfile_comment'],
				);
			}
			else{
				if(!$ddTry){
					$ddTry = true;
					fseek($fh, 12-4, SEEK_CUR); // Jump over 'crc-32'(4) 'compressed-size'(4), 'uncompressed-size'(4)
					continue;
				}
				break;
			}
			$ddTry = false;
		}
		return $this->compressedList;
	}
	Function getExtraInfo($compressedFileName){
		return
			isset($this->centralDirList[$compressedFileName])?
			$this->centralDirList[$compressedFileName]:
			false;
	}
	Function getZipInfo($detail=false){
		return $detail?
			$this->endOfCentral[$detail]:
			$this->endOfCentral;
	}
	Function unzip($compressedFileName, $targetFileName=false){
		$fdetails = &$this->compressedList[$compressedFileName];
		if(!sizeof($this->compressedList)) $this->getList(false, $compressedFileName);
		if(!isset($this->compressedList[$compressedFileName])) return false;
		if(substr($compressedFileName, -1) == "/") return false;
		if(!$fdetails['uncompressed_size']) return $targetFileName?	rfow($targetFileName, ""):"";
		fseek($this->fh, $fdetails['contents-startOffset']);
		return $this->uncompress(
				fread($this->fh, $fdetails['compressed_size']),
				$fdetails['compression_method'],
				$fdetails['uncompressed_size'],
				$targetFileName
			);
	}
	Function unzipall($targetDir=false, $baseDir="", $maintainStructure=true, $chmod=false){
		if($targetDir === false) $targetDir = dirname(__FILE__)."/";
		$lista = $this->getList();
		if(sizeof($lista)) foreach($lista as $fileName=>$trash){
			$dirname  = dirname($fileName);
			$outDN    = "$targetDir/$dirname";
			if(substr($dirname, 0, strlen($baseDir)) != $baseDir) continue;
			if(!is_dir($outDN) && $maintainStructure){
				$str = "";
				$folders = explode("/", $dirname);
				foreach($folders as $folder){
					$str = $str?"$str/$folder":$folder;
					if(!is_dir("$targetDir/$str")){
						mkdir("$targetDir/$str");
						if($chmod) chmod("$targetDir/$str", $chmod);
					}
				}
			}
			if(substr($fileName, -1, 1) == "/") continue;
			$maintainStructure?
				$this->unzip($fileName, "$targetDir/$fileName"):
				$this->unzip($fileName, "$targetDir/".basename($fileName));
			if($chmod)
				chmod($maintainStructure?"$targetDir/$fileName":"$targetDir/".basename($fileName), $chmod);
		}
	}

	Function close(){ // Free the file resource
		if($this->fh) fclose($this->fh);
	}
	Function uncompress($content, $mode, $uncompressedSize, $targetFileName=false){
		switch($mode){
			case 0:
				return $targetFileName?	rfow($targetFileName, $content):	$content;
			case 1:
				return false;
			case 2:
			case 3:
			case 4:
			case 5:
				return false;
			case 6:
				return false;
			case 7:
				return false;
			case 8:
				return $targetFileName?
					rfow($targetFileName, gzinflate($content, $uncompressedSize)):
					gzinflate($content, $uncompressedSize);
			case 9:
				return false;
			case 10:
				return false;
			default:
				return false;
		}
	}
}
?>