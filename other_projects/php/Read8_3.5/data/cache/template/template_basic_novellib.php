<?php
if(!defined('IN_Read8')) {
	exit('Access Denied (template_basic_novellib.php)');
}
?>
	<div id="rd8_center">
		<div id="rd8_left">
			<div class="rd8_subnav">
				<div class="rd8_subject">
					<dl>
						<dt>按&nbsp;题&nbsp;材</dt>
						<dd class="current_dd"><a href="novellib.php?subject=0&amp;content=<?php
echo $_obj['ncontent'];
?>
&amp;initial=<?php
echo $_obj['ninitial'];
?>
&amp;words=<?php
echo $_obj['nwords'];
?>
">全部</a></dd> 
<?php
	if(!empty($_obj['subjects'])) {
		if(!is_array($_obj['subjects']))
			$_obj['subjects'] = array(array('subjects' => $_obj['subjects']));
		$_tmp_arr_keys = array_keys($_obj['subjects']);
		if($_tmp_arr_keys[0] != '0')
			$_obj['subjects'] = array(0 => $_obj['subjects']);
		$_stack[$_stack_cnt++] = $_obj;
		foreach($_obj['subjects'] as $rowcnt=>$subjects) {
			$subjects['I'] = $rowcnt;
			$subjects['II'] = $rowcnt+1;
			$subjects['III'] = $rowcnt%7;
			$subjects['F_uid'] = $GLOBALS['_assign']['F_uid'];
			$subjects['F_username'] = $GLOBALS['_assign']['F_username'];
			$subjects['G_sitename'] = $GLOBALS['_assign']['G_sitename'];
			$subjects['G_siteurl'] = $GLOBALS['_assign']['G_siteurl'];
			$subjects['G_style'] = $GLOBALS['_assign']['G_style'];
			$subjects['G_tpldir'] = $GLOBALS['_assign']['G_tpldir'];
			$subjects['G_PHPSELF'] = $GLOBALS['_assign']['G_PHPSELF'];
			$subjects['G_encode'] = $GLOBALS['_assign']['G_encode'];
			$subjects['TIMESTAMP'] = $GLOBALS['_assign']['TIMESTAMP'];
			$subjects['nsubject'] = $GLOBALS['_assign']['nsubject'];
			$subjects['ncontent'] = $GLOBALS['_assign']['ncontent'];
			$subjects['ninitial'] = $GLOBALS['_assign']['ninitial'];
			$subjects['nwords'] = $GLOBALS['_assign']['nwords'];
			$subjects['A'] = $rowcnt % 2;
			$subjects['B'] = $rowcnt % 2;
			$_obj = &$subjects;
?>
						<dd><a href="novellib.php?subject=<?php
echo $_obj['id'];
?>
&amp;content=<?php
echo $_obj['ncontent'];
?>
&amp;initial=<?php
echo $_obj['ninitial'];
?>
&amp;words=<?php
echo $_obj['nwords'];
?>
"<?php
if(!empty($_obj['selected'])) {
?> class="rd8_currentlink" <?php
}
?>><?php
echo $_obj['title'];
?>
</a></dd>
<?php
}
$_obj=$_stack[--$_stack_cnt];}
?>
					</dl>
				</div>
				<div class="rd8_contents">
					<dl>
						<dt>按&nbsp;内&nbsp;容</dt>
						<dd class="current_dd"><a href="novellib.php?subject=<?php
echo $_obj['nsubject'];
?>
&amp;content=0&amp;initial=<?php
echo $_obj['ninitial'];
?>
&amp;words=<?php
echo $_obj['nwords'];
?>
">全&nbsp;&nbsp;&nbsp;&nbsp;部</a></dd>
<?php
	if(!empty($_obj['contents'])) {
		if(!is_array($_obj['contents']))
			$_obj['contents'] = array(array('contents' => $_obj['contents']));
		$_tmp_arr_keys = array_keys($_obj['contents']);
		if($_tmp_arr_keys[0] != '0')
			$_obj['contents'] = array(0 => $_obj['contents']);
		$_stack[$_stack_cnt++] = $_obj;
		foreach($_obj['contents'] as $rowcnt=>$contents) {
			$contents['I'] = $rowcnt;
			$contents['II'] = $rowcnt+1;
			$contents['III'] = $rowcnt%7;
			$contents['F_uid'] = $GLOBALS['_assign']['F_uid'];
			$contents['F_username'] = $GLOBALS['_assign']['F_username'];
			$contents['G_sitename'] = $GLOBALS['_assign']['G_sitename'];
			$contents['G_siteurl'] = $GLOBALS['_assign']['G_siteurl'];
			$contents['G_style'] = $GLOBALS['_assign']['G_style'];
			$contents['G_tpldir'] = $GLOBALS['_assign']['G_tpldir'];
			$contents['G_PHPSELF'] = $GLOBALS['_assign']['G_PHPSELF'];
			$contents['G_encode'] = $GLOBALS['_assign']['G_encode'];
			$contents['TIMESTAMP'] = $GLOBALS['_assign']['TIMESTAMP'];
			$contents['nsubject'] = $GLOBALS['_assign']['nsubject'];
			$contents['ncontent'] = $GLOBALS['_assign']['ncontent'];
			$contents['ninitial'] = $GLOBALS['_assign']['ninitial'];
			$contents['nwords'] = $GLOBALS['_assign']['nwords'];
			$contents['A'] = $rowcnt % 2;
			$contents['B'] = $rowcnt % 2;
			$_obj = &$contents;
?>
						<dd><a href="novellib.php?subject=<?php
echo $_obj['nsubject'];
?>
&amp;content=<?php
echo $_obj['id'];
?>
&amp;initial=<?php
echo $_obj['ninitial'];
?>
&amp;words=<?php
echo $_obj['nwords'];
?>
"<?php
if(!empty($_obj['selected'])) {
?> class="rd8_currentlink" <?php
}
?>><?php
echo $_obj['title'];
?>
</a> </dd>
<?php
}
$_obj=$_stack[--$_stack_cnt];}
?>
					</dl>
				</div>
				<div class="rd8_initial" style="clear:both;">
					<dl>
						<dt>按&nbsp;首&nbsp;字&nbsp;母</dt>
						<dd class="current_dd"><a href="novellib.php?subject=<?php
echo $_obj['nsubject'];
?>
&amp;content=<?php
echo $_obj['ncontent'];
?>
&amp;initial=&amp;words=<?php
echo $_obj['nwords'];
?>
">[全&nbsp;&nbsp;部]</a></dd>
<?php
	if(!empty($_obj['initial'])) {
		if(!is_array($_obj['initial']))
			$_obj['initial'] = array(array('initial' => $_obj['initial']));
		$_tmp_arr_keys = array_keys($_obj['initial']);
		if($_tmp_arr_keys[0] != '0')
			$_obj['initial'] = array(0 => $_obj['initial']);
		$_stack[$_stack_cnt++] = $_obj;
		foreach($_obj['initial'] as $rowcnt=>$initial) {
			$initial['I'] = $rowcnt;
			$initial['II'] = $rowcnt+1;
			$initial['III'] = $rowcnt%7;
			$initial['F_uid'] = $GLOBALS['_assign']['F_uid'];
			$initial['F_username'] = $GLOBALS['_assign']['F_username'];
			$initial['G_sitename'] = $GLOBALS['_assign']['G_sitename'];
			$initial['G_siteurl'] = $GLOBALS['_assign']['G_siteurl'];
			$initial['G_style'] = $GLOBALS['_assign']['G_style'];
			$initial['G_tpldir'] = $GLOBALS['_assign']['G_tpldir'];
			$initial['G_PHPSELF'] = $GLOBALS['_assign']['G_PHPSELF'];
			$initial['G_encode'] = $GLOBALS['_assign']['G_encode'];
			$initial['TIMESTAMP'] = $GLOBALS['_assign']['TIMESTAMP'];
			$initial['nsubject'] = $GLOBALS['_assign']['nsubject'];
			$initial['ncontent'] = $GLOBALS['_assign']['ncontent'];
			$initial['ninitial'] = $GLOBALS['_assign']['ninitial'];
			$initial['nwords'] = $GLOBALS['_assign']['nwords'];
			$initial['A'] = $rowcnt % 2;
			$initial['B'] = $rowcnt % 2;
			$_obj = &$initial;
?>
					 <dd><a href="novellib.php?subject=<?php
echo $_obj['nsubject'];
?>
&amp;content=<?php
echo $_obj['ncontent'];
?>
&amp;initial=<?php
echo $_obj['v'];
?>
&amp;words=<?php
echo $_obj['nwords'];
?>
"<?php
if(!empty($_obj['selected'])) {
?>  class="rd8_currentlink"<?php
}
?>>[<?php
echo $_obj['v'];
?>
]</a></dd>
<?php
}
$_obj=$_stack[--$_stack_cnt];}
?>
					</dl>
				</div>
				<div class="rd8_word" style="clear:both;">
					<dl>
						<dt>按&nbsp;字&nbsp;数</dt>
						<dd class="current_dd"><a href="novellib.php?subject=<?php
echo $_obj['nsubject'];
?>
&amp;content=<?php
echo $_obj['ncontent'];
?>
&amp;initial=<?php
echo $_obj['ninitial'];
?>
&amp;words=">[全&nbsp;部]</a></dd>
<?php
	if(!empty($_obj['words'])) {
		if(!is_array($_obj['words']))
			$_obj['words'] = array(array('words' => $_obj['words']));
		$_tmp_arr_keys = array_keys($_obj['words']);
		if($_tmp_arr_keys[0] != '0')
			$_obj['words'] = array(0 => $_obj['words']);
		$_stack[$_stack_cnt++] = $_obj;
		foreach($_obj['words'] as $rowcnt=>$words) {
			$words['I'] = $rowcnt;
			$words['II'] = $rowcnt+1;
			$words['III'] = $rowcnt%7;
			$words['F_uid'] = $GLOBALS['_assign']['F_uid'];
			$words['F_username'] = $GLOBALS['_assign']['F_username'];
			$words['G_sitename'] = $GLOBALS['_assign']['G_sitename'];
			$words['G_siteurl'] = $GLOBALS['_assign']['G_siteurl'];
			$words['G_style'] = $GLOBALS['_assign']['G_style'];
			$words['G_tpldir'] = $GLOBALS['_assign']['G_tpldir'];
			$words['G_PHPSELF'] = $GLOBALS['_assign']['G_PHPSELF'];
			$words['G_encode'] = $GLOBALS['_assign']['G_encode'];
			$words['TIMESTAMP'] = $GLOBALS['_assign']['TIMESTAMP'];
			$words['nsubject'] = $GLOBALS['_assign']['nsubject'];
			$words['ncontent'] = $GLOBALS['_assign']['ncontent'];
			$words['ninitial'] = $GLOBALS['_assign']['ninitial'];
			$words['nwords'] = $GLOBALS['_assign']['nwords'];
			$words['A'] = $rowcnt % 2;
			$words['B'] = $rowcnt % 2;
			$_obj = &$words;
?>
						<dd><a href="novellib.php?subject=<?php
echo $_obj['nsubject'];
?>
&amp;content=<?php
echo $_obj['ncontent'];
?>
&amp;initial=<?php
echo $_obj['ninitial'];
?>
&amp;words=<?php
echo $_obj['v'];
?>
"<?php
if(!empty($_obj['selected'])) {
?>  class="rd8_currentlink"<?php
}
?>>[<?php
echo $_obj['v'];
?>
]</a></dd>
<?php
}
$_obj=$_stack[--$_stack_cnt];}
?>
					</dl>
				</div>
			</div>
		</div>

		<div id="rd8_right">
			<p class="rd8_guide">  当前所在位置：大众书库</p>
			<table>
				<tr class="rd8_texture">
					<td>作品名称</td>
					<td>最新章节</td>
					<td>类别</td>
					<td>作者</td>
					<td>更新时间</td>
				</tr>
<?php
	if(!empty($_obj['novels'])) {
		if(!is_array($_obj['novels']))
			$_obj['novels'] = array(array('novels' => $_obj['novels']));
		$_tmp_arr_keys = array_keys($_obj['novels']);
		if($_tmp_arr_keys[0] != '0')
			$_obj['novels'] = array(0 => $_obj['novels']);
		$_stack[$_stack_cnt++] = $_obj;
		foreach($_obj['novels'] as $rowcnt=>$novels) {
			$novels['I'] = $rowcnt;
			$novels['II'] = $rowcnt+1;
			$novels['III'] = $rowcnt%7;
			$novels['F_uid'] = $GLOBALS['_assign']['F_uid'];
			$novels['F_username'] = $GLOBALS['_assign']['F_username'];
			$novels['G_sitename'] = $GLOBALS['_assign']['G_sitename'];
			$novels['G_siteurl'] = $GLOBALS['_assign']['G_siteurl'];
			$novels['G_style'] = $GLOBALS['_assign']['G_style'];
			$novels['G_tpldir'] = $GLOBALS['_assign']['G_tpldir'];
			$novels['G_PHPSELF'] = $GLOBALS['_assign']['G_PHPSELF'];
			$novels['G_encode'] = $GLOBALS['_assign']['G_encode'];
			$novels['TIMESTAMP'] = $GLOBALS['_assign']['TIMESTAMP'];
			$novels['nsubject'] = $GLOBALS['_assign']['nsubject'];
			$novels['ncontent'] = $GLOBALS['_assign']['ncontent'];
			$novels['ninitial'] = $GLOBALS['_assign']['ninitial'];
			$novels['nwords'] = $GLOBALS['_assign']['nwords'];
			$novels['A'] = $rowcnt % 2;
			$novels['B'] = $rowcnt % 2;
			$_obj = &$novels;
?>
				<tr class="<?php
if(!empty($_obj['A'])) {
?>rd8_oddtablerow<?php
}else {
?>rd8_eventablerow<?php
}
?>" onmouseover="this.className='rd8_selectedrow'" onmouseout="this.className='<?php
if(!empty($_obj['A'])) {
?>rd8_oddtablerow<?php
}else {
?>rd8_eventablerow<?php
}
?>'">
					<td><a href="<?php
echo $_obj['InfoURL'];
?>
">《<?php
echo $_obj['title'];
?>
》</a></td>
					<td><a href="<?php
echo $_obj['ChapterURL'];
?>
"><?php
echo $_obj['chapter_title'];
?>
</a></td>
					<td><?php
echo $_obj['subject'];
?>
 - <?php
echo $_obj['content'];
?>
</td>
					<td><?php
echo $_obj['author'];
?>
</td>
					<td><?php
echo rdate($_obj['lastupdate'],'Y-m-d H:i:s');
?>
</td>
				</tr>
<?php
}
$_obj=$_stack[--$_stack_cnt];}
?>
			</table>
			<div class="rd8_tablebottom"><?php
echo $_obj['pagelink'];
?>
</div>
		</div>
	</div>
