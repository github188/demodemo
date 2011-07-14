<?php
if(!defined('IN_Read8')) {
	exit('Access Denied (template_basic_typewriter.php)');
}
?>

	<div class="rd8_fullborder">
		<div class="rd8_head">作品管理 => 书籍列表</div>
		<div class="rd8_col0">
			<table>
				<tr class="rd8_category">
					<td>书名</td>
					<td>授权</td>
					<td>类别</td>
					<td>字数</td>
					<td>状态</td>
					<td>时间</td>
					<td>最后更新</td>
					<td>管理</td>
				</tr>
<?php
	if(!empty($_obj['itemlist'])) {
		if(!is_array($_obj['itemlist']))
			$_obj['itemlist'] = array(array('itemlist' => $_obj['itemlist']));
		$_tmp_arr_keys = array_keys($_obj['itemlist']);
		if($_tmp_arr_keys[0] != '0')
			$_obj['itemlist'] = array(0 => $_obj['itemlist']);
		$_stack[$_stack_cnt++] = $_obj;
		foreach($_obj['itemlist'] as $rowcnt=>$itemlist) {
			$itemlist['I'] = $rowcnt;
			$itemlist['II'] = $rowcnt+1;
			$itemlist['III'] = $rowcnt%7;
			$itemlist['F_uid'] = $GLOBALS['_assign']['F_uid'];
			$itemlist['F_username'] = $GLOBALS['_assign']['F_username'];
			$itemlist['G_sitename'] = $GLOBALS['_assign']['G_sitename'];
			$itemlist['G_siteurl'] = $GLOBALS['_assign']['G_siteurl'];
			$itemlist['G_style'] = $GLOBALS['_assign']['G_style'];
			$itemlist['G_tpldir'] = $GLOBALS['_assign']['G_tpldir'];
			$itemlist['G_PHPSELF'] = $GLOBALS['_assign']['G_PHPSELF'];
			$itemlist['G_encode'] = $GLOBALS['_assign']['G_encode'];
			$itemlist['TIMESTAMP'] = $GLOBALS['_assign']['TIMESTAMP'];
			$itemlist['typepriv'] = $GLOBALS['_assign']['typepriv'];
			$itemlist['A'] = $rowcnt % 2;
			$itemlist['B'] = $rowcnt % 2;
			$_obj = &$itemlist;
?>
				<tr class="rd8_col<?php
echo $_obj['A'];
?>
" align="center" onmouseover="this.className='rd8_col2'" onmouseout="this.className='rd8_col<?php
echo $_obj['A'];
?>
'"> 
					<td><a href="novel.php?bid=<?php
echo $_obj['id'];
?>
" target="_blank">《<?php
echo $_obj['title'];
?>
》</a></td>
					<td><?php
echo $_obj['license'];
?>
</td>
					<td><?php
echo $_obj['subject'];
?>
 - <?php
echo $_obj['content'];
?>
</td>
					<td><?php
echo $_obj['words'];
?>
</td>
					<td><?php
echo $_obj['state'];
?>
</td>
					<td><?php
echo rdate($_obj['dateline'],'Y-m-d H:i:s');
?>
</td>
					<td><?php
if(!empty($_obj['lastupdate'])) {
?><?php
echo rdate($_obj['lastupdate'],'Y-m-d H:i:s');
?>
<?php
}else {
?>--<?php
}
?></td>
					<td>
						<a href="typewriter/?bid=<?php
echo $_obj['id'];
?>
&amp;action=novel_edit">编辑</a>&nbsp;
						<a href="typewriter/?bid=<?php
echo $_obj['id'];
?>
&amp;action=chapter_add">更新</a>&nbsp;
						<a href="typewriter/?bid=<?php
echo $_obj['id'];
?>
&amp;action=chapter_edit">管理</a>&nbsp;
						<a href="javascript:cfmoption('确实要删除该书吗？', 'typewriter/?bid=<?php
echo $_obj['id'];
?>
&amp;action=novel_del');">删除</a>
					</td>
				</tr>
<?php
}
$_obj=$_stack[--$_stack_cnt];}
?>
			</table>
		</div>
		<?php
if(!empty($_obj['pagelink'])) {
?><div class="rd8_colt"><?php
echo $_obj['pagelink'];
?>
</div><?php
}
?>
	</div>
