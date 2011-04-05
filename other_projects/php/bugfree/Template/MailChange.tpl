<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
<meta http-equiv="Content-Type"     content="text/html; charset={$Lang.Charset}">
<meta http-equiv="Content-Language" content="{$Lang.Charset}">
<style type="text/css">
{$CssStyle}
</style>
<title>{$Lang.ProductName}</title>
</head>
<body style="margin:16px;">
  <table width="98%" align="center" class="CommonTable {$ObjType}Mode">
    <tr style="text-align:left;font-size:12px;padding:4px 8px;">
      <th style="border-bottom-width:1px;">
      {foreach from=$ObjList item=ObjID}
      <a target="_blank" href="{$CFG.BaseURL}/{$ObjType}.php?{$ObjType}ID={$ObjID}">[{$ObjType} #{$ObjID} => {$AssignedTo}]</a><br />
      {/foreach}
      <span><strong>{$ActionInfo|bbcode2html}</strong></span>
      </th>
    <tr>
    <tr>
      <td style="padding:12px;border:0;">
      <div>{$Notes|bbcode2html}</div>
      </td>
    </tr>
  </table>
</body>
</html>
