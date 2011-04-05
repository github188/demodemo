{include file="Admin/AdminHeader.tpl"}
<body class="AdminBody">
{include file="Admin/AdminTopNav.tpl"}
  <div class="AdminAction">
  {$PaginationHtml}<a href="AdminGroup.php?ActionType=AddGroup" class="BigButton">{$Lang.AddGroup}</a>
    <form class="AdminSearch" name="SearchGroupForm" method="get">
      <input type="text" id="SearchGroup" name="SearchGroup" value="{$SearchGroup}" />
      &nbsp;<input type="submit" class="Btn" value="{$Lang.PostQuery}"/>
      <input type="reset" class="Btn" value="{$Lang.ResetQuery}" onclick="window.location.href= 'AdminGroupList.php?reset=1'" />
    </form>
  </div>
  <table class="CommonTable AdminTable">
    <tr>
      <th>{$Lang.GroupID}</th>
      <th>{$Lang.GroupName}</th>
      <th width="15%">{$Lang.GroupUser}</th>
      <th width="15%">{$Lang.GroupManager}</th>
	  <th>{$Lang.Edit}</th>
      <th>{$Lang.LastModifiedBy}</th>
      <th>{$Lang.LastTime}</th>
      <th>{$Lang.AddedBy}</th>
      <th>{$Lang.AddTime}</th>
      <th></th>
    </tr>
    {foreach item=Group from=$GroupList}
    <tr>
      <td>{$Group.GroupID}</td>
      <td>{$Group.GroupName}</td>
      <td>{if $Group.GroupID > 1}{$Group.GroupUserSingleSelect}{/if}</td>
      <td>{if $Group.GroupID > 1}{$Group.GroupManagerSingleSelect}{/if}</td>
      <td>
        {if $Group.GroupID > 1 && $Group.IsEditable}
        <a href="AdminGroup.php?ActionType=EditGroup&GroupID={$Group.GroupID}">{$Lang.Edit}</a>|
        <a href="AdminGroup.php?ActionType=EditGroup&DropGroupID={$Group.GroupID}" onclick="return confirm('{$Lang.ConfirmDelGroup}');">{$Lang.Delete}</a>
        {/if}
      </td>
      <td>{assign var="UserName" value=$Group.LastEditedBy}{$UserList[$UserName]}</td>
      <td>{$Group.LastDate}</td>
      <td>{assign var="UserName" value=$Group.AddedBy}{$UserList[$UserName]}</td>
      <td>{$Group.AddDate}</td>
    </tr>
    {/foreach}
  </table>
</body>
</html>
