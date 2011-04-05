{include file="Admin/AdminHeader.tpl"}
<body class="AdminBody">
{include file="Admin/AdminTopNav.tpl"}
  <div class="AdminAction">
    {$PaginationHtml}
      {if $templatelite.session.TestIsAdmin}<a href="AdminProject.php?ActionType=AddProject" class="BigButton">{$Lang.AddProject}</a>{/if}
    <form class="AdminSearch" name="SearchProjectForm" method="get">
    <input type="text" id="SearchProject" name="SearchProject" value="{$SearchProject}" />
    &nbsp;<input class="Btn" type="submit" value="{$Lang.PostQuery}"/>
    <input class="Btn"  type="reset" value="{$Lang.ResetQuery}" onclick="window.location.href= 'AdminProjectList.php?reset=1'" />
    </form>
  </div>
  <table class="Commontable AdminTable">
    <tr>
      <th>{$Lang.DisplayOrder}</th>
      <th>{$Lang.ProjectID}</th>
      <th>{$Lang.ProjectName}</th>
      <th width="12%">{$Lang.ProjectGroup}</th>
      <th width="8%">{$Lang.ProjectManager}</th>
      <th>{$Lang.Edit}</th>
      <th>{$Lang.ManageModule}</th>
      <th>{$Lang.LastModifiedBy}</th>
      <th>{$Lang.LastTime}</th>
      <th>{$Lang.AddedBy}</th>
      <th>{$Lang.AddTime}</th>
    </tr>
    {foreach item=Project from=$ProjectList}
    <tr{if $Project.IsDroped eq '1'} class="Droped" {/if}>
      <td>{$Project.DisplayOrder}</td>
      <td>{$Project.ProjectID}</td>
      <td>{$Project.ProjectName}</td>
      <td>{$Project.ProjectGroupListHTML}</td>
      <td>{$Project.ProjectManagerListHTML}</td>
      <td>
        <a href="AdminProject.php?ActionType=EditProject&ProjectID={$Project.ProjectID}">{$Lang.Edit}</a>{if $templatelite.session.TestIsAdmin}|
        {if $Project.IsDroped eq '0'}
          <a href="AdminProject.php?ActionType=EditProject&ProjectID={$Project.ProjectID}&amp;IsDroped={$Project.IsDroped}" onclick="return confirm('{$Lang.ConfirmDropProject}');">
        {$Lang.DropProject}{else}
          <a href="AdminProject.php?ActionType=EditProject&ProjectID={$Project.ProjectID}&amp;IsDroped={$Project.IsDroped}" onclick="return confirm('{$Lang.ConfirmActiveProject}');">
        {$Lang.ActiveProject}{/if}
        </a>{/if}
      </td>
      <td>|
        <a href="AdminModuleList.php?ProjectID={$Project.ProjectID}&ModuleType=Bug">{$Lang.ManageBugModule}</a>|
        <a href="AdminModuleList.php?ProjectID={$Project.ProjectID}&ModuleType=Case">{$Lang.ManageCaseModule}</a>
	  </td>
      <td>{$Project.LastEditedByName}</td>
      <td>{$Project.LastDate}</td>
      <td>{$Project.AddedByName}</td>
      <td>{$Project.AddDate}</td>
    </tr>
    {/foreach}
  </table>
</body>
</html>
