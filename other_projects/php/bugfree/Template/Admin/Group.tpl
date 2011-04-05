{include file="Admin/AdminHeader.tpl"}
<body class="AdminBody">
{include file="Admin/AdminTopNav.tpl"}
  <div class="AdminNav">
    &lt;<a href="AdminGroupList.php">{$Lang.BackToGroupList}</a>&#124;
    <strong>{$GroupInfo.GroupName}</strong>
  </div>
  {include file="ActionMessage.tpl"}
  <form id="GroupForm" action="" method="post" onsubmit="xajax_xAdmin{$ActionType}(xajax.getFormValues('GroupForm'));return false;">
  <div id="GForm" class="CommonForm AdminForm">
    <h1>{if $ActionType eq 'AddGroup'}{$Lang.AddGroup}{else}{$Lang.EditGroup}{/if}</h1>
    <dl>
      <dt><label for="GroupName">{$Lang.GroupName}</label></dt>
      <dd><input type="text" id="GroupName" name="GroupName" value="{$GroupInfo.GroupName}" size="30" /><dd>
    </dl>
    <dl class="Line"></dl>
    <dl>
      <dt><label>{$Lang.GroupUser}</label></dt>
      <dd>{$UserList2}</dd>
      <dd class="SelectButton">
        <input type="button" value="&gt;&gt;" onclick="displaceSelect('GroupUserList','GroupUserNames','UserList2');"/><br />
        <input type="button" value="&lt;&lt;" onclick="displaceSelect('GroupUserList','GroupUserNames');"/>
      </dd>
      <dd>{$GroupUserList}</dd>
    </dl>
    <dl>
      <dt><label>{$Lang.GroupManager}</label></dt>
      <dd>{$UserList}</dd>
      <dd class="SelectButton">
        <input type="button" value="&gt;&gt;" onclick="displaceSelect('GroupManagerList','GroupManagerNames','UserList');"/><br />
        <input type="button" value="&lt;&lt;" onclick="displaceSelect('GroupManagerList','GroupManagerNames');"/>
      </dd>
      <dd>{$GroupManagerList}</dd>
    </dl>
    <dl class="Line"></dl>
    <dl>
      <input type="submit" class="Btn" name="SaveGroup" value="{$Lang.SaveGroup}" />
      <input type="hidden" name="GroupID" value="{$GroupInfo.GroupID}" />
      <input type="hidden" name="GroupUserNames" id="GroupUserNames" value="{$GroupInfo.GroupUserNames}" />
      <input type="hidden" name="GroupManagerNames" id="GroupManagerNames" value="{$GroupInfo.GroupManagerNames}" />
    </dl>
  </div>
  </form>
</body>
</html>
