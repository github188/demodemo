{include file="Admin/AdminHeader.tpl"}
<body class="AdminBody">
{include file="Admin/AdminTopNav.tpl"}
  <div class="AdminAction">
  {$PaginationHtml}
    <form class="AdminSearch" name="SearchLogForm" method="get">
      <input type="text" id="SearchLog" name="SearchLog" value="{$SearchLog}" />
      &nbsp;<input type="submit" class="Btn" value="{$Lang.PostQuery}"/>
      <input type="reset" class="Btn" value="{$Lang.ResetQuery}" onclick="window.location.href= 'AdminUserLogList.php?reset=1'" />
    </form>
  </div>
  <table class="Commontable AdminTable">
    <tr>
      <th>{$Lang.LogID}</th>
      <th>{$Lang.UserName}</th>
      <th>{$Lang.LoginIP}</th>
      <th>{$Lang.LoginTime}</th>
    </tr>
    {foreach item=UserLog from=$UserLogList}
    <tr>
      <td>{$UserLog.LogID}</td>
      <td>{$UserLog.UserName}</td>
      <td>{$UserLog.LoginIP}</td>
      <td>{$UserLog.LoginTime}</td>
    </tr>
    {/foreach}
  </table>
</body>
</html>
