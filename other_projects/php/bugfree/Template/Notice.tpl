<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
<meta http-equiv="Content-Type"     content="text/html; charset={$Lang.Charset}">
<meta http-equiv="Content-Language" content="{$Lang.Charset}">
<style type="text/css">
{$CssStyle}
</style>
<title>{$Lang.Title}</title>
</head>
<body style="margin:16px;">
 <table width="98%" align="center" class="CommonTable BugMode">
    <tr>
      <th>{$Lang.BugFields.BugID}</th>
      <th>Sev</th>
      <th>Pri</th>
      <th>{$Lang.BugFields.BugTitle}</th>
      <th>{$Lang.BugFields.BugStatus}</th>
      <th>{$Lang.BugFields.OpenedBy}</th>
      <th>{$Lang.BugFields.AssignedTo}</th>
      <th>{$Lang.BugFields.ResolvedBy}</th>
      <th>{$Lang.BugFields.Resolution}</th>
      <th>{$Lang.BugFields.LastEditedDate}</th>
    </tr>
    {foreach from=$UserBugList item=BugInfo}
    <tr class="BugStatus{$BugInfo.BugStatus}">
      <td align="center">
        {$BugInfo.BugID}
      </td>
      <td align="center">{$BugInfo.BugSeverityName}</td>
      <td align="center">{$BugInfo.BugPriorityName}</td>
      <td title="{$BugInfo.BugTitle}">
        <a target="_blank" href="{$BaseUrl}/Bug.php?BugID={$BugInfo.BugID}">{$BugInfo.ListTitle}</a>
      </td>
      <td align="center">{$BugInfo.BugStatusName}</td>
      <td align="center">{$BugInfo.OpenedByName}</td>
      <td align="center">{$BugInfo.AssignedToName}</td>
      <td align="center">{$BugInfo.ResolvedByName}</td>
      <td align="center">{$BugInfo.ResolutionName}</td>
      <td align="center">{$BugInfo.LastEditedDate|date_format:"%Y-%m-%d"}</td>
    </tr>
    {/foreach}
  </table>
</body>
</html>
