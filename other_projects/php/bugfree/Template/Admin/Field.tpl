{include file="Admin/AdminHeader.tpl"}
<body class="AdminBody">
{include file="Admin/AdminTopNav.tpl"}
  <div class="AdminNav">
    &lt;<a href="AdminProjectList.php">{$Lang.BackToProjectList}</a>&#124;
    <strong>{if $ModuleType eq 'Bug'}{$Lang.ManageBugModule}{elseif  $ModuleType eq 'Case'}{$Lang.ManageCaseModule}{/if}</strong>
  </div>
  {include file="ActionMessage.tpl"}
{if $Action eq "AddField"}
<form id="FieldForm" action="" method="post" onsubmit="xajax_xAdminAddField('{$ModuleType}', '{$ProjectInfo.ProjectID}', xajax.getFormValues('FieldForm'));return false;">
{else}
<form id="FieldForm" action="" method="post" onsubmit="xajax_xAdminUpdateField('{$ModuleType}', '{$ProjectInfo.ProjectID}', xajax.getFormValues('FieldForm'));return false;">
{/if}
<table class="Field">
  <caption><a href="AdminModuleList.php?ProjectID={$ProjectInfo.ProjectID}&ModuleType={$ModuleType}">{$ProjectInfo.ProjectName}</a></caption>
  <tr>
    <th>{$Lang.FieldText}<div class="FieldTip" >{$Lang.FieldTextTip}</div></th>
    <td><input class="Text" type="text" name="FieldText" value="{$Field.FieldText}" /></td>
  </tr>
  <tr>
    <th>{$Lang.FieldName}<div class="FieldTip" >{$Lang.FieldNameTip}</div></th>
    <td>
      {if $Action eq "UpdateField"}
      <input class="Text ReadOnly" readonly="readonly" type="text" name="FieldName" value="{$Field.FieldName}" />
      {else}
      <input class="Text" type="text" name="FieldName" value="{$Field.FieldName}" />
      {/if}
      <input type="hidden" name="FieldOldName" value="{$Field.FieldName}" />
    </td>
  </tr>
  <tr>
    <th>{$Lang.FieldType}<div class="FieldTip" >{$Lang.FieldTypeTip}</div></th>
    <td>
    <input class="Radio" type="radio" name="FieldType" value="text" {if $Field.FieldType eq "text" or $Field eq null}checked="checked"{/if}/><span class="Radio">{$Lang.Field.Text}</span>
    <input class="Radio" type="radio" name="FieldType" value="select" {if $Field.FieldType eq "select"}checked="checked"{/if}/><span class="Radio">{$Lang.Field.Select}</span>
    <input class="Radio" type="radio" name="FieldType" value="date" {if $Field.FieldType eq "date"}checked="checked"{/if}/><span class="Radio">{$Lang.Field.Date}</span>
    <input class="Radio" type="radio" name="FieldType" value="user" {if $Field.FieldType eq "user"}checked="checked"{/if}/><span class="Radio">{$Lang.Field.User}</span>
    </td>
  </tr>
  <tr>
    <th>{$Lang.FieldValue}<div class="FieldTip" >{$Lang.FieldValueTip}</div></th>
    <td>
      <textarea name="FieldValue">{$Field.FieldValue}</textarea>
    </td>
  </tr>
  <tr>
    <th>{$Lang.FieldOption}<div class="FieldTip" >{$Lang.FieldOptionTip}</div></th>
    <td>
      <input class="Radio" type="radio" name="FieldOption" value="not null" {if $Field.FieldOption eq "not null"}checked="checked"{/if}/><span class="Radio">{$Lang.FieldNotNull}</span>
      <input class="Radio" type="radio" name="FieldOption" value="null" {if $Field.FieldOption neq "not null" }checked="checked"{/if}/><span class="Radio">{$Lang.FieldNull}</span>
    </td>
  </tr>
  <tr>
    <th>{$Lang.FieldStatus}<div class="FieldTip" >{$Lang.FieldStatusTip}</div></th>
    <td>
      <input class="Radio" type="radio" name="FieldStatus" value="active" {if $Field.FieldStatus eq "active" or $Action neq "UpdateField"}checked="checked"{/if}/><span class="Radio">{$Lang.FieldActive}</span>
      <input class="Radio" type="radio" name="FieldStatus" value="disable" {if $Field.FieldStatus neq "active" and $Action eq "UpdateField"}checked="checked"{/if}/><span class="Radio">{$Lang.FieldDisable}</span>
    </td>
  </tr>
  <tr>
    <td class="Button" colspan="2">
      <input id="FieldSave" type="submit" class="Btn" value="{$Lang.Save}" />
      <input id="FieldReset" type="reset" class="Btn" value="{$Lang.Reset}" />
    </td>
  </tr>
</table>
</form>
</body>