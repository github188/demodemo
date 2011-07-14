<?php
if(!defined('IN_Read8')) {
	exit('Access Denied (template_basic_navigation_ucp.php)');
}
?>
	<link rel="stylesheet" type="text/css" media="all" href="template/<?php
echo $_obj['G_tpldir'];
?>
/basic_my.css" />

	<div class="rd8_fullborder">
		<div id="rd8_navigation_ucp">
			 &nbsp;<a href="">返回首页</a> &nbsp;<?php
	if(!empty($_obj['categories'])) {
		if(!is_array($_obj['categories']))
			$_obj['categories'] = array(array('categories' => $_obj['categories']));
		$_tmp_arr_keys = array_keys($_obj['categories']);
		if($_tmp_arr_keys[0] != '0')
			$_obj['categories'] = array(0 => $_obj['categories']);
		$_stack[$_stack_cnt++] = $_obj;
		foreach($_obj['categories'] as $rowcnt=>$categories) {
			$categories['I'] = $rowcnt;
			$categories['II'] = $rowcnt+1;
			$categories['III'] = $rowcnt%7;
			$categories['F_uid'] = $GLOBALS['_assign']['F_uid'];
			$categories['F_username'] = $GLOBALS['_assign']['F_username'];
			$categories['G_sitename'] = $GLOBALS['_assign']['G_sitename'];
			$categories['G_siteurl'] = $GLOBALS['_assign']['G_siteurl'];
			$categories['G_style'] = $GLOBALS['_assign']['G_style'];
			$categories['G_tpldir'] = $GLOBALS['_assign']['G_tpldir'];
			$categories['G_PHPSELF'] = $GLOBALS['_assign']['G_PHPSELF'];
			$categories['G_encode'] = $GLOBALS['_assign']['G_encode'];
			$categories['TIMESTAMP'] = $GLOBALS['_assign']['TIMESTAMP'];
			$categories['A'] = $rowcnt % 2;
			$categories['B'] = $rowcnt % 2;
			$_obj = &$categories;
?><?php
if(!empty($_obj['c'])) {
?><span><?php
}else {
?><a href="my/<?php
echo $_obj['f'];
?>
.php"><?php
}
?><?php
echo $_obj['t'];
?>
<?php
if(!empty($_obj['c'])) {
?></span><?php
}else {
?></a><?php
}
?> &nbsp;<?php
}
$_obj=$_stack[--$_stack_cnt];}
?>
		</div>
	</div>
