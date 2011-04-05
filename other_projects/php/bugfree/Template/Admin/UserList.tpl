{include file="Admin/AdminHeader.tpl"}
<body class="AdminBody">
{include file="Admin/AdminTopNav.tpl"}
  <div class="AdminAction">
  {$PaginationHtml}{if !$OtherUserDB}<a href="AdminUser.php?ActionType=AddUser" class="BigButton">{$Lang.AddUser}</a>{/if}
    <form class="AdminSearch" name="SearchUserForm" method="get">
    <input type="text" id="SearchUser" name="SearchUser" value="{$SearchUser}" />
    &nbsp;
    <input type="submit" class="Btn" value="{$Lang.PostQuery}"/>
    <input type="reset" class="Btn" value="{$Lang.ResetQuery}" onclick="window.location.href= 'AdminUserList.php?reset=1'" />
    </form>
  </div>
  <table class="CommonTable AdminTable">
    <tr>
      {if !$OtherUserDB}
      <th>{$Lang.UserID}</th>{/if}
      <th>{$Lang.UserName}</th>
      <th>{$Lang.AuthModeName}</th>
      <th>{$Lang.RealName}</th>
      <th>{$Lang.Email}</th>
      <th width="20%">{$Lang.GroupList}</th>
      {if !$OtherUserDB}
      <th>{$Lang.Edit}</th>{/if}
      <th>{$Lang.LastModifiedBy}</th>
      <th>{$Lang.LastTime}</th>
      <th>{$Lang.AddedBy}</th>
      <th>{$Lang.AddTime}</th>
    </tr>
    {foreach item=User from=$UserList}
    <tr{if $User.IsDroped eq '1'} class="Droped" {/if}>
      {if !$OtherUserDB}
      <td>{$User.UserID}</td>{/if}
      <td>{$User.UserName}</td>
      <td>{$User.AuthModeName}</td>
      <td>{$User.RealName}</td>
      <td>{$User.Email}</td>
      <td>{$User.UserGroupListHTML}</td>
      {if !$OtherUserDB}
      <td>
	    {if $templatelite.session.TestIsAdmin || $templatelite.SESSION.TestUserName eq $User.AddedBy || $templatelite.SESSION.TestUserName eq $User.UserName}
          {if $User.AuthMode != 'LDAP'}
          <a href="AdminUser.php?ActionType=EditUser&UserID={$User.UserID}">{$Lang.Edit}</a>&nbsp;|
          {/if}
          {if $templatelite.SESSION.TestUserName neq $User.UserName}
            {if $User.IsDroped eq '0'}
              <a href="AdminUser.php?ActionType=EditUser&UserID={$User.UserID}&amp;IsDroped=0" onclick="return confirm('{$Lang.ConfirmDelUser}');">{$Lang.DropUser}</a>
            {else}
              <a href="AdminUser.php?ActionType=EditUser&UserID={$User.UserID}&amp;IsDroped=1" onclick="return confirm('{$Lang.ConfirmActiveUser}');">{$Lang.ActiveUser}</a>
            {/if}
 		  {/if}
        {/if}
      </td>
      <td>{assign var="UserName" value=$User.LastEditedBy}{$UserNameList[$UserName]}</td>
	  <td>{if $User.LastDate neq $CFG.ZeroTime}{$User.LastDate}{/if}</td>
	  <td>{assign var="UserName" value=$User.AddedBy}{$UserNameList[$UserName]}</td>
	  <td>{if $User.AddDate neq $CFG.ZeroTime}{$User.AddDate}{/if}</td>{/if}
    </tr>
    {/foreach}
  </table>
{literal}
<script type="text/javascript">
//<![CDATA[
xajax.$('SearchUser').focus();
//]]>
</script>
{/literal}
</body>
</html>
