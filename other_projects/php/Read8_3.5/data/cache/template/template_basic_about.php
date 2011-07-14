<?php
if(!defined('IN_Read8')) {
	exit('Access Denied (template_basic_about.php)');
}
?>
<div class="rd8_main">
	<div class="rd8_full">
		<div class="rd8_fullborder">
			<div class="rd8_head"><?php
echo $_obj['C_abouttitle'];
?>
</div>
			<div class="rd8_col0" style="width: 99%;padding-left: 1%;"><?php
echo $_obj['C_content'];
?>
</div>
		</div>
	</div>
</div>