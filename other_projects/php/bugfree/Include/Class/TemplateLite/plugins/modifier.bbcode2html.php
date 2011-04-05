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
function tpl_modifier_bbcode2html($data)
{
	$data = nl2br(stripslashes(addslashes($data)));



	$search = array("\n", "\r");
	$replace = array("", "");
	$data = str_replace($search, $replace, $data);

	$search = array(
		"/\[email\](.*?)\[\/email\]/si",
		"/\[email=(.*?)\](.*?)\[\/email\]/si",
		"/\[url\](.*?)\[\/url\]/si",
		"/\[url=(.*?)\]([^]]*?)\[\/url\]/si",
		"/\[img\](.*?)\[\/img\]/si",
		"/\[code\](.*?)\[\/code\]/si",
		"/\[pre\](.*?)\[\/pre\]/si",
		"/\[list\](.*?)\[\/list\]/si",
		"/\[\*\](.*?)/si",
		"/\[b\](.*?)\[\/b\]/si",
		"/\[i\](.*?)\[\/i\]/si",
		"/\[u\](.*?)\[\/u\]/si",
	);
	$replace = array(
		"<a href=\"mailto:\\1\">\\1</a>",
		"<a href=\"mailto:\\1\">\\2</a>",
		"<a href=\"\\1\" target=\"_blank\">\\1</a>",
		"<a href=\"\\1\" target=\"_blank\">\\2</a>",
		"<img src=\"\\1\" border=\"0\">",
		"<p><blockquote><font size=\"1\">code:</font><hr noshade size=\"1\"><pre>\\1</pre><br><hr noshade size=\"1\"></blockquote></p>",
		"<pre>\\1<br></pre>",
		"<ul>\\1</ul>",
		"<li>\\1</li>",
		"<b>\\1</b>",
		"<i>\\1</i>",
		"<u>\\1</u>",
	);
	$data = preg_replace($search, $replace, $data);

	$search = array(
		"/\[bug\](\d*?)\[\/bug\]/si",
		"/\[case\](\d*?)\[\/case\]/si",
		"/\[result\](\d*?)\[\/result\]/si",
	);
	$replace = array(
		"<a href=\"Bug.php?BugID=\\1\" target=\"_blank\">\\1</a>",
		"<a href=\"Case.php?CaseID=\\1\" target=\"_blank\">\\1</a>",
		"<a href=\"Result.php?ResultID=\\1\" target=\"_blank\">\\1</a>",
	);
	$data = preg_replace($search, $replace, $data);

	return $data;
}
?>
