<?php
/**
 * Template_Lite bbcode modifier plugin
 *
 * Type:     modifier
 * Name:     bbcode2html
 * Purpose:  converts special bbcode syntax into standard html
 * Input:<br>
 *         - string: data to convert
 */
function tpl_modifier_overlib_bbcode2html($data)
{
    $data = str_replace(" ", "&nbsp;", $data);
	$data = nl2br(stripslashes(addslashes($data)));

	$search = array("\n", "\r", "[b]", "[/b]", "[i]", "[/i]", "[u]", "[/u]");
	$replace = array("", "", "<b>", "</b>", "<i>", "</i>", "<u>", "</u>");
	$data = str_replace($search, $replace, $data);

	$search = array("&lt;", "&gt;");
	$replace = array("&lt; ", "&gt; ");
	$data = str_replace($search, $replace, $data);

	$search = array("\\","\"","'");
	$replace = array("\\\\", '\\"', "\\'");
	$data = str_replace($search, $replace, $data);

	return $data;
}
?>
