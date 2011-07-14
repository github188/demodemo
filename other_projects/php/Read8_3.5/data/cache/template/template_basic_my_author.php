<?php
if(!defined('IN_Read8')) {
	exit('Access Denied (template_basic_my_author.php)');
}
?>
	<div class="rd8_fullborder">
		<form action="my/author_apply.php" method="post">
			<div class="rd8_head">申请作家</div>
			<div class="rd8_col0" style="width:25%;margin:1px auto;">
				<table>
					<tr class="rd8_col0" style="width:25%;">
						<td align="right">笔名:&nbsp;</td>
						<td>&nbsp;<?php
if($_obj['applyed'] == "1") {
?><?php
echo $_obj['pseudonym'];
?>
<?php
}else {
?><input type="text" name="pseudonym" /><?php
}
?></td>
					</tr>
					<tr class="rd8_colt">
						<td align="center" colspan="2"><?php
if($_obj['applyed'] == "1") {
?>您已使用 笔名: “<?php
echo $_obj['pseudonym'];
?>
” 申请成为作家，请耐心等待审核<?php
}else {
?><input type="submit" value=" 申请作家 " /><?php
}
?></td>
					</tr>
				</table>
			</div>
		</form>
	</div>
