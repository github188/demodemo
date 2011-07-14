<?php
if(!defined('IN_Read8')) {
	exit('Access Denied (template_basic_index.php)');
}
?>
	<div id="rd8_center">
<!--广告位开始 -->
		<div class="rd8_ad_position">
			<div style="float:left;overflow:hidden;"><?php
echo $_obj['CAD_index_1'];
?>
</div>
			<div style="float:right;overflow:hidden;"><?php
echo $_obj['CAD_index_2'];
?>
</div>

		</div>
<!--广告位结束 -->
		<!--封面图片和FLASH -->
		<div class="rd8_images">
		
			<div class="rd8_imagesleft">
				<div class="rd8_book">
<?php
	if(!empty($_obj['custom_fmtj'])) {
		if(!is_array($_obj['custom_fmtj']))
			$_obj['custom_fmtj'] = array(array('custom_fmtj' => $_obj['custom_fmtj']));
		$_tmp_arr_keys = array_keys($_obj['custom_fmtj']);
		if($_tmp_arr_keys[0] != '0')
			$_obj['custom_fmtj'] = array(0 => $_obj['custom_fmtj']);
		$_stack[$_stack_cnt++] = $_obj;
		foreach($_obj['custom_fmtj'] as $rowcnt=>$custom_fmtj) {
			$custom_fmtj['I'] = $rowcnt;
			$custom_fmtj['II'] = $rowcnt+1;
			$custom_fmtj['III'] = $rowcnt%7;
			$custom_fmtj['F_uid'] = $GLOBALS['_assign']['F_uid'];
			$custom_fmtj['F_username'] = $GLOBALS['_assign']['F_username'];
			$custom_fmtj['G_sitename'] = $GLOBALS['_assign']['G_sitename'];
			$custom_fmtj['G_siteurl'] = $GLOBALS['_assign']['G_siteurl'];
			$custom_fmtj['G_style'] = $GLOBALS['_assign']['G_style'];
			$custom_fmtj['G_tpldir'] = $GLOBALS['_assign']['G_tpldir'];
			$custom_fmtj['G_PHPSELF'] = $GLOBALS['_assign']['G_PHPSELF'];
			$custom_fmtj['G_encode'] = $GLOBALS['_assign']['G_encode'];
			$custom_fmtj['TIMESTAMP'] = $GLOBALS['_assign']['TIMESTAMP'];
			$custom_fmtj['A'] = $rowcnt % 2;
			$custom_fmtj['B'] = $rowcnt % 2;
			$_obj = &$custom_fmtj;
?>
					<div class="rd8_booklinkbox">
						<a href="<?php
echo $_obj['InfoURL'];
?>
"><img src="<?php
echo $_obj['HTMLURL'];
?>
/bookface.jpg" alt="<?php
echo $_obj['title'];
?>
" height="130" style="border:none;" width="100"/></a>
					</div>
<?php
}
$_obj=$_stack[--$_stack_cnt];}
?>
				</div>
			</div>
			<div class="rd8_imagesright">
				<a target="_self" href="javascript:goUrl()"> 
					<script type="text/javascript">/*<![CDATA[*/
						var focus_width=303;
						var focus_height=164;
						var text_height=18;
						var swf_height = focus_height+text_height;

						var pics='<?php
echo $_obj['pics'];
?>
';
						var links='<?php
echo $_obj['links'];
?>
';
						var texts='<?php
echo $_obj['texts'];
?>
';
							 
						document.write('<object classid="clsid:d27cdb6e-ae6d-11cf-96b8-444553540000" codebase="http://fpdownload.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=6,0,0,0" width="'+ focus_width +'" height="'+ swf_height +'">');
						document.write('<param name="allowScriptAccess" value="sameDomain"><param name="movie" value="include/flash/focus.swf"><param name="quality" value="high"><param name="bgcolor" value="#CCCCCC">');
						document.write('<param name="menu" value="false"><param name=wmode value="opaque">');
						document.write('<param name="FlashVars" value="pics='+pics+'&links='+links+'&texts='+texts+'&borderwidth='+focus_width+'&borderheight='+focus_height+'&textheight='+text_height+'">');
						document.write('<embed src="include/flash/focus.swf" wmode="opaque" FlashVars="pics='+pics+'&links='+links+'&texts='+texts+'&borderwidth='+focus_width+'&borderheight='+focus_height+'&textheight='+text_height+'" menu="false" bgcolor="#F0F0F0" quality="high" width="'+ focus_width +'" height="'+ focus_height +'" allowScriptAccess="sameDomain" type="application/x-shockwave-flash" pluginspage="http://www.macromedia.com/go/getflashplayer" />');  document.write('</object>');
					/*]]>*/</script>
				</a>
			</div>
		

		</div>
		<!--推荐栏开始 -->
		<div class="rd8_recommend">
			<div style="float:left;margin-bottom:6px;*margin-bottom:0;">
				<div class="rd8_recommendlist">
				
					<dl>
						<dt>强力推荐榜</dt>
<?php
	if(!empty($_obj['custom_qltjb'])) {
		if(!is_array($_obj['custom_qltjb']))
			$_obj['custom_qltjb'] = array(array('custom_qltjb' => $_obj['custom_qltjb']));
		$_tmp_arr_keys = array_keys($_obj['custom_qltjb']);
		if($_tmp_arr_keys[0] != '0')
			$_obj['custom_qltjb'] = array(0 => $_obj['custom_qltjb']);
		$_stack[$_stack_cnt++] = $_obj;
		foreach($_obj['custom_qltjb'] as $rowcnt=>$custom_qltjb) {
			$custom_qltjb['I'] = $rowcnt;
			$custom_qltjb['II'] = $rowcnt+1;
			$custom_qltjb['III'] = $rowcnt%7;
			$custom_qltjb['F_uid'] = $GLOBALS['_assign']['F_uid'];
			$custom_qltjb['F_username'] = $GLOBALS['_assign']['F_username'];
			$custom_qltjb['G_sitename'] = $GLOBALS['_assign']['G_sitename'];
			$custom_qltjb['G_siteurl'] = $GLOBALS['_assign']['G_siteurl'];
			$custom_qltjb['G_style'] = $GLOBALS['_assign']['G_style'];
			$custom_qltjb['G_tpldir'] = $GLOBALS['_assign']['G_tpldir'];
			$custom_qltjb['G_PHPSELF'] = $GLOBALS['_assign']['G_PHPSELF'];
			$custom_qltjb['G_encode'] = $GLOBALS['_assign']['G_encode'];
			$custom_qltjb['TIMESTAMP'] = $GLOBALS['_assign']['TIMESTAMP'];
			$custom_qltjb['A'] = $rowcnt % 2;
			$custom_qltjb['B'] = $rowcnt % 2;
			$_obj = &$custom_qltjb;
?>
						<dd>
							<strong class="rd8_bookname"><a href="<?php
echo $_obj['InfoURL'];
?>
">
							<img src="<?php
echo $_obj['HTMLURL'];
?>
/bookface.jpg" alt="<?php
echo $_obj['title'];
?>
" align="left" width="100" height="125"/><?php
echo $_obj['title'];
?>
<br />
							</a></strong>
						</dd>
<?php
}
$_obj=$_stack[--$_stack_cnt];}
?>
					</dl>
				</div>

 				<div class="rd8_talk">
					<dl>
						<dt>作家访谈</dt>

						<dd>
							<?php
echo $_obj['interview']['content'];
?>

						</dd>

					</dl>
					<span><a href="interview.php?id=<?php
if(!empty($_obj['interview']['id'])) {
?><?php
echo $_obj['interview']['id'];
?>
<?php
}
?>">[查看全部]</a></span>
			
				</div>
			</div>


			<div class="rd8_recommendmiddle">
				<div id="rd8_recommendcontent">
					<ul id="rd8_rectabs">
<?php
	if(!empty($_obj['custom_sflbt'])) {
		if(!is_array($_obj['custom_sflbt']))
			$_obj['custom_sflbt'] = array(array('custom_sflbt' => $_obj['custom_sflbt']));
		$_tmp_arr_keys = array_keys($_obj['custom_sflbt']);
		if($_tmp_arr_keys[0] != '0')
			$_obj['custom_sflbt'] = array(0 => $_obj['custom_sflbt']);
		$_stack[$_stack_cnt++] = $_obj;
		foreach($_obj['custom_sflbt'] as $rowcnt=>$custom_sflbt) {
			$custom_sflbt['I'] = $rowcnt;
			$custom_sflbt['II'] = $rowcnt+1;
			$custom_sflbt['III'] = $rowcnt%7;
			$custom_sflbt['F_uid'] = $GLOBALS['_assign']['F_uid'];
			$custom_sflbt['F_username'] = $GLOBALS['_assign']['F_username'];
			$custom_sflbt['G_sitename'] = $GLOBALS['_assign']['G_sitename'];
			$custom_sflbt['G_siteurl'] = $GLOBALS['_assign']['G_siteurl'];
			$custom_sflbt['G_style'] = $GLOBALS['_assign']['G_style'];
			$custom_sflbt['G_tpldir'] = $GLOBALS['_assign']['G_tpldir'];
			$custom_sflbt['G_PHPSELF'] = $GLOBALS['_assign']['G_PHPSELF'];
			$custom_sflbt['G_encode'] = $GLOBALS['_assign']['G_encode'];
			$custom_sflbt['TIMESTAMP'] = $GLOBALS['_assign']['TIMESTAMP'];
			$custom_sflbt['A'] = $rowcnt % 2;
			$custom_sflbt['B'] = $rowcnt % 2;
			$_obj = &$custom_sflbt;
?>
						<li id="rd8_rectab<?php
echo $_obj['subject_id'];
?>
" onmouseover="recsetTab('rd8_recommendcontent', 'rd8_reccontent<?php
echo $_obj['subject_id'];
?>
', 'rd8_rectab<?php
echo $_obj['subject_id'];
?>
');"<?php
if($_obj['II'] == "1") {
?> class="rd8_selectedtab" style="margin: 0 0 0 -32px;*margin:0;"<?php
}
?>><?php
echo $_obj['title'];
?>
</li>
<?php
}
$_obj=$_stack[--$_stack_cnt];}
?>
					</ul>
<?php
	if(!empty($_obj['custom_mgl'])) {
		if(!is_array($_obj['custom_mgl']))
			$_obj['custom_mgl'] = array(array('custom_mgl' => $_obj['custom_mgl']));
		$_tmp_arr_keys = array_keys($_obj['custom_mgl']);
		if($_tmp_arr_keys[0] != '0')
			$_obj['custom_mgl'] = array(0 => $_obj['custom_mgl']);
		$_stack[$_stack_cnt++] = $_obj;
		foreach($_obj['custom_mgl'] as $rowcnt=>$custom_mgl) {
			$custom_mgl['I'] = $rowcnt;
			$custom_mgl['II'] = $rowcnt+1;
			$custom_mgl['III'] = $rowcnt%7;
			$custom_mgl['F_uid'] = $GLOBALS['_assign']['F_uid'];
			$custom_mgl['F_username'] = $GLOBALS['_assign']['F_username'];
			$custom_mgl['G_sitename'] = $GLOBALS['_assign']['G_sitename'];
			$custom_mgl['G_siteurl'] = $GLOBALS['_assign']['G_siteurl'];
			$custom_mgl['G_style'] = $GLOBALS['_assign']['G_style'];
			$custom_mgl['G_tpldir'] = $GLOBALS['_assign']['G_tpldir'];
			$custom_mgl['G_PHPSELF'] = $GLOBALS['_assign']['G_PHPSELF'];
			$custom_mgl['G_encode'] = $GLOBALS['_assign']['G_encode'];
			$custom_mgl['TIMESTAMP'] = $GLOBALS['_assign']['TIMESTAMP'];
			$custom_mgl['A'] = $rowcnt % 2;
			$custom_mgl['B'] = $rowcnt % 2;
			$_obj = &$custom_mgl;
?>
					<div class="rd8_rectabcon" id="rd8_reccontent<?php
echo $_obj['subject_id'];
?>
"<?php
if($_obj['II'] > "1") {
?> style="display: none"<?php
}
?>>
<?php
	if(!empty($_obj['custom__data'])) {
		if(!is_array($_obj['custom__data']))
			$_obj['custom__data'] = array(array('custom__data' => $_obj['custom__data']));
		$_tmp_arr_keys = array_keys($_obj['custom__data']);
		if($_tmp_arr_keys[0] != '0')
			$_obj['custom__data'] = array(0 => $_obj['custom__data']);
		$_stack[$_stack_cnt++] = $_obj;
		foreach($_obj['custom__data'] as $rowcnt=>$custom__data) {
			$custom__data['I'] = $rowcnt;
			$custom__data['II'] = $rowcnt+1;
			$custom__data['III'] = $rowcnt%7;
			$custom__data['F_uid'] = $GLOBALS['_assign']['F_uid'];
			$custom__data['F_username'] = $GLOBALS['_assign']['F_username'];
			$custom__data['G_sitename'] = $GLOBALS['_assign']['G_sitename'];
			$custom__data['G_siteurl'] = $GLOBALS['_assign']['G_siteurl'];
			$custom__data['G_style'] = $GLOBALS['_assign']['G_style'];
			$custom__data['G_tpldir'] = $GLOBALS['_assign']['G_tpldir'];
			$custom__data['G_PHPSELF'] = $GLOBALS['_assign']['G_PHPSELF'];
			$custom__data['G_encode'] = $GLOBALS['_assign']['G_encode'];
			$custom__data['TIMESTAMP'] = $GLOBALS['_assign']['TIMESTAMP'];
			$custom__data['A'] = $rowcnt % 2;
			$custom__data['B'] = $rowcnt % 2;
			$_obj = &$custom__data;
?>
						<p class="rd8_imgtxttit"><strong class="rd8_bookname"><a href="<?php
echo $_obj['InfoURL'];
?>
"><?php
echo $_obj['title'];
?>
</a></strong><span style="padding-right:6px;">作者：<?php
echo $_obj['author'];
?>
</span></p>
						<p class="rd8_imgtxt"><a href="<?php
echo $_obj['InfoURL'];
?>
"><img src="<?php
echo $_obj['HTMLURL'];
?>
/bookface.jpg" alt="<?php
echo $_obj['title'];
?>
" align="left" width="144" height="179" /></a><span><?php
echo $_obj['intro'];
?>
</span></p>
<?php
}
$_obj=$_stack[--$_stack_cnt];}
?>
					</div>
<?php
}
$_obj=$_stack[--$_stack_cnt];}
?>
				</div>

				<div class="rd8_notice">
					<div class="rd8_g">最新公告</div>
					<ul>
<?php
	if(!empty($_obj['Announceblock'])) {
		if(!is_array($_obj['Announceblock']))
			$_obj['Announceblock'] = array(array('Announceblock' => $_obj['Announceblock']));
		$_tmp_arr_keys = array_keys($_obj['Announceblock']);
		if($_tmp_arr_keys[0] != '0')
			$_obj['Announceblock'] = array(0 => $_obj['Announceblock']);
		$_stack[$_stack_cnt++] = $_obj;
		foreach($_obj['Announceblock'] as $rowcnt=>$Announceblock) {
			$Announceblock['I'] = $rowcnt;
			$Announceblock['II'] = $rowcnt+1;
			$Announceblock['III'] = $rowcnt%7;
			$Announceblock['F_uid'] = $GLOBALS['_assign']['F_uid'];
			$Announceblock['F_username'] = $GLOBALS['_assign']['F_username'];
			$Announceblock['G_sitename'] = $GLOBALS['_assign']['G_sitename'];
			$Announceblock['G_siteurl'] = $GLOBALS['_assign']['G_siteurl'];
			$Announceblock['G_style'] = $GLOBALS['_assign']['G_style'];
			$Announceblock['G_tpldir'] = $GLOBALS['_assign']['G_tpldir'];
			$Announceblock['G_PHPSELF'] = $GLOBALS['_assign']['G_PHPSELF'];
			$Announceblock['G_encode'] = $GLOBALS['_assign']['G_encode'];
			$Announceblock['TIMESTAMP'] = $GLOBALS['_assign']['TIMESTAMP'];
			$Announceblock['A'] = $rowcnt % 2;
			$Announceblock['B'] = $rowcnt % 2;
			$_obj = &$Announceblock;
?>
						<li><a href="announcement.php?id=<?php
echo $_obj['id'];
?>
"><?php
echo $_obj['title'];
?>
</a><span>（<?php
echo rdate($_obj['dateline'],'y-m-d');
?>
）</span></li>
<?php
}
$_obj=$_stack[--$_stack_cnt];}
?>
					</ul>
					<span style="float:right;padding-top:4px;*padding:0;padding-right:2px;"><a href="announcement.php">更多&gt;&gt;</a></span>
				 </div>
			</div>
			<div style="float:left;margin-left:8px !important;margin-left:6px;">
				<div class="rd8_recommendlist">
					<dl>
						<dt>编辑推荐栏</dt>
<?php
	if(!empty($_obj['novel_3'])) {
		if(!is_array($_obj['novel_3']))
			$_obj['novel_3'] = array(array('novel_3' => $_obj['novel_3']));
		$_tmp_arr_keys = array_keys($_obj['novel_3']);
		if($_tmp_arr_keys[0] != '0')
			$_obj['novel_3'] = array(0 => $_obj['novel_3']);
		$_stack[$_stack_cnt++] = $_obj;
		foreach($_obj['novel_3'] as $rowcnt=>$novel_3) {
			$novel_3['I'] = $rowcnt;
			$novel_3['II'] = $rowcnt+1;
			$novel_3['III'] = $rowcnt%7;
			$novel_3['F_uid'] = $GLOBALS['_assign']['F_uid'];
			$novel_3['F_username'] = $GLOBALS['_assign']['F_username'];
			$novel_3['G_sitename'] = $GLOBALS['_assign']['G_sitename'];
			$novel_3['G_siteurl'] = $GLOBALS['_assign']['G_siteurl'];
			$novel_3['G_style'] = $GLOBALS['_assign']['G_style'];
			$novel_3['G_tpldir'] = $GLOBALS['_assign']['G_tpldir'];
			$novel_3['G_PHPSELF'] = $GLOBALS['_assign']['G_PHPSELF'];
			$novel_3['G_encode'] = $GLOBALS['_assign']['G_encode'];
			$novel_3['TIMESTAMP'] = $GLOBALS['_assign']['TIMESTAMP'];
			$novel_3['A'] = $rowcnt % 2;
			$novel_3['B'] = $rowcnt % 2;
			$_obj = &$novel_3;
?>
						<dd>
							<strong class="rd8_bookname"><a href="<?php
echo $_obj['InfoURL'];
?>
">
							<img src="<?php
echo $_obj['HTMLURL'];
?>
/bookface.jpg" alt="<?php
echo $_obj['title'];
?>
" align="left" width="100" height="125"/><?php
echo $_obj['title'];
?>
<br />
							</a></strong>
						</dd>
<?php
}
$_obj=$_stack[--$_stack_cnt];}
?>
					</dl>
				</div>
				<div class="rd8_recommendlist" style="height:100px;margin-bottom:8px;">
					<dl>
						<dt>搜索</dt>
						<dd><div class="rd8_mysearch">
							<form method="get" action="search.php" onsubmit="return this.value && this.value != '在此输入关键字'">
								<select name="by">
									<option value="title"selected="selected">按书名</option>
									<option value="author">按作者</option>
								</select>
								<input type="text" name="kw" style="width:100px;height:20px;border:1px solid #698CC3;" value="在此输入关键字" onfocus="if(this.value == '在此输入关键字') this.value = '';" onblur="if(this.value == '') this.value = '在此输入关键字'" />
								<input type="submit" value="搜索" style="width:50px;height:20px;margin-left:80px;border:1px solid #698CC3;" />
							</form>
						</div>
						</dd>
					</dl>
				</div>
				<div class="rd8_recommendlist" style="height:259px;">
					<dl>
						<dt>就在刚才</dt>
<?php
	if(!empty($_obj['justnow'])) {
		if(!is_array($_obj['justnow']))
			$_obj['justnow'] = array(array('justnow' => $_obj['justnow']));
		$_tmp_arr_keys = array_keys($_obj['justnow']);
		if($_tmp_arr_keys[0] != '0')
			$_obj['justnow'] = array(0 => $_obj['justnow']);
		$_stack[$_stack_cnt++] = $_obj;
		foreach($_obj['justnow'] as $rowcnt=>$justnow) {
			$justnow['I'] = $rowcnt;
			$justnow['II'] = $rowcnt+1;
			$justnow['III'] = $rowcnt%7;
			$justnow['F_uid'] = $GLOBALS['_assign']['F_uid'];
			$justnow['F_username'] = $GLOBALS['_assign']['F_username'];
			$justnow['G_sitename'] = $GLOBALS['_assign']['G_sitename'];
			$justnow['G_siteurl'] = $GLOBALS['_assign']['G_siteurl'];
			$justnow['G_style'] = $GLOBALS['_assign']['G_style'];
			$justnow['G_tpldir'] = $GLOBALS['_assign']['G_tpldir'];
			$justnow['G_PHPSELF'] = $GLOBALS['_assign']['G_PHPSELF'];
			$justnow['G_encode'] = $GLOBALS['_assign']['G_encode'];
			$justnow['TIMESTAMP'] = $GLOBALS['_assign']['TIMESTAMP'];
			$justnow['A'] = $rowcnt % 2;
			$justnow['B'] = $rowcnt % 2;
			$_obj = &$justnow;
?>
						<dd>&nbsp;<?php
echo $_obj['text'];
?>
</dd>
<?php
}
$_obj=$_stack[--$_stack_cnt];}
?>
					</dl>
				</div>
				</div>
		</div>
		<!--推荐栏结束 -->

		<!--点击榜开始 -->
		<div class="rd8_events">
			<div class="rd8_Canada">
				<dl>
					<dt>总点击榜</dt>
<?php
	if(!empty($_obj['custom_zdjb'])) {
		if(!is_array($_obj['custom_zdjb']))
			$_obj['custom_zdjb'] = array(array('custom_zdjb' => $_obj['custom_zdjb']));
		$_tmp_arr_keys = array_keys($_obj['custom_zdjb']);
		if($_tmp_arr_keys[0] != '0')
			$_obj['custom_zdjb'] = array(0 => $_obj['custom_zdjb']);
		$_stack[$_stack_cnt++] = $_obj;
		foreach($_obj['custom_zdjb'] as $rowcnt=>$custom_zdjb) {
			$custom_zdjb['I'] = $rowcnt;
			$custom_zdjb['II'] = $rowcnt+1;
			$custom_zdjb['III'] = $rowcnt%7;
			$custom_zdjb['F_uid'] = $GLOBALS['_assign']['F_uid'];
			$custom_zdjb['F_username'] = $GLOBALS['_assign']['F_username'];
			$custom_zdjb['G_sitename'] = $GLOBALS['_assign']['G_sitename'];
			$custom_zdjb['G_siteurl'] = $GLOBALS['_assign']['G_siteurl'];
			$custom_zdjb['G_style'] = $GLOBALS['_assign']['G_style'];
			$custom_zdjb['G_tpldir'] = $GLOBALS['_assign']['G_tpldir'];
			$custom_zdjb['G_PHPSELF'] = $GLOBALS['_assign']['G_PHPSELF'];
			$custom_zdjb['G_encode'] = $GLOBALS['_assign']['G_encode'];
			$custom_zdjb['TIMESTAMP'] = $GLOBALS['_assign']['TIMESTAMP'];
			$custom_zdjb['A'] = $rowcnt % 2;
			$custom_zdjb['B'] = $rowcnt % 2;
			$_obj = &$custom_zdjb;
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
				<span><a href="rank.php?times=total&amp;types=click">更多&gt;&gt;</a></span>
			</div>
<?php
	if(!empty($_obj['custom_trwyg'])) {
		if(!is_array($_obj['custom_trwyg']))
			$_obj['custom_trwyg'] = array(array('custom_trwyg' => $_obj['custom_trwyg']));
		$_tmp_arr_keys = array_keys($_obj['custom_trwyg']);
		if($_tmp_arr_keys[0] != '0')
			$_obj['custom_trwyg'] = array(0 => $_obj['custom_trwyg']);
		$_stack[$_stack_cnt++] = $_obj;
		foreach($_obj['custom_trwyg'] as $rowcnt=>$custom_trwyg) {
			$custom_trwyg['I'] = $rowcnt;
			$custom_trwyg['II'] = $rowcnt+1;
			$custom_trwyg['III'] = $rowcnt%7;
			$custom_trwyg['F_uid'] = $GLOBALS['_assign']['F_uid'];
			$custom_trwyg['F_username'] = $GLOBALS['_assign']['F_username'];
			$custom_trwyg['G_sitename'] = $GLOBALS['_assign']['G_sitename'];
			$custom_trwyg['G_siteurl'] = $GLOBALS['_assign']['G_siteurl'];
			$custom_trwyg['G_style'] = $GLOBALS['_assign']['G_style'];
			$custom_trwyg['G_tpldir'] = $GLOBALS['_assign']['G_tpldir'];
			$custom_trwyg['G_PHPSELF'] = $GLOBALS['_assign']['G_PHPSELF'];
			$custom_trwyg['G_encode'] = $GLOBALS['_assign']['G_encode'];
			$custom_trwyg['TIMESTAMP'] = $GLOBALS['_assign']['TIMESTAMP'];
			$custom_trwyg['A'] = $rowcnt % 2;
			$custom_trwyg['B'] = $rowcnt % 2;
			$_obj = &$custom_trwyg;
?>
			<div class="rd8_fiction">
				<p class="rd8_title"><?php
echo $_obj['subject'];
?>
阁</p>
	<?php
	if(!empty($_obj['custom__data'])) {
		if(!is_array($_obj['custom__data']))
			$_obj['custom__data'] = array(array('custom__data' => $_obj['custom__data']));
		$_tmp_arr_keys = array_keys($_obj['custom__data']);
		if($_tmp_arr_keys[0] != '0')
			$_obj['custom__data'] = array(0 => $_obj['custom__data']);
		$_stack[$_stack_cnt++] = $_obj;
		foreach($_obj['custom__data'] as $rowcnt=>$custom__data) {
			$custom__data['I'] = $rowcnt;
			$custom__data['II'] = $rowcnt+1;
			$custom__data['III'] = $rowcnt%7;
			$custom__data['F_uid'] = $GLOBALS['_assign']['F_uid'];
			$custom__data['F_username'] = $GLOBALS['_assign']['F_username'];
			$custom__data['G_sitename'] = $GLOBALS['_assign']['G_sitename'];
			$custom__data['G_siteurl'] = $GLOBALS['_assign']['G_siteurl'];
			$custom__data['G_style'] = $GLOBALS['_assign']['G_style'];
			$custom__data['G_tpldir'] = $GLOBALS['_assign']['G_tpldir'];
			$custom__data['G_PHPSELF'] = $GLOBALS['_assign']['G_PHPSELF'];
			$custom__data['G_encode'] = $GLOBALS['_assign']['G_encode'];
			$custom__data['TIMESTAMP'] = $GLOBALS['_assign']['TIMESTAMP'];
			$custom__data['A'] = $rowcnt % 2;
			$custom__data['B'] = $rowcnt % 2;
			$_obj = &$custom__data;
?>
				<p class="rd8_ficimgtxt">
					<a href="<?php
echo $_obj['InfoURL'];
?>
"><img src="<?php
echo $_obj['HTMLURL'];
?>
/bookface.jpg" alt="<?php
echo $_obj['title'];
?>
" width="80" height="100" align="left" /></a><?php
echo $_obj['intro'];
?>

				</p>
	<?php
}
$_obj=$_stack[--$_stack_cnt];}
?>
			</div>
<?php
}
$_obj=$_stack[--$_stack_cnt];}
?>
		</div>
		<!--点击榜结束 -->

		<!--投票榜开始 -->
		<div class="rd8_events">
			<div class="rd8_Canada">
				<dl>
					<dt>总推荐榜</dt>
<?php
	if(!empty($_obj['custom_ztjb'])) {
		if(!is_array($_obj['custom_ztjb']))
			$_obj['custom_ztjb'] = array(array('custom_ztjb' => $_obj['custom_ztjb']));
		$_tmp_arr_keys = array_keys($_obj['custom_ztjb']);
		if($_tmp_arr_keys[0] != '0')
			$_obj['custom_ztjb'] = array(0 => $_obj['custom_ztjb']);
		$_stack[$_stack_cnt++] = $_obj;
		foreach($_obj['custom_ztjb'] as $rowcnt=>$custom_ztjb) {
			$custom_ztjb['I'] = $rowcnt;
			$custom_ztjb['II'] = $rowcnt+1;
			$custom_ztjb['III'] = $rowcnt%7;
			$custom_ztjb['F_uid'] = $GLOBALS['_assign']['F_uid'];
			$custom_ztjb['F_username'] = $GLOBALS['_assign']['F_username'];
			$custom_ztjb['G_sitename'] = $GLOBALS['_assign']['G_sitename'];
			$custom_ztjb['G_siteurl'] = $GLOBALS['_assign']['G_siteurl'];
			$custom_ztjb['G_style'] = $GLOBALS['_assign']['G_style'];
			$custom_ztjb['G_tpldir'] = $GLOBALS['_assign']['G_tpldir'];
			$custom_ztjb['G_PHPSELF'] = $GLOBALS['_assign']['G_PHPSELF'];
			$custom_ztjb['G_encode'] = $GLOBALS['_assign']['G_encode'];
			$custom_ztjb['TIMESTAMP'] = $GLOBALS['_assign']['TIMESTAMP'];
			$custom_ztjb['A'] = $rowcnt % 2;
			$custom_ztjb['B'] = $rowcnt % 2;
			$_obj = &$custom_ztjb;
?>
					<dd><a href="<?php
echo $_obj['InfoURL'];
?>
"><?php
echo $_obj['title'];
?>
</a>(<?php
echo $_obj['commend_total'];
?>
)</dd>
<?php
}
$_obj=$_stack[--$_stack_cnt];}
?>
				</dl>	
				<span><a href="rank.php?times=total&amp;types=commend">更多&gt;&gt;</a></span>
			</div>

<?php
	if(!empty($_obj['custom_tydmg'])) {
		if(!is_array($_obj['custom_tydmg']))
			$_obj['custom_tydmg'] = array(array('custom_tydmg' => $_obj['custom_tydmg']));
		$_tmp_arr_keys = array_keys($_obj['custom_tydmg']);
		if($_tmp_arr_keys[0] != '0')
			$_obj['custom_tydmg'] = array(0 => $_obj['custom_tydmg']);
		$_stack[$_stack_cnt++] = $_obj;
		foreach($_obj['custom_tydmg'] as $rowcnt=>$custom_tydmg) {
			$custom_tydmg['I'] = $rowcnt;
			$custom_tydmg['II'] = $rowcnt+1;
			$custom_tydmg['III'] = $rowcnt%7;
			$custom_tydmg['F_uid'] = $GLOBALS['_assign']['F_uid'];
			$custom_tydmg['F_username'] = $GLOBALS['_assign']['F_username'];
			$custom_tydmg['G_sitename'] = $GLOBALS['_assign']['G_sitename'];
			$custom_tydmg['G_siteurl'] = $GLOBALS['_assign']['G_siteurl'];
			$custom_tydmg['G_style'] = $GLOBALS['_assign']['G_style'];
			$custom_tydmg['G_tpldir'] = $GLOBALS['_assign']['G_tpldir'];
			$custom_tydmg['G_PHPSELF'] = $GLOBALS['_assign']['G_PHPSELF'];
			$custom_tydmg['G_encode'] = $GLOBALS['_assign']['G_encode'];
			$custom_tydmg['TIMESTAMP'] = $GLOBALS['_assign']['TIMESTAMP'];
			$custom_tydmg['A'] = $rowcnt % 2;
			$custom_tydmg['B'] = $rowcnt % 2;
			$_obj = &$custom_tydmg;
?>
			<div class="rd8_fiction">
				<p class="rd8_title"><?php
echo $_obj['subject'];
?>
阁</p>
	<?php
	if(!empty($_obj['custom__data'])) {
		if(!is_array($_obj['custom__data']))
			$_obj['custom__data'] = array(array('custom__data' => $_obj['custom__data']));
		$_tmp_arr_keys = array_keys($_obj['custom__data']);
		if($_tmp_arr_keys[0] != '0')
			$_obj['custom__data'] = array(0 => $_obj['custom__data']);
		$_stack[$_stack_cnt++] = $_obj;
		foreach($_obj['custom__data'] as $rowcnt=>$custom__data) {
			$custom__data['I'] = $rowcnt;
			$custom__data['II'] = $rowcnt+1;
			$custom__data['III'] = $rowcnt%7;
			$custom__data['F_uid'] = $GLOBALS['_assign']['F_uid'];
			$custom__data['F_username'] = $GLOBALS['_assign']['F_username'];
			$custom__data['G_sitename'] = $GLOBALS['_assign']['G_sitename'];
			$custom__data['G_siteurl'] = $GLOBALS['_assign']['G_siteurl'];
			$custom__data['G_style'] = $GLOBALS['_assign']['G_style'];
			$custom__data['G_tpldir'] = $GLOBALS['_assign']['G_tpldir'];
			$custom__data['G_PHPSELF'] = $GLOBALS['_assign']['G_PHPSELF'];
			$custom__data['G_encode'] = $GLOBALS['_assign']['G_encode'];
			$custom__data['TIMESTAMP'] = $GLOBALS['_assign']['TIMESTAMP'];
			$custom__data['A'] = $rowcnt % 2;
			$custom__data['B'] = $rowcnt % 2;
			$_obj = &$custom__data;
?>
				<p class="rd8_ficimgtxt">
					<a href="<?php
echo $_obj['InfoURL'];
?>
"><img src="<?php
echo $_obj['HTMLURL'];
?>
/bookface.jpg" alt="<?php
echo $_obj['title'];
?>
" width="80" height="100" align="left" /></a><?php
echo $_obj['intro'];
?>

				</p>
	<?php
}
$_obj=$_stack[--$_stack_cnt];}
?>
			</div>
<?php
}
$_obj=$_stack[--$_stack_cnt];}
?>
		</div>
		<!--投票榜结束 -->

		<div class="rd8_news">
			<div class="rd8_newsleft">
				<div class="rd8_Canada">
					<dl>
						<dt>最新入库作品</dt>
<?php
	if(!empty($_obj['nc11'])) {
		if(!is_array($_obj['nc11']))
			$_obj['nc11'] = array(array('nc11' => $_obj['nc11']));
		$_tmp_arr_keys = array_keys($_obj['nc11']);
		if($_tmp_arr_keys[0] != '0')
			$_obj['nc11'] = array(0 => $_obj['nc11']);
		$_stack[$_stack_cnt++] = $_obj;
		foreach($_obj['nc11'] as $rowcnt=>$nc11) {
			$nc11['I'] = $rowcnt;
			$nc11['II'] = $rowcnt+1;
			$nc11['III'] = $rowcnt%7;
			$nc11['F_uid'] = $GLOBALS['_assign']['F_uid'];
			$nc11['F_username'] = $GLOBALS['_assign']['F_username'];
			$nc11['G_sitename'] = $GLOBALS['_assign']['G_sitename'];
			$nc11['G_siteurl'] = $GLOBALS['_assign']['G_siteurl'];
			$nc11['G_style'] = $GLOBALS['_assign']['G_style'];
			$nc11['G_tpldir'] = $GLOBALS['_assign']['G_tpldir'];
			$nc11['G_PHPSELF'] = $GLOBALS['_assign']['G_PHPSELF'];
			$nc11['G_encode'] = $GLOBALS['_assign']['G_encode'];
			$nc11['TIMESTAMP'] = $GLOBALS['_assign']['TIMESTAMP'];
			$nc11['A'] = $rowcnt % 2;
			$nc11['B'] = $rowcnt % 2;
			$_obj = &$nc11;
?>
						<dd><a href="<?php
echo $_obj['InfoURL'];
?>
"><?php
echo $_obj['title'];
?>
</a>(<?php
echo rdate($_obj['dateline'],'m-d');
?>
)</dd>
<?php
}
$_obj=$_stack[--$_stack_cnt];}
?>
					</dl>
					<span><a href="novellib.php?oderbycreate=1">更多&gt;&gt;</a></span>
				</div>

				<div class="rd8_Canada" style="margin-bottom:0;">
					<dl>
						<dt>总收藏榜</dt>
<?php
	if(!empty($_obj['custom_zscb'])) {
		if(!is_array($_obj['custom_zscb']))
			$_obj['custom_zscb'] = array(array('custom_zscb' => $_obj['custom_zscb']));
		$_tmp_arr_keys = array_keys($_obj['custom_zscb']);
		if($_tmp_arr_keys[0] != '0')
			$_obj['custom_zscb'] = array(0 => $_obj['custom_zscb']);
		$_stack[$_stack_cnt++] = $_obj;
		foreach($_obj['custom_zscb'] as $rowcnt=>$custom_zscb) {
			$custom_zscb['I'] = $rowcnt;
			$custom_zscb['II'] = $rowcnt+1;
			$custom_zscb['III'] = $rowcnt%7;
			$custom_zscb['F_uid'] = $GLOBALS['_assign']['F_uid'];
			$custom_zscb['F_username'] = $GLOBALS['_assign']['F_username'];
			$custom_zscb['G_sitename'] = $GLOBALS['_assign']['G_sitename'];
			$custom_zscb['G_siteurl'] = $GLOBALS['_assign']['G_siteurl'];
			$custom_zscb['G_style'] = $GLOBALS['_assign']['G_style'];
			$custom_zscb['G_tpldir'] = $GLOBALS['_assign']['G_tpldir'];
			$custom_zscb['G_PHPSELF'] = $GLOBALS['_assign']['G_PHPSELF'];
			$custom_zscb['G_encode'] = $GLOBALS['_assign']['G_encode'];
			$custom_zscb['TIMESTAMP'] = $GLOBALS['_assign']['TIMESTAMP'];
			$custom_zscb['A'] = $rowcnt % 2;
			$custom_zscb['B'] = $rowcnt % 2;
			$_obj = &$custom_zscb;
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
					<span><a href="rank.php?times=total&amp;types=favo">更多&gt;&gt;</a></span>
				</div>
			</div>

			<!--最近更新、最新全本开始 -->
			<div id="rd8_newsmiddle">
				<ul id="rd8_newstabs" style="margin-bottom:0;">
					<li id="rd8_newstab1" onmouseover="newssetTab('rd8_newsmiddle','rd8_newscontent1','rd8_newstab1');" class="rd8_selectedtab" style="margin: 0 0 0 -40px;*margin: 0;">最近更新</li>
					<li id="rd8_newstab2" onmouseover="newssetTab('rd8_newsmiddle','rd8_newscontent2','rd8_newstab2');">最新全本</li>
				</ul>

				<div class="rd8_newscon" id="rd8_newscontent1">
					<ul>
<?php
	if(!empty($_obj['lastupdate'])) {
		if(!is_array($_obj['lastupdate']))
			$_obj['lastupdate'] = array(array('lastupdate' => $_obj['lastupdate']));
		$_tmp_arr_keys = array_keys($_obj['lastupdate']);
		if($_tmp_arr_keys[0] != '0')
			$_obj['lastupdate'] = array(0 => $_obj['lastupdate']);
		$_stack[$_stack_cnt++] = $_obj;
		foreach($_obj['lastupdate'] as $rowcnt=>$lastupdate) {
			$lastupdate['I'] = $rowcnt;
			$lastupdate['II'] = $rowcnt+1;
			$lastupdate['III'] = $rowcnt%7;
			$lastupdate['F_uid'] = $GLOBALS['_assign']['F_uid'];
			$lastupdate['F_username'] = $GLOBALS['_assign']['F_username'];
			$lastupdate['G_sitename'] = $GLOBALS['_assign']['G_sitename'];
			$lastupdate['G_siteurl'] = $GLOBALS['_assign']['G_siteurl'];
			$lastupdate['G_style'] = $GLOBALS['_assign']['G_style'];
			$lastupdate['G_tpldir'] = $GLOBALS['_assign']['G_tpldir'];
			$lastupdate['G_PHPSELF'] = $GLOBALS['_assign']['G_PHPSELF'];
			$lastupdate['G_encode'] = $GLOBALS['_assign']['G_encode'];
			$lastupdate['TIMESTAMP'] = $GLOBALS['_assign']['TIMESTAMP'];
			$lastupdate['A'] = $rowcnt % 2;
			$lastupdate['B'] = $rowcnt % 2;
			$_obj = &$lastupdate;
?>
						<li><span class="rd8_lisubject">[<?php
echo $_obj['subject'];
?>
]</span> <a href="<?php
echo $_obj['InfoURL'];
?>
">《<?php
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
				</div>

				<div class="rd8_newscon" id="rd8_newscontent2" style="display: none">
					<ul>
<?php
	if(!empty($_obj['lastfinish'])) {
		if(!is_array($_obj['lastfinish']))
			$_obj['lastfinish'] = array(array('lastfinish' => $_obj['lastfinish']));
		$_tmp_arr_keys = array_keys($_obj['lastfinish']);
		if($_tmp_arr_keys[0] != '0')
			$_obj['lastfinish'] = array(0 => $_obj['lastfinish']);
		$_stack[$_stack_cnt++] = $_obj;
		foreach($_obj['lastfinish'] as $rowcnt=>$lastfinish) {
			$lastfinish['I'] = $rowcnt;
			$lastfinish['II'] = $rowcnt+1;
			$lastfinish['III'] = $rowcnt%7;
			$lastfinish['F_uid'] = $GLOBALS['_assign']['F_uid'];
			$lastfinish['F_username'] = $GLOBALS['_assign']['F_username'];
			$lastfinish['G_sitename'] = $GLOBALS['_assign']['G_sitename'];
			$lastfinish['G_siteurl'] = $GLOBALS['_assign']['G_siteurl'];
			$lastfinish['G_style'] = $GLOBALS['_assign']['G_style'];
			$lastfinish['G_tpldir'] = $GLOBALS['_assign']['G_tpldir'];
			$lastfinish['G_PHPSELF'] = $GLOBALS['_assign']['G_PHPSELF'];
			$lastfinish['G_encode'] = $GLOBALS['_assign']['G_encode'];
			$lastfinish['TIMESTAMP'] = $GLOBALS['_assign']['TIMESTAMP'];
			$lastfinish['A'] = $rowcnt % 2;
			$lastfinish['B'] = $rowcnt % 2;
			$_obj = &$lastfinish;
?>
						<li><span class="rd8_lisubject">[<?php
echo $_obj['subject'];
?>
]</span> <a href="<?php
echo $_obj['InfoURL'];
?>
">《<?php
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
				</div>
			</div>
			<!--最近更新、最新全本结束 -->

			<div class="rd8_newsright">
				<div class="rd8_Canada">
					<dl>
						<dt>月点击榜</dt>
<?php
	if(!empty($_obj['custom_ydjb'])) {
		if(!is_array($_obj['custom_ydjb']))
			$_obj['custom_ydjb'] = array(array('custom_ydjb' => $_obj['custom_ydjb']));
		$_tmp_arr_keys = array_keys($_obj['custom_ydjb']);
		if($_tmp_arr_keys[0] != '0')
			$_obj['custom_ydjb'] = array(0 => $_obj['custom_ydjb']);
		$_stack[$_stack_cnt++] = $_obj;
		foreach($_obj['custom_ydjb'] as $rowcnt=>$custom_ydjb) {
			$custom_ydjb['I'] = $rowcnt;
			$custom_ydjb['II'] = $rowcnt+1;
			$custom_ydjb['III'] = $rowcnt%7;
			$custom_ydjb['F_uid'] = $GLOBALS['_assign']['F_uid'];
			$custom_ydjb['F_username'] = $GLOBALS['_assign']['F_username'];
			$custom_ydjb['G_sitename'] = $GLOBALS['_assign']['G_sitename'];
			$custom_ydjb['G_siteurl'] = $GLOBALS['_assign']['G_siteurl'];
			$custom_ydjb['G_style'] = $GLOBALS['_assign']['G_style'];
			$custom_ydjb['G_tpldir'] = $GLOBALS['_assign']['G_tpldir'];
			$custom_ydjb['G_PHPSELF'] = $GLOBALS['_assign']['G_PHPSELF'];
			$custom_ydjb['G_encode'] = $GLOBALS['_assign']['G_encode'];
			$custom_ydjb['TIMESTAMP'] = $GLOBALS['_assign']['TIMESTAMP'];
			$custom_ydjb['A'] = $rowcnt % 2;
			$custom_ydjb['B'] = $rowcnt % 2;
			$_obj = &$custom_ydjb;
?>
						<dd><a href="<?php
echo $_obj['InfoURL'];
?>
"><?php
echo $_obj['title'];
?>
</a>(<?php
echo $_obj['click_month'];
?>
)</dd>
<?php
}
$_obj=$_stack[--$_stack_cnt];}
?>
					</dl>
					<span><a href="rank.php?times=month&amp;types=click">更多&gt;&gt;</a></span>
				</div>

				<div class="rd8_Canada" style="margin-bottom:0;">
					<dl>
						<dt>月推荐榜</dt>
<?php
	if(!empty($_obj['custom_ytjb'])) {
		if(!is_array($_obj['custom_ytjb']))
			$_obj['custom_ytjb'] = array(array('custom_ytjb' => $_obj['custom_ytjb']));
		$_tmp_arr_keys = array_keys($_obj['custom_ytjb']);
		if($_tmp_arr_keys[0] != '0')
			$_obj['custom_ytjb'] = array(0 => $_obj['custom_ytjb']);
		$_stack[$_stack_cnt++] = $_obj;
		foreach($_obj['custom_ytjb'] as $rowcnt=>$custom_ytjb) {
			$custom_ytjb['I'] = $rowcnt;
			$custom_ytjb['II'] = $rowcnt+1;
			$custom_ytjb['III'] = $rowcnt%7;
			$custom_ytjb['F_uid'] = $GLOBALS['_assign']['F_uid'];
			$custom_ytjb['F_username'] = $GLOBALS['_assign']['F_username'];
			$custom_ytjb['G_sitename'] = $GLOBALS['_assign']['G_sitename'];
			$custom_ytjb['G_siteurl'] = $GLOBALS['_assign']['G_siteurl'];
			$custom_ytjb['G_style'] = $GLOBALS['_assign']['G_style'];
			$custom_ytjb['G_tpldir'] = $GLOBALS['_assign']['G_tpldir'];
			$custom_ytjb['G_PHPSELF'] = $GLOBALS['_assign']['G_PHPSELF'];
			$custom_ytjb['G_encode'] = $GLOBALS['_assign']['G_encode'];
			$custom_ytjb['TIMESTAMP'] = $GLOBALS['_assign']['TIMESTAMP'];
			$custom_ytjb['A'] = $rowcnt % 2;
			$custom_ytjb['B'] = $rowcnt % 2;
			$_obj = &$custom_ytjb;
?>
						<dd><a href="<?php
echo $_obj['InfoURL'];
?>
"><?php
echo $_obj['title'];
?>
</a>(<?php
echo $_obj['commend_month'];
?>
)</dd>
<?php
}
$_obj=$_stack[--$_stack_cnt];}
?>
					</dl>
					<span><a href="rank.php?times=month&amp;types=commend">更多&gt;&gt;</a></span>
				</div>
			</div>
		</div>

		<!-- 一级分类示例图书标签切换开始 -->
		<div id="rd8_catetabs">
			<ul id="rd8_tabs">
<?php
	if(!empty($_obj['custom_dpflbt'])) {
		if(!is_array($_obj['custom_dpflbt']))
			$_obj['custom_dpflbt'] = array(array('custom_dpflbt' => $_obj['custom_dpflbt']));
		$_tmp_arr_keys = array_keys($_obj['custom_dpflbt']);
		if($_tmp_arr_keys[0] != '0')
			$_obj['custom_dpflbt'] = array(0 => $_obj['custom_dpflbt']);
		$_stack[$_stack_cnt++] = $_obj;
		foreach($_obj['custom_dpflbt'] as $rowcnt=>$custom_dpflbt) {
			$custom_dpflbt['I'] = $rowcnt;
			$custom_dpflbt['II'] = $rowcnt+1;
			$custom_dpflbt['III'] = $rowcnt%7;
			$custom_dpflbt['F_uid'] = $GLOBALS['_assign']['F_uid'];
			$custom_dpflbt['F_username'] = $GLOBALS['_assign']['F_username'];
			$custom_dpflbt['G_sitename'] = $GLOBALS['_assign']['G_sitename'];
			$custom_dpflbt['G_siteurl'] = $GLOBALS['_assign']['G_siteurl'];
			$custom_dpflbt['G_style'] = $GLOBALS['_assign']['G_style'];
			$custom_dpflbt['G_tpldir'] = $GLOBALS['_assign']['G_tpldir'];
			$custom_dpflbt['G_PHPSELF'] = $GLOBALS['_assign']['G_PHPSELF'];
			$custom_dpflbt['G_encode'] = $GLOBALS['_assign']['G_encode'];
			$custom_dpflbt['TIMESTAMP'] = $GLOBALS['_assign']['TIMESTAMP'];
			$custom_dpflbt['A'] = $rowcnt % 2;
			$custom_dpflbt['B'] = $rowcnt % 2;
			$_obj = &$custom_dpflbt;
?>
				<li id="rd8_tab<?php
echo $_obj['subject_id'];
?>
" onmouseover="setTab('rd8_catetabs', 'rd8_content<?php
echo $_obj['subject_id'];
?>
', 'rd8_tab<?php
echo $_obj['subject_id'];
?>
');"<?php
if($_obj['II'] == "1") {
?> class="selectedtab"<?php
}
?>><?php
echo $_obj['title'];
?>
</li>
<?php
}
$_obj=$_stack[--$_stack_cnt];}
?>
			</ul>

<?php
	if(!empty($_obj['custom_dpmgl'])) {
		if(!is_array($_obj['custom_dpmgl']))
			$_obj['custom_dpmgl'] = array(array('custom_dpmgl' => $_obj['custom_dpmgl']));
		$_tmp_arr_keys = array_keys($_obj['custom_dpmgl']);
		if($_tmp_arr_keys[0] != '0')
			$_obj['custom_dpmgl'] = array(0 => $_obj['custom_dpmgl']);
		$_stack[$_stack_cnt++] = $_obj;
		foreach($_obj['custom_dpmgl'] as $rowcnt=>$custom_dpmgl) {
			$custom_dpmgl['I'] = $rowcnt;
			$custom_dpmgl['II'] = $rowcnt+1;
			$custom_dpmgl['III'] = $rowcnt%7;
			$custom_dpmgl['F_uid'] = $GLOBALS['_assign']['F_uid'];
			$custom_dpmgl['F_username'] = $GLOBALS['_assign']['F_username'];
			$custom_dpmgl['G_sitename'] = $GLOBALS['_assign']['G_sitename'];
			$custom_dpmgl['G_siteurl'] = $GLOBALS['_assign']['G_siteurl'];
			$custom_dpmgl['G_style'] = $GLOBALS['_assign']['G_style'];
			$custom_dpmgl['G_tpldir'] = $GLOBALS['_assign']['G_tpldir'];
			$custom_dpmgl['G_PHPSELF'] = $GLOBALS['_assign']['G_PHPSELF'];
			$custom_dpmgl['G_encode'] = $GLOBALS['_assign']['G_encode'];
			$custom_dpmgl['TIMESTAMP'] = $GLOBALS['_assign']['TIMESTAMP'];
			$custom_dpmgl['A'] = $rowcnt % 2;
			$custom_dpmgl['B'] = $rowcnt % 2;
			$_obj = &$custom_dpmgl;
?>
			<div class="rd8_catecon" id="rd8_content<?php
echo $_obj['subject_id'];
?>
"<?php
if($_obj['II'] > "1") {
?> style="display: none"<?php
}
?>>
<?php
	if(!empty($_obj['custom__data'])) {
		if(!is_array($_obj['custom__data']))
			$_obj['custom__data'] = array(array('custom__data' => $_obj['custom__data']));
		$_tmp_arr_keys = array_keys($_obj['custom__data']);
		if($_tmp_arr_keys[0] != '0')
			$_obj['custom__data'] = array(0 => $_obj['custom__data']);
		$_stack[$_stack_cnt++] = $_obj;
		foreach($_obj['custom__data'] as $rowcnt=>$custom__data) {
			$custom__data['I'] = $rowcnt;
			$custom__data['II'] = $rowcnt+1;
			$custom__data['III'] = $rowcnt%7;
			$custom__data['F_uid'] = $GLOBALS['_assign']['F_uid'];
			$custom__data['F_username'] = $GLOBALS['_assign']['F_username'];
			$custom__data['G_sitename'] = $GLOBALS['_assign']['G_sitename'];
			$custom__data['G_siteurl'] = $GLOBALS['_assign']['G_siteurl'];
			$custom__data['G_style'] = $GLOBALS['_assign']['G_style'];
			$custom__data['G_tpldir'] = $GLOBALS['_assign']['G_tpldir'];
			$custom__data['G_PHPSELF'] = $GLOBALS['_assign']['G_PHPSELF'];
			$custom__data['G_encode'] = $GLOBALS['_assign']['G_encode'];
			$custom__data['TIMESTAMP'] = $GLOBALS['_assign']['TIMESTAMP'];
			$custom__data['A'] = $rowcnt % 2;
			$custom__data['B'] = $rowcnt % 2;
			$_obj = &$custom__data;
?>
				<p class="rd8_typeimgtxt">
					<a href="<?php
echo $_obj['InfoURL'];
?>
">
						<img src="<?php
echo $_obj['HTMLURL'];
?>
/template/bookface.jpg" width="100" height="125" align="left" /><strong class="rd8_bookname"><?php
echo $_obj['title'];
?>
</strong>
					</a>
					<br /><br />
					<strong class="rd8_detail">作品类型：</strong><?php
echo $_obj['subject'];
?>
<br />
					<strong class="rd8_detail">作 &nbsp; &nbsp;者：</strong><?php
echo $_obj['author'];
?>
<br />
					<strong class="rd8_detail">更新日期：</strong><?php
echo rdate($_obj['lastupdate'],'m-d');
?>
<br />
				</p>
<?php
}
$_obj=$_stack[--$_stack_cnt];}
?>
			</div>
<?php
}
$_obj=$_stack[--$_stack_cnt];}
?>
		</div>
		<!-- 一级分类示例图书标签切换结束 -->

		<div class="rd8_authorcomment">
			<div class="rd8_Canada">
				<dl>
					<dt>DIGG</dt>
<?php
	if(!empty($_obj['custom_digg'])) {
		if(!is_array($_obj['custom_digg']))
			$_obj['custom_digg'] = array(array('custom_digg' => $_obj['custom_digg']));
		$_tmp_arr_keys = array_keys($_obj['custom_digg']);
		if($_tmp_arr_keys[0] != '0')
			$_obj['custom_digg'] = array(0 => $_obj['custom_digg']);
		$_stack[$_stack_cnt++] = $_obj;
		foreach($_obj['custom_digg'] as $rowcnt=>$custom_digg) {
			$custom_digg['I'] = $rowcnt;
			$custom_digg['II'] = $rowcnt+1;
			$custom_digg['III'] = $rowcnt%7;
			$custom_digg['F_uid'] = $GLOBALS['_assign']['F_uid'];
			$custom_digg['F_username'] = $GLOBALS['_assign']['F_username'];
			$custom_digg['G_sitename'] = $GLOBALS['_assign']['G_sitename'];
			$custom_digg['G_siteurl'] = $GLOBALS['_assign']['G_siteurl'];
			$custom_digg['G_style'] = $GLOBALS['_assign']['G_style'];
			$custom_digg['G_tpldir'] = $GLOBALS['_assign']['G_tpldir'];
			$custom_digg['G_PHPSELF'] = $GLOBALS['_assign']['G_PHPSELF'];
			$custom_digg['G_encode'] = $GLOBALS['_assign']['G_encode'];
			$custom_digg['TIMESTAMP'] = $GLOBALS['_assign']['TIMESTAMP'];
			$custom_digg['A'] = $rowcnt % 2;
			$custom_digg['B'] = $rowcnt % 2;
			$_obj = &$custom_digg;
?>
					<dd><a href="<?php
echo $_obj['InfoURL'];
?>
"><?php
echo $_obj['title'];
?>
</a>(<?php
echo $_obj['digg_today'];
?>
)</dd>
<?php
}
$_obj=$_stack[--$_stack_cnt];}
?>
				</dl>
			</div>

			<!-- 最新评论开始 -->
			<div class="rd8_comment">
				<dl>
					<dt> 最新评论 </dt>
<?php
	if(!empty($_obj['custom_zxpl'])) {
		if(!is_array($_obj['custom_zxpl']))
			$_obj['custom_zxpl'] = array(array('custom_zxpl' => $_obj['custom_zxpl']));
		$_tmp_arr_keys = array_keys($_obj['custom_zxpl']);
		if($_tmp_arr_keys[0] != '0')
			$_obj['custom_zxpl'] = array(0 => $_obj['custom_zxpl']);
		$_stack[$_stack_cnt++] = $_obj;
		foreach($_obj['custom_zxpl'] as $rowcnt=>$custom_zxpl) {
			$custom_zxpl['I'] = $rowcnt;
			$custom_zxpl['II'] = $rowcnt+1;
			$custom_zxpl['III'] = $rowcnt%7;
			$custom_zxpl['F_uid'] = $GLOBALS['_assign']['F_uid'];
			$custom_zxpl['F_username'] = $GLOBALS['_assign']['F_username'];
			$custom_zxpl['G_sitename'] = $GLOBALS['_assign']['G_sitename'];
			$custom_zxpl['G_siteurl'] = $GLOBALS['_assign']['G_siteurl'];
			$custom_zxpl['G_style'] = $GLOBALS['_assign']['G_style'];
			$custom_zxpl['G_tpldir'] = $GLOBALS['_assign']['G_tpldir'];
			$custom_zxpl['G_PHPSELF'] = $GLOBALS['_assign']['G_PHPSELF'];
			$custom_zxpl['G_encode'] = $GLOBALS['_assign']['G_encode'];
			$custom_zxpl['TIMESTAMP'] = $GLOBALS['_assign']['TIMESTAMP'];
			$custom_zxpl['A'] = $rowcnt % 2;
			$custom_zxpl['B'] = $rowcnt % 2;
			$_obj = &$custom_zxpl;
?>
					<dd><a href="<?php
echo $_obj['InfoURL'];
?>
">&nbsp;<?php
echo $_obj['title'];
?>
</a> <font class="rd8_commenttime"><?php
echo $_obj['username'];
?>
 于 <?php
echo rdate($_obj['dateline'],'m-d H:i');
?>
 评  <strong>《<?php
echo $_obj['title'];
?>
》</strong></font> </dd>
<?php
}
$_obj=$_stack[--$_stack_cnt];}
?>
				</dl>
			</div>
			<!-- 最新评论结束 -->
		</div>
		<!-- 友情链接开始 -->
		<div class="rd8_unionwebsite">
<?php
	if(!empty($_obj['friendlinks'])) {
		if(!is_array($_obj['friendlinks']))
			$_obj['friendlinks'] = array(array('friendlinks' => $_obj['friendlinks']));
		$_tmp_arr_keys = array_keys($_obj['friendlinks']);
		if($_tmp_arr_keys[0] != '0')
			$_obj['friendlinks'] = array(0 => $_obj['friendlinks']);
		$_stack[$_stack_cnt++] = $_obj;
		foreach($_obj['friendlinks'] as $rowcnt=>$friendlinks) {
			$friendlinks['I'] = $rowcnt;
			$friendlinks['II'] = $rowcnt+1;
			$friendlinks['III'] = $rowcnt%7;
			$friendlinks['F_uid'] = $GLOBALS['_assign']['F_uid'];
			$friendlinks['F_username'] = $GLOBALS['_assign']['F_username'];
			$friendlinks['G_sitename'] = $GLOBALS['_assign']['G_sitename'];
			$friendlinks['G_siteurl'] = $GLOBALS['_assign']['G_siteurl'];
			$friendlinks['G_style'] = $GLOBALS['_assign']['G_style'];
			$friendlinks['G_tpldir'] = $GLOBALS['_assign']['G_tpldir'];
			$friendlinks['G_PHPSELF'] = $GLOBALS['_assign']['G_PHPSELF'];
			$friendlinks['G_encode'] = $GLOBALS['_assign']['G_encode'];
			$friendlinks['TIMESTAMP'] = $GLOBALS['_assign']['TIMESTAMP'];
			$friendlinks['A'] = $rowcnt % 2;
			$friendlinks['B'] = $rowcnt % 2;
			$_obj = &$friendlinks;
?>
			<a href="http://<?php
echo $_obj['url'];
?>
" target="_blank"><?php
echo $_obj['name'];
?>
</a>
<?php
}
$_obj=$_stack[--$_stack_cnt];}
?>
		</div>
		<!-- 友情链接结束 -->
	</div>
