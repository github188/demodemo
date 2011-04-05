{include file="Admin/AdminHeader.tpl"}
<body class="AdminBody">
{include file="Admin/AdminTopNav.tpl"}
  <div class="AdminNav">
    &lt;<a href="AdminProjectList.php">{$Lang.BackToProjectList}</a>&#124;
    <strong>{$ProjectInfo.ProjectName}</strong>
  </div>
  {include file="ActionMessage.tpl"}
  <form id="ProjectForm" action="" method="post" onsubmit="if( xajax.$('NotifyEmail').value.trim()=='' || xajax.$('NotifyEmail').value.match(/^\w+([-+.']\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*(,\w+([-+.']\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*)*$/)) xajax_xAdmin{$ActionType}(xajax.getFormValues('ProjectForm')); else alert('{$Lang.WrongEmailFormat}'); return false;">
  <div id="PForm" class="CommonForm AdminForm">
    <h1>{if $ActionType eq 'AddProject'}{$Lang.AddProject}{elseif $ActionType eq 'EditProject'}{$Lang.EditProject}{/if}</h1>
    <dl>
      <dt><label for="ProjectName"><strong>{$Lang.ProjectName}</strong></label></dt>
      <dd>{if $TestIsProjectAdmin}{$ProjectInfo.ProjectName}
        <input type="hidden" id="ProjectName" name="ProjectName" value="{$ProjectInfo.ProjectName}"/>{else}
        <input type="text" id="ProjectName" name="ProjectName" value="{$ProjectInfo.ProjectName}" size="20" />{/if}
    </dd>
    </dl>
    <dl>
      <dt><label for="DisplayOrder"><strong>{$Lang.DisplayOrder}</strong></label></dt>
      <dd>{if $TestIsProjectAdmin}{$ProjectInfo.DisplayOrder}
        <input type="hidden" id="DisplayOrder" name="DisplayOrder" value="{$ProjectInfo.DisplayOrder|default:0}"/>{else}
        <input type="text" id="DisplayOrder" name="DisplayOrder" value="{$ProjectInfo.DisplayOrder|default:0}" size="20" />{$Lang.DisplayOrderNote}{/if}
    </dd>
    </dl>
    <dl>
      <dt><label for="NotifyEmail"><strong>{$Lang.NotifyEmail}</strong></label></dt>
      <dd>               <input type="text" id="NotifyEmail" name="NotifyEmail" value="{$ProjectInfo.NotifyEmail}" size="50" /> {$Lang.MutipleEmails}
                  </dd>
     
    </dl>
    <dl class="Line"></dl>
    <dl>
      <dt><label>{$Lang.ProjectGroup}</label></dt>
      <dd>{$GroupList}</dd>
      <dd class="SelectButton">
        <input type="button" value="&gt;&gt;" onclick="displaceSelect('ProjectGroupList','ProjectGroupIDs','GroupList');"  /><br />
        <input type="button" value="&lt;&lt;" onclick="displaceSelect('ProjectGroupList','ProjectGroupIDs');" />
      </dd>
      <dd>{$ProjectGroupList}</dd>
    </dl>
    <dl>
      <dt><label>{$Lang.ProjectManager}</label></dt>
      <dd>{$UserList}</dd>
      <dd class="SelectButton">
        <input type="button" value="&gt;&gt;" onclick="displaceSelect('ProjectManagerList','ProjectManagers','UserList');"{if $TestIsProjectAdmin} disabled="disabled"{/if}/><br />
        <input type="button" value="&lt;&lt;" onclick="displaceSelect('ProjectManagerList','ProjectManagers');"{if $TestIsProjectAdmin} disabled="disabled"{/if}/>
      </dd>
      <dd>{$GroupUserList}</dd>
    </dl>
    <!--
    <dl></dl>
    <dl>
      <dt><label for="ProjectDoc">{$Lang.ProjectDoc}</label></dt>
      <dd><input type="text" id="ProjectDoc" name="ProjectDoc" value="" size="25" /></dd>
    </dl>
    <dl>
      <dt><label for="ProjectPlan">{$Lang.ProjectPlan}</label></dt>
      <dd><input type="text" id="ProjectPlan" name="ProjectPlan" value="" size="25" /></dd>
    </dl>-->
    <dl class="Line"></dl>
    <dl>
      <input type="submit" class="Btn" name="SaveProject" value="{$Lang.SaveProject}" />
      <input type="hidden" name="ProjectManagers" id="ProjectManagers" value="{$ProjectManagers}" />
      <input type="hidden" name="ProjectGroupIDs" id="ProjectGroupIDs" value="{$ProjectGroupIDs}" />
      <input type="hidden" name="ProjectID" id="ProjectID" value="{$ProjectInfo.ProjectID}" />
    </dl>
  </div>
  </form>
</body>
</html>
