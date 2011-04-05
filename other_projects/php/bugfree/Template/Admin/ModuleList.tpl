{include file="Admin/AdminHeader.tpl"}
<body class="AdminBody">
<link href="../Css/TreeMenu.css" rel="stylesheet" type="text/css">
<script language="JavaScript" src="../JS/TreeMenu.js" type="text/javascript"></script>
{include file="Admin/AdminTopNav.tpl"}
  <div class="AdminNav">
    &lt;<a href="AdminProjectList.php">{$Lang.BackToProjectList}</a>&#124;
    <strong>{if $ModuleType eq 'Bug'}{$Lang.ManageBugModule}{elseif  $ModuleType eq 'Case'}{$Lang.ManageCaseModule}{/if}</strong>
  </div>
  {include file="ActionMessage.tpl"}
  <div id="ModuleList" class="CommonDiv" style="margin-top:3px;width:300px;height:60%;overflow:auto;background-color:#FFFFFF;">
    {$ModuleList}
  </div>
  <form id="EditModuleForm" action="" method="post" onsubmit="xajax.$('SaveModuleSubmit').disabled='disabled';xajax_xAdminEditModule(xajax.getFormValues('EditModuleForm'));return false;">
  <div id="EMForm" class="CommonForm AdminForm" style="width:500px;float:left;">
  {if $ModuleID > 0}
    <h1>{$Lang.EditModule}</h1>
    <dl>
      <dt>{$Lang.OwnProject}</dt>
      <dd>{$SelectProjectList}</dd>
    </dl>
    <dl>
      <dt>{$Lang.ParentModule}</dt>
      <dd id="SlaveModuleList">{$SelectEditModuleList}</dd>
    </dl>
    <dl></dl>
    <dl>
      <dt><label for="ModuleName"><strong>{$Lang.ModuleName}</strong></label></dt>
      <dd><input type="text" id="ModuleName" name="ModuleName" value="{$ModuleInfo.ModuleName}" size="20" /></dd>
    </dl>
    <dl>
      <dt><label for="DisplayOrder"><strong>{$Lang.DisplayOrder}</strong></label></dt>
      <dd><input type="text" id="DisplayOrder" name="DisplayOrder" value="{$ModuleInfo.DisplayOrder}" size="20" />{$Lang.DisplayOrderNote}</dd>
    </dl>
    <dl>
      <dt>{$Lang.ModuleOwner}</dt>
      <dd id="AssignedToUserList">{$SelectEditModuleUserList}</dd>
    </dl>
    <dl class="Line"></dl>
    <dl>
      <input type="hidden" name="ModuleID" value="{$ModuleID}" />
      <input type="hidden" name="ModuleType" value="{$ModuleType}" />
      <input type="submit" class="Btn" id="SaveModuleSubmit" name="SaveModuleSubmit" value="{$Lang.SaveModule}" />
      <input type="submit" class="Btn" name="DeleteModuleSbumit" value="{$Lang.DeleteModule}" onclick="{literal}if(confirm('{/literal}{$Lang.ConfirmDeleteModule}{literal}')){xajax.$('DeleteModule').value=1;return true;}else{return false;}{/literal}"/>
      <input type="hidden" id="DeleteModule" name="DeleteModule" value="0" />
    </dl>{else}
    <h1>{$Lang.MergeProject}</h1>
    <dl>
      <dt>{$Lang.MergeTo}</dt>
      <dd>{$SelectProjectList}</dd>
    </dl>
    <dl>
      <dt>{$Lang.ParentModule}</dt>
      <dd id="SlaveModuleList">{$SelectEditModuleList}</dd>
    </dl>
    <dl></dl>
    <dl>
      <dt><label for="ModuleName"><strong>{$Lang.MergeName}</strong></label></dt>
      <dd><input type="text" id="ModuleName" name="ModuleName" value="{$ProjectInfo.ProjectName}" size="20"  /></dd>
    </dl>
    <dl>
      <dt><label for="DisplayOrder"><strong>{$Lang.DisplayOrder}</strong></label></dt>
      <dd><input type="text" id="DisplayOrder" name="DisplayOrder" value="{$ModuleInfo.DisplayOrder}" size="20" />{$Lang.DisplayOrderNote}</dd>
    </dl>
    <dl>
      <dt>{$Lang.ModuleOwner}</dt>
      <dd id="AssignedToUserList">{$SelectEditModuleUserList}</dd>
    </dl>
    <dl class="Line"></dl>
    <dl>
      <input type="hidden" name="RawProjectID" value="{$ProjectID}" />
      <input type="hidden" name="ModuleID" value="{$ModuleID}" />
      <input type="hidden" name="ModuleType" value="{$ModuleType}" />
      <input type="submit" class="Btn" id="SaveModuleSubmit" name="SaveModuleSubmit" value="{$Lang.Save}" />
      <input type="hidden" id="DeleteModule" name="DeleteModule" value="0" />
    </dl>{/if}
  </div>
  </form>
  <form id="AddModuleForm" action="" method="post" onsubmit="xajax_xAdminAddModule(xajax.getFormValues('AddModuleForm'));return false;">
  <div id="AMForm" class="CommonForm AdminForm" style="width:500px;float:left;margin-top:20px">
    <h1>{$Lang.AddModule}</h1>
    <dl>
      <dt>{$Lang.ParentModule}</dt>
      <dd>{$SelectAddModuleList}</dd>
    </dl>
    <dl></dl>
    <dl>
      <dt><label for="ModuleName"><strong>{$Lang.ModuleName}</strong></label></dt>
      <dd><input type="text" id="ModuleName" name="ModuleName" value="" size="20" /></dd>
    </dl>
    <dl>
      <dt><label for="DisplayOrder"><strong>{$Lang.DisplayOrder}</strong></label></dt>
      <dd><input type="text" id="DisplayOrder" name="DisplayOrder" value="0" size="20" />{$Lang.DisplayOrderNote}</dd>
    </dl>
    <dl>
      <dt>{$Lang.ModuleOwner}</dt>
      <dd>{$SelectAddModuleUserList}</dd>
    </dl>
    <dl class="Line"></dl>
    <dl>
      <input type="hidden" name="ProjectID" value="{$ProjectID}" />
      <input type="hidden" name="ModuleType" value="{$ModuleType}" />
      <input type="submit" class="Btn" name="SaveProject" value="{$Lang.AddModule}" />
    </dl>
  </div>
  <script type="text/javascript">createTreeMenu("ModuleList");</script>
</body>
</html>
