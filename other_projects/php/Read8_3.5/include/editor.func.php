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

function html_editor($id, $cHtml = '', $width = '638', $height = '343'){
	$cHtml = editor_show_replace($cHtml);
	$html = "<script language=\"javascript\" type=\"text/javascript\" src=\"include/editor/editor.js\"></script>";
	$html .="<link rel=\"Stylesheet\" href=\"include/editor/editor.css\" type=\"text/css\" media=\"all\" />";
	$html .="<div id=\"$id\"></div>";
	$html .="<script type=\"text/javascript\">function editor_init() {editor = new word(\"$id\" ,\"$cHtml\", \"$width\", \"$height\");}if(window.Event) {window.onload = editor_init;}else {setTimeout(editor_init, 100);}</script>";
	return $html;
}

function editor_show_replace($content){
	return str_replace(array('/','"',"\r\n","\n"), array('\/','\"',"\\n","\\n"), $content);
}

function editor_safe_replace($content){
	$tags = array(
		"'<iframe[^>]*?>.*?</iframe>'is",
		"'<frame[^>]*?>.*?</frame>'is",
		"'<script[^>]*?>.*?</script>'is",
		"'<head[^>]*?>.*?</head>'is",
		"'<title[^>]*?>.*?</title>'is",
		"'<meta[^>]*?>'is",
		"'<link[^>]*?>'is",
		"'<!-- r8artlbegin -->'is",
		"'<!-- r8artlend -->'is",
		"'<!--- r8artlbegin --->'is",
		"'<!--- r8artlend --->'is",
	);
	return preg_replace($tags, array("","","","","","","","","",), $content);
}
