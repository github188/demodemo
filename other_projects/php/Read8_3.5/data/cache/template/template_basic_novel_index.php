<?php
if(!defined('IN_Read8')) {
	exit('Access Denied (template_basic_novel_index.php)');
}
?>
	<div id="rd8_center">			
		<h1 class="rd8_subguide">您正在浏览：<acronym><?php
echo $_obj['nc_title'];
?>
</acronym>频道</h1>
			<div id="rd8_left" style="margin-top:7px;">
				<div class="rd8_subnav">
					<div class="rd8_subject">
						<dl>
							<dt>按&nbsp;题&nbsp;材</dt>
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
			$subjects['nc'] = $GLOBALS['_assign']['nc'];
			$subjects['A'] = $rowcnt % 2;
			$subjects['B'] = $rowcnt % 2;
			$_obj = &$subjects;
?>
							<dd><a href="index.php?nc=<?php
echo $_obj['id'];
?>
"><?php
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
			$contents['nc'] = $GLOBALS['_assign']['nc'];
			$contents['A'] = $rowcnt % 2;
			$contents['B'] = $rowcnt % 2;
			$_obj = &$contents;
?>
							<dd><a href="novellib.php?subject=<?php
echo $_obj['nc'];
?>
&amp;content=<?php
echo $_obj['id'];
?>
"><?php
echo $_obj['title'];
?>
</a></dd>
<?php
}
$_obj=$_stack[--$_stack_cnt];}
?>
						</dl>
					</div>
					<div class="rd8_initial" style="clear:both;">
						<dl>
							<dt>按&nbsp;首&nbsp;字&nbsp;母</dt>
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
			$initial['nc'] = $GLOBALS['_assign']['nc'];
			$initial['A'] = $rowcnt % 2;
			$initial['B'] = $rowcnt % 2;
			$_obj = &$initial;
?>
							<dd><a href="novellib.php?subject=<?php
echo $_obj['nc'];
?>
&amp;initial=<?php
echo $_obj['v'];
?>
">[<?php
echo $_obj['v'];
?>
]</a></dd>
<?php
}
$_obj=$_stack[--$_stack_cnt];}
?>
						</dl>
					</div>
					<div class="rd8_subject" style="clear:both;">
						<dl>
							<dt>按&nbsp;字&nbsp;数</dt>
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
			$words['nc'] = $GLOBALS['_assign']['nc'];
			$words['A'] = $rowcnt % 2;
			$words['B'] = $rowcnt % 2;
			$_obj = &$words;
?>
							<dd><a href="novellib.php?subject=<?php
echo $_obj['nc'];
?>
&amp;words=<?php
echo $_obj['v'];
?>
">[<?php
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

			<div id="rd8_middle">
				<p class="rd8_catetitle">最新上架</p>
					<ul>
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
			$novels['nc'] = $GLOBALS['_assign']['nc'];
			$novels['A'] = $rowcnt % 2;
			$novels['B'] = $rowcnt % 2;
			$_obj = &$novels;
?>
						<li<?php
if(!empty($_obj['A'])) {
?> class="rd8_evenrow"<?php
}
?>><span class="rd8_lisubject">[<?php
echo $_obj['content'];
?>
]</span><a href="<?php
echo $_obj['InfoURL'];
?>
"> 《<?php
echo $_obj['title'];
?>
》 <?php
echo $_obj['chapter_title'];
?>
 </a><span class="rd8_lidate">[<?php
echo rdate($_obj['lastupdate'],'m-d');
?>
]</span><span class="rd8_liauthor"><?php
echo $_obj['author'];
?>
</span></li>
<?php
}
$_obj=$_stack[--$_stack_cnt];}
?>
					</ul>
					<small><a href="novellib.php?subject=<?php
echo $_obj['nc'];
?>
">查看所有图书&gt;&gt</a></small>
			</div>

			<div id="rd8_subright">
				<div class="rd8_Canada">
					<dl>
						<dt>本类总推荐榜</dt>
	<?php
	if(!empty($_obj['novel_total_commend'])) {
		if(!is_array($_obj['novel_total_commend']))
			$_obj['novel_total_commend'] = array(array('novel_total_commend' => $_obj['novel_total_commend']));
		$_tmp_arr_keys = array_keys($_obj['novel_total_commend']);
		if($_tmp_arr_keys[0] != '0')
			$_obj['novel_total_commend'] = array(0 => $_obj['novel_total_commend']);
		$_stack[$_stack_cnt++] = $_obj;
		foreach($_obj['novel_total_commend'] as $rowcnt=>$novel_total_commend) {
			$novel_total_commend['I'] = $rowcnt;
			$novel_total_commend['II'] = $rowcnt+1;
			$novel_total_commend['III'] = $rowcnt%7;
			$novel_total_commend['F_uid'] = $GLOBALS['_assign']['F_uid'];
			$novel_total_commend['F_username'] = $GLOBALS['_assign']['F_username'];
			$novel_total_commend['G_sitename'] = $GLOBALS['_assign']['G_sitename'];
			$novel_total_commend['G_siteurl'] = $GLOBALS['_assign']['G_siteurl'];
			$novel_total_commend['G_style'] = $GLOBALS['_assign']['G_style'];
			$novel_total_commend['G_tpldir'] = $GLOBALS['_assign']['G_tpldir'];
			$novel_total_commend['G_PHPSELF'] = $GLOBALS['_assign']['G_PHPSELF'];
			$novel_total_commend['G_encode'] = $GLOBALS['_assign']['G_encode'];
			$novel_total_commend['TIMESTAMP'] = $GLOBALS['_assign']['TIMESTAMP'];
			$novel_total_commend['nc'] = $GLOBALS['_assign']['nc'];
			$novel_total_commend['A'] = $rowcnt % 2;
			$novel_total_commend['B'] = $rowcnt % 2;
			$_obj = &$novel_total_commend;
?>
						<dd><a href="<?php
echo $_obj['InfoURL'];
?>
"><?php
echo $_obj['title'];
?>
</a>(<?php
echo $_obj['vote_total'];
?>
)</dd>
	<?php
}
$_obj=$_stack[--$_stack_cnt];}
?>
					</dl>	
					<span><a href="rank.php?times=total&amp;types=commend&amp;sc=<?php
echo $_obj['nc'];
?>
">更多&gt;&gt;</a></span>
				</div>

				<div class="rd8_Canada">
					<dl>
						<dt>本类总点击榜</dt>
<?php
	if(!empty($_obj['novel_total_click'])) {
		if(!is_array($_obj['novel_total_click']))
			$_obj['novel_total_click'] = array(array('novel_total_click' => $_obj['novel_total_click']));
		$_tmp_arr_keys = array_keys($_obj['novel_total_click']);
		if($_tmp_arr_keys[0] != '0')
			$_obj['novel_total_click'] = array(0 => $_obj['novel_total_click']);
		$_stack[$_stack_cnt++] = $_obj;
		foreach($_obj['novel_total_click'] as $rowcnt=>$novel_total_click) {
			$novel_total_click['I'] = $rowcnt;
			$novel_total_click['II'] = $rowcnt+1;
			$novel_total_click['III'] = $rowcnt%7;
			$novel_total_click['F_uid'] = $GLOBALS['_assign']['F_uid'];
			$novel_total_click['F_username'] = $GLOBALS['_assign']['F_username'];
			$novel_total_click['G_sitename'] = $GLOBALS['_assign']['G_sitename'];
			$novel_total_click['G_siteurl'] = $GLOBALS['_assign']['G_siteurl'];
			$novel_total_click['G_style'] = $GLOBALS['_assign']['G_style'];
			$novel_total_click['G_tpldir'] = $GLOBALS['_assign']['G_tpldir'];
			$novel_total_click['G_PHPSELF'] = $GLOBALS['_assign']['G_PHPSELF'];
			$novel_total_click['G_encode'] = $GLOBALS['_assign']['G_encode'];
			$novel_total_click['TIMESTAMP'] = $GLOBALS['_assign']['TIMESTAMP'];
			$novel_total_click['nc'] = $GLOBALS['_assign']['nc'];
			$novel_total_click['A'] = $rowcnt % 2;
			$novel_total_click['B'] = $rowcnt % 2;
			$_obj = &$novel_total_click;
?>
						<dd><a href="<?php
echo $_obj['InfoURL'];
?>
"><?php
echo $_obj['title'];
?>
</a>(<?php
echo $_obj['click_total'];
?>
)</dd>
<?php
}
$_obj=$_stack[--$_stack_cnt];}
?>
					</dl>
					<span><a href="rank.php?times=total&amp;types=click&amp;sc=<?php
echo $_obj['nc'];
?>
">更多&gt;&gt;</a></span>
				</div>

				<div class="rd8_Canada">
					<dl>
						<dt>本类总收藏榜</dt>
<?php
	if(!empty($_obj['novel_total_favo'])) {
		if(!is_array($_obj['novel_total_favo']))
			$_obj['novel_total_favo'] = array(array('novel_total_favo' => $_obj['novel_total_favo']));
		$_tmp_arr_keys = array_keys($_obj['novel_total_favo']);
		if($_tmp_arr_keys[0] != '0')
			$_obj['novel_total_favo'] = array(0 => $_obj['novel_total_favo']);
		$_stack[$_stack_cnt++] = $_obj;
		foreach($_obj['novel_total_favo'] as $rowcnt=>$novel_total_favo) {
			$novel_total_favo['I'] = $rowcnt;
			$novel_total_favo['II'] = $rowcnt+1;
			$novel_total_favo['III'] = $rowcnt%7;
			$novel_total_favo['F_uid'] = $GLOBALS['_assign']['F_uid'];
			$novel_total_favo['F_username'] = $GLOBALS['_assign']['F_username'];
			$novel_total_favo['G_sitename'] = $GLOBALS['_assign']['G_sitename'];
			$novel_total_favo['G_siteurl'] = $GLOBALS['_assign']['G_siteurl'];
			$novel_total_favo['G_style'] = $GLOBALS['_assign']['G_style'];
			$novel_total_favo['G_tpldir'] = $GLOBALS['_assign']['G_tpldir'];
			$novel_total_favo['G_PHPSELF'] = $GLOBALS['_assign']['G_PHPSELF'];
			$novel_total_favo['G_encode'] = $GLOBALS['_assign']['G_encode'];
			$novel_total_favo['TIMESTAMP'] = $GLOBALS['_assign']['TIMESTAMP'];
			$novel_total_favo['nc'] = $GLOBALS['_assign']['nc'];
			$novel_total_favo['A'] = $rowcnt % 2;
			$novel_total_favo['B'] = $rowcnt % 2;
			$_obj = &$novel_total_favo;
?>
						<dd><a href="<?php
echo $_obj['InfoURL'];
?>
"><?php
echo $_obj['title'];
?>
</a>(<?php
echo $_obj['favo_total'];
?>
)</dd>
<?php
}
$_obj=$_stack[--$_stack_cnt];}
?>
					</dl>
					<span><a href="rank.php?times=total&amp;types=favo&amp;sc=<?php
echo $_obj['nc'];
?>
">更多&gt;&gt;</a></span>
					</div>	
				</div>
		</div>
