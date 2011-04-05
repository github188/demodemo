<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type"     content="text/html; charset={$Lang.Charset}">
<meta http-equiv="Content-Language" content="{$Lang.Charset}">
<link href="Css/Basic.css" rel="stylesheet" type="text/css">
<link href="Css/calendar.css" rel="stylesheet" type="text/css">
{$xajax_javascript}
<script src="JS/Main.js"></script>
<script src="JS/overlib.js"></script>
<script src="JS/calendar.js"></script>
<script src="JS/calendar_{$CFG.DefaultLang}.js"></script>
<script src="JS/calendar-setup.js"></script>
<title>{$HeaderTitle|default:$Lang.ProductName}</title>
{if $ActionType != ''}
<script language="JavaScript">
  var NeedToConfirm = true;
  var ConfirmIds = {$ConfirmIds};
  var ConfirmValues = new Array();
  var ConfirmText = "{$Lang.NotSaveMod}";
  window.onbeforeunload = confirmWhenExit;
</script>
{/if}
<script language="JavaScript">
var TestMode="{$TestMode}";
</script>
<base target="{$BaseTarget|default:"RightBottomFrame"}">
</head>
