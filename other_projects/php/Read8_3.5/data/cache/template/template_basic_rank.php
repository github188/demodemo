<?php
if(!defined('IN_Read8')) {
	exit('Access Denied (template_basic_rank.php)');
}
?>
<div class="rd8_main">
	<div class="rd8_full">
		<p class="rd8_guide">当前所在位置:  <?php
echo $_obj['rank_title'];
?>
</p>
		<div class="rd8_fullborder">
			<div class="rd8_col0">
				<table>
					<tr class="rd8_head">
						<td>作品名称</td>
						<td>作者</td>
						<td>更新时间</td>
						<td>类别</td>
						<td>状态</td>
						<td>总点击</td>
						<td>本周点击</td>
					</tr>
<?php
	if(!empty($_obj['ranklist'])) {
		if(!is_array($_obj['ranklist']))
			$_obj['ranklist'] = array(array('ranklist' => $_obj['ranklist']));
		$_tmp_arr_keys = array_keys($_obj['ranklist']);
		if($_tmp_arr_keys[0] != '0')
			$_obj['ranklist'] = array(0 => $_obj['ranklist']);
		$_stack[$_stack_cnt++] = $_obj;
		foreach($_obj['ranklist'] as $rowcnt=>$ranklist) {
			$ranklist['I'] = $rowcnt;
			$ranklist['II'] = $rowcnt+1;
			$ranklist['III'] = $rowcnt%7;
			$ranklist['F_uid'] = $GLOBALS['_assign']['F_uid'];
			$ranklist['F_username'] = $GLOBALS['_assign']['F_username'];
			$ranklist['G_sitename'] = $GLOBALS['_assign']['G_sitename'];
			$ranklist['G_siteurl'] = $GLOBALS['_assign']['G_siteurl'];
			$ranklist['G_style'] = $GLOBALS['_assign']['G_style'];
			$ranklist['G_tpldir'] = $GLOBALS['_assign']['G_tpldir'];
			$ranklist['G_PHPSELF'] = $GLOBALS['_assign']['G_PHPSELF'];
			$ranklist['G_encode'] = $GLOBALS['_assign']['G_encode'];
			$ranklist['TIMESTAMP'] = $GLOBALS['_assign']['TIMESTAMP'];
			$ranklist['A'] = $rowcnt % 2;
			$ranklist['B'] = $rowcnt % 2;
			$_obj = &$ranklist;
?>
					<tr align="center" class="rd8_col<?php
echo $_obj['A'];
?>
" onmouseover="this.className='rd8_col2'" onmouseout="this.className='rd8_col<?php
echo $_obj['A'];
?>
'">
						<td><a href="<?php
echo $_obj['InfoURL'];
?>
" rel="external"><?php
echo $_obj['title'];
?>
</a></td>
						<td><?php
if(!empty($_obj['author_id'])) {
?><a href="<?php
echo $_obj['AuthorURL'];
?>
" rel="external"><?php
}
?><?php
echo $_obj['author'];
?>
<?php
if(!empty($_obj['author_id'])) {
?></a><?php
}
?></td>
						<td><?php
echo rdate($_obj['lastupdate'],'Y-m-d H:i:s');
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
echo $_obj['state'];
?>
</td>
						<td><?php
echo $_obj['click_total'];
?>
</td>
						<td><?php
echo $_obj['click_week'];
?>
</td>
					</tr>
<?php
}
$_obj=$_stack[--$_stack_cnt];}
?>
				</table>
			</div>
		</div>
		<div class="rd8_tablebottom" style="clear:both;"><?php
if(!empty($_obj['pagelink'])) {
?><?php
echo $_obj['pagelink'];
?>
<?php
}else {
?>共一页<?php
}
?></div>
	</div>
</div>