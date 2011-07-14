<?php
if(!defined('IN_Read8')) {
	exit('Access Denied (template_basic_footer.php)');
}
?>
	<div id="rd8_footer">
		<p class="rd8_aboutus">
			<a href="about.php?action=about">关于我们</a> | <a href="about.php?action=guide">用户指南</a> | <a href="about.php?action=contact">联系方法</a> | <a href="<?php
echo $_obj['G_bbsurl'];
?>
">留言建议</a><br />
<?php
if(!empty($_obj['copyright'])) {
?>			<?php
echo $_obj['copyright'];
?>
<br /><?php
}
?>
			<?php
if(!empty($_obj['G_miibeian'])) {
?><a href="http://www.miibeian.gov.cn" rel="external"><?php
echo $_obj['G_miibeian'];
?>
</a><?php
if(!empty($_obj['G_statcode'])) {
?><?php
echo $_obj['G_statcode'];
?>
<?php
}
?><br /><?php
}
?>
			<?php
if(!empty($_obj['G_SpeedTrace'])) {
?>Processed in <?php
echo $_obj['C_totaltime'];
?>
 second(s), <?php
echo $_obj['C_dbcount'];
?>
 queries<br /><?php
}
?>
			<?php
echo $_obj['copyright_system'];
?>

		</p>
	</div>
	<div id="rd8_userstate">
		<div id="rd8_userlogin"></div>
		<div id="rd8_wap"><span class="rd8_a" onclick="location.href='wap';"><img src="images/wap.gif" align="absmiddle" />手机阅读</span></div>
		<div id="rd8_favorite"><span class="rd8_a" onclick="javascript:addBrowserFavorite('<?php
echo $_obj['G_sitename'];
?>
', '<?php
echo $_obj['G_siteurl'];
?>
');"><img src="images/favorite.gif" align="absmiddle" />加入收藏</span></div>
		<div id="rd8_homepage"><span class="rd8_a" onclick="javascript:setHomePage('<?php
echo $_obj['G_siteurl'];
?>
');"><img src="images/homepage.gif" align="absmiddle" />设为首页</span></div>
		<div id="rd8_font_lang"></div>
	</div>
</div>
<div id="pagecolor" style="display: none;"></div>
<!--主体结束 -->
<script src="user.php" type="text/javascript"></script>
<script src="include/js/lang.js" type="text/javascript"></script>
<script src="include/js/f_lang.js" type="text/javascript"></script>
</body>
</html>
