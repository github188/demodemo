{include file="Header.tpl"}
<body class="ResultMode ResultMain" onload="TestMode='Result';initShowGotoBCR();">

{literal}
<script type="text/javascript">
//<![CDATA[
{/literal}
var ActionType = '{$ActionType}';
var CaseListCreateResultsLen = '{$CaseListCreateResultsLen}';

{literal}
function confirmSaveResult(message)
{
  if(ActionType == 'OpenResults')
  {
     if(confirm(message.replace("{param0}",CaseListCreateResultsLen)))
        document.ResultForm.submit();
     else
        window.close();
  }
  else
     document.ResultForm.submit();
}
//]]>
</script>
{/literal}

{if $ActionType eq 'Edited'}
    {assign var="EditMode" value= 'true'}
{/if}

{if $ActionType eq 'OpenResult'}
    <form id="ResultForm" name="ResultForm" action="PostAction.php?Action=OpenResult" target="PostActionFrame" enctype="multipart/form-data" method="post">
{elseif $ActionType eq 'OpenResults'}
    <form id="ResultForm" name="ResultForm" action="PostAction.php?Action=OpenResults" target="PostActionFrame" enctype="multipart/form-data" method="post">
{elseif $EditMode eq 'true'}
  <form id="ResultForm" name="ResultForm" action="PostAction.php?Action=EditResult" target="PostActionFrame" enctype="multipart/form-data" method="post">
{else}
  <form id="ResultForm" name="ResultForm" action="Result.php?ResultID={$ResultInfo.ResultID}" method="post" target="_self">
{/if}

<div id="TopNavMain" class="TopBar">
  <a id="TopNavLogo" href="./" target="_top"><img src="Image/logo.png" title={$Lang.ProductName} /></a>
  <!-- Set the Toolbar buttons status by Lichuan Liu -->
  {if $PreResultID eq 0 or $EditMode eq 'true'}{assign var="PreButtonStatus" value = 'disabled="disabled"'}{/if}
  {if $NextResultID eq 0 or $EditMode eq 'true'}{assign var="NextButtonStatus" value = 'disabled="disabled"'}{/if}
  {if $ActionType eq 'OpenResult' or $ActionType eq 'OpenResults' or $EditMode eq 'true'}{assign var="EditButtonStatus" value = 'disabled="disabled"'}{/if}
  {if $ActionType eq 'OpenResult'or $ActionType eq 'OpenResults' or $EditMode eq 'true' or $ResultInfo.ResultValue eq 'Pass'}{assign var="OpenBugButtonStatus" value = 'disabled="disabled"'}{/if}
  {if $ActionType eq ''}{assign var="SaveButtonStatus" value = 'disabled="disabled"'}{/if}
  
  <div id="ButtonList">
    <input type="button" class="ActionButton Btn" accesskey="P" value="{$Lang.PreButton}" onclick="location.href='Result.php?ResultID={$PreResultID}'" {$PreButtonStatus}/>
    <input type="button" class="ActionButton Btn" accesskey="N" value="{$Lang.NextButton}" onclick="location.href='Result.php?ResultID={$NextResultID}'" {$NextButtonStatus}/>
    <input type="button" class="ActionButton Btn" accesskey="E" value="{$Lang.EditResultButton}" onclick="xajax.$('ActionType').value='Edited';submitForm('ResultForm')" {$EditButtonStatus}}/>
    <input type="button" class="ActionButton Btn" accesskey="B" value="{$Lang.OpenBugButton}" {$OpenBugButtonStatus}  onclick="openWindow('Bug.php?ActionType=OpenBug&ResultID={$ResultInfo.ResultID}','OpenBug');"/>
    <input type="button" class="ActionButton Btn" accesskey="S" value="{$Lang.SaveButton}" name="SubmitButton" id="SubmitButton" onclick="this.disabled='disabled';NeedToConfirm=false;confirmSaveResult('{$Lang.RunCasesConfirm}');" {$SaveButtonStatus}/>
  </div>
</div>

<div id="ResultMain" class="CommonForm">
  <span id="ResultId">{{if $ActionType eq 'OpenResult'}{$Lang.OpenResult}{elseif $ActionType eq 'OpenResults'}{$Lang.RunCases}{else}{$ResultInfo.ResultID}{/if}</span>
  <div id="ResultSummaryInfo">
    <dl style="line-height:17pt">
      <dt>{$Lang.ResultFields.ResultTitle}</dt>
      <dd>
        {if $ActionType eq 'OpenResult' or $ActionType eq 'OpenResults'}
        <span class="ResultValueBlank">&nbsp;</span>
        {elseif $EditMode eq 'true'}
        <span class="ResultValueBlank">&nbsp;</span>
        {else}
        <span class="ResultValue{$ResultInfo.ResultValueName}">&nbsp;</span>
        {/if}
      </dd>
      <dd>
          {if $ActionType eq 'OpenResult'}
          {$CaseInfo.CaseTitle}
          <input type="hidden" name="ResultTitle" value="{$CaseInfo.CaseTitle}" />
          {elseif $ActionType eq 'OpenResults'}
          {$Lang.MultipleCases}
          {else}{$ResultInfo.ResultTitle}<input type="hidden" name="ResultTitle" value="{$ResultInfo.ResultTitle}" />{/if}
      </dd>
    </dl>
    <dl style="line-height:17pt">
      <dt>{$Lang.ResultFields.ProjectName}/{$Lang.ResultFields.ModulePath}</dt>
      <dd><span class="ResultValueBlank">&nbsp;</span></dd>
         {if $ActionType eq 'OpenResult'}
            <dd>
               {$CaseInfo.ProjectName}
               <input type="hidden" name="ProjectID" value="{$CaseInfo.ProjectID}" />
            </dd>
             <dd>
               {$CaseInfo.ModulePath}
               <input type="hidden" name="ModuleID" value="{$CaseInfo.ModuleID}" />
             </dd>
        {elseif $ActionType eq 'OpenResults'}
          <dd>{$Lang.MultipleCases}</dd>
        {else}
          <dd>{$ResultInfo.ProjectName}{$ResultInfo.ModulePath}</dd>
        {/if}
    </dl>
  </div>
    
  <div id="ResultMainInfo">
        <table style="width: 100%">
            <tr>
                <td style="width: 30%" valign="top">
              <fieldset class="Normal FloatLeft" style="width: 94%">
                <legend>{$Lang.ResultStatusInfo}</legend>
                <dl style="line-height:17pt">
                  <dt>{$Lang.ResultFields.ResultValue}</dt>
                  <dd>
                      {if $ActionType eq 'OpenResult' or $ActionType eq 'OpenResults'}
                      {$ResultValueSelectList}
                      {elseif $EditMode eq 'true'}
                      {$ResultValueSelectList}
                      {else}
                      {$ResultInfo.ResultValueName}
                      {/if}
                  </dd>
                </dl>
                <dl style="line-height:17pt">
                  <dt>{$Lang.ResultFields.ResultStatus}</dt>
                  <dd>
                      {if $ActionType eq 'OpenResult' or $EditMode eq 'true' or $ActionType eq 'OpenResults'}
                      {$ResultStatusSelectList}
                      {else}
                      {$ResultInfo.ResultStatusName}
                      {/if}
                  </dd>
                </dl>
                <dl style="line-height:17pt">
                  <dt>{$Lang.ResultFields.AssignedTo}</dt>
                  <dd>
                               {if $ActionType eq 'OpenResult' or $EditMode eq 'true' or $ActionType eq 'OpenResults'}
                          <span id="AssignedToUserList">{$AssignedToUserList}</span>
                      {else}
                                    {$ResultInfo.AssignedToName}
                                {/if}
                  </dd>
                </dl>
                <dl style="line-height:17pt">
                  <dt>{$Lang.ResultFields.MailTo}</dt>
                  <dd>
                      {if $ActionType eq 'OpenResult' or $EditMode eq 'true' or $ActionType eq 'OpenResults'}
                          <input id="MailTo" name="MailTo" type="text" value="{$ResultInfo.MailTo}" size="16" maxlength="255" AUTOCOMPLETE="OFF" />
                      {else}
                          <input id="MailTo" name="MailTo" type="text" value="{$ResultInfo.MailToName}" size="16" maxlength="255" readonly=true class="ReadOnlyField" AUTOCOMPLETE="OFF" />
                      {/if}
                  </dd>
                </dl>
                <dl>
                  <dt>{$Lang.ResultFields.LastEditedBy}</dt>
                  <dd>{$ResultInfo.LastEditedByName}</dd>
                </dl>
                <dl>
                  <dt>{$Lang.ResultFields.LastEditedDate}</dt>
                  <dd>{$ResultInfo.LastEditedDate|date_format:"%Y-%m-%d"}</dd>
                </dl>
              </fieldset>
                </td>
                <td style="width: 40%" valign="top">
                <fieldset class="Normal FloatLeft" style="width: 95%">
                  <legend>{$Lang.ResultOpenedInfo}</legend>
                  <dl>
                    <dt>{$Lang.ResultFields.OpenedBy}</dt>
                    <dd>
                                {if $ActionType eq 'OpenResult' or $ActionType eq 'OpenResults'}
                            {$templatelite.session.TestRealName}
                        {else}
                            {$ResultInfo.OpenedByName}
                        {/if}
                    </dd>
                  </dl>
                  <dl>
                    <dt>{$Lang.ResultFields.OpenedDate}</dt>
                    <dd>
                                {if $ActionType eq 'OpenResult' or $ActionType eq 'OpenResults'}
                            {$templatelite.now|date_format:"%Y-%m-%d"}
                        {else}
                            {$ResultInfo.OpenedDate|date_format:"%Y-%m-%d"}
                        {/if}
                    </dd>
                  </dl>
                </fieldset>
                <fieldset class="Normal FloatLeft" style="width: 95%">
                  <legend>{$Lang.ResultEnvInfo}</legend>
                  <dl style="line-height:17pt">
                    <dt>{$Lang.ResultFields.ResultBuild}</dt>
                    <dd id="BuildContainer">
                      {if $ActionType eq 'OpenResult' or $ActionType eq 'OpenResults'}
		        	    <input type=text name="ResultBuild" id="ResultBuildInput" size="35" class="MyInput RequiredField" value ="{$ResultInfo.ResultBuild}" />
                      {elseif $ActionType eq 'Edited'}
					    <input type=text name="ResultBuild" id="ResultBuildInput" size="35" class="MyInput RequiredField" value ="{$ResultInfo.ResultBuild}" />
                      {else}
					    <input type=text name="ResultBuild" id="ResultBuildInput" size="35" class="MyInput ReadOnlyField" readonly=true value ="{$ResultInfo.ResultBuild}" />
                      {/if}
                    </dd>
                  </dl>
                  <dl style="line-height:17pt">
                    <dt>{$Lang.ResultFields.ResultOS}</dt>
                  <dd id="BuildContainer">
                      {if $ActionType eq 'OpenResult' or $EditMode eq 'true' or $ActionType eq 'OpenResults'}
                      {$ResultOSSelectList}
                      {else}
                          {$ResultInfo.ResultOSName}
                        {/if}
                  </dd>
                  </dl>
                  <dl style="line-height:17pt">
                    <dt>{$Lang.ResultFields.ResultBrowser}</dt>
                  <dd id="BuildContainer">
                      {if $ActionType eq 'OpenResult' or $EditMode eq 'true' or $ActionType eq 'OpenResults'}
                          {$ResultBrowserSelectList}
                      {else}
                          {$ResultInfo.ResultBrowserName}
                      {/if}
                  </dd>
                  </dl>
                </fieldset>
                </td>
                <td style="width: 30%" valign="top">
                <fieldset class="Normal FloatLeft" style="width: 94%">
                  <legend>{$Lang.ResultOtherInfo}</legend>
                  <dl style="line-height:17pt">
                    <dt>{$Lang.ResultFields.ResultMachine}</dt>
                    <dd id="BuildContainer">
                      {if $ActionType eq 'OpenResult' or $EditMode eq 'true' or $ActionType eq 'OpenResults'}
                        <input type=text name="ResultMachine" id="ResultMachine" size="17" class="MyInput" maxlength="80" value="{$ResultInfo.ResultMachine}" />
                      {else}
                        <input type=text name="ResultMachine" id="ResultMachine" size="17" class="MyInput ReadOnlyField" readonly=true maxlength="80" value="{$ResultInfo.ResultMachine}" />
                      {/if}
                    </dd>
				  </dl>
                  <dl style="line-height:17pt">
                    <dt>{$Lang.ResultFields.ResultKeyword}</dt>
                    <dd>
                        {if $ActionType eq 'OpenResult' or $EditMode eq 'true' or $ActionType eq 'OpenResults'}
                          <input type="text" name="ResultKeyword" id="ResultKeyword" size="17" class="MyInput" maxlength="80" value="{$ResultInfo.ResultKeyword}" />
                        {else}
                          <input type="text" name="ResultKeyword" id="ResultKeyword" size="17" class="MyInput ReadOnlyField" readonly=true maxlength="80" value="{$ResultInfo.ResultKeyword}" />
                        {/if}
                    </dd>
                  </dl>
                </fieldset>
                <fieldset class="Normal FloatLeft" style="width: 94%">
                  <legend>{$Lang.ResultConditionInfo}</legend>
                  <dl style="line-height:17pt">
                    <dt>{$Lang.ResultFields.CaseID}</dt>
                    <dd>
                        {if $ActionType eq 'OpenResult'}
                        <a href="Case.php?CaseID={$CaseInfo.CaseID}" target="_blank">{$CaseInfo.CaseID}</a>
                        <input type="hidden" name="CaseID" value="{$CaseInfo.CaseID}" />
                        {elseif $ActionType eq 'OpenResults'}
                        {$Lang.MultipleCases}
                        {else}
                        <a href="Case.php?CaseID={$ResultInfo.CaseID}" target="_blank">{$ResultInfo.CaseID}</a>
                      {/if}
                    </dd>
                  </dl>
                  <dl style="line-height:17pt">
                    <dt>{$Lang.ResultFields.BugID}</dt>
                    <dd>
                       {if $ActionType eq 'OpenResults'}
                        {$Lang.MultipleCases}
                        {else}
                        {foreach from=$ResultInfo.BugIDList item="BugID"}
                            <a href="Bug.php?BugID={$BugID}" target="_blank">{$BugID}</a>
                        {/foreach}
                       {/if}
                    </dd>
                  </dl>
                </fieldset>
                <fieldset class="Normal FloatLeft" style="width: 94%">
                  <legend>{$Lang.ResultFiles}</legend>
                   <dl>
                    <dd style="text-align:left;">
                    {if $ActionType eq 'OpenResults'}
                    {else}
                      {assign var="FileList" value=$ResultInfo.FileList}
                      {include file="FileInfo.tpl"}
                      {/if}
                    </dd>
                </fieldset>
                 </td>
            </tr>
        </table>
    </div>

  <div id="ResultHistory">
      <table style="width: 100%">
            <tr>
              <td style="width: 50%" valign="top">
                <fieldset class="Normal FloatLeft" id="ResultHistoryInfo" style="width: 98%">
                  <legend>{$Lang.Comments}</legend>
                  {if $ActionType eq 'OpenResult' or $EditMode eq 'true' or $ActionType eq 'OpenResults'}
                      <textarea id="ReplyNote" name="ReplyNote" rows="5" style="overflow-y:visible;" ></textarea>
                  {/if}
                  {include file="ResultHistory.tpl"}
                </fieldset>
              </td>
                <td style="width: 50%" valign="top">
                    <fieldset class="Normal FloatLeft" id="ResultStepsInfo" style="width: 96%">
                  <legend>{$Lang.ResultStepsInfo}</legend>
                  {if $ActionType eq 'OpenResult' or $EditMode eq 'true'}
                    <textarea id="ResultSteps" name="ResultSteps" rows="11" style="overflow-y:visible;">{if $ActionType eq 'OpenResult'}
{$CaseInfo.CaseSteps}{else}
{$ResultInfo.ResultSteps}{/if}</textarea>
                  {elseif $ActionType eq 'OpenResults'}
                     {$Lang.MultipleCases}
                  {else}
                    <p style="overflow: auto">{$ResultInfo.ResultSteps|replace:" ":"&nbsp;"|bbcode2html}</p>
                  {/if}
                </fieldset>
              </td>
            </tr>
        </table>
  </div>

</div>

<input type="hidden" id="ResultID" name="ResultID" value="{$ResultInfo.ResultID}" />
<input type="hidden" id="DeleteFileIDs" name="DeleteFileIDs" value="" />
<input type="hidden" id="ActionType" name="ActionType" value="{$ActionType}" />
<input type="hidden" id="ActionObj" name="ActionObj" value="Result" />
<input type="hidden" id="TestUserName" name="TestUserName" value="{$templatelite.session.TestUserName}" />
<input type="hidden" id="TestRealName" name="TestRealName" value="{$templatelite.session.TestRealName}" />
<input type="hidden" id="ToDisabledObj" name="ToDisabledObj" value="SubmitButton" />
<input type="hidden" id="CurrentProjectID" name="CurrentProjectID" value="{$ProjectID}" />
<input type="hidden" id="LastAcitonID" name="LastActionID" value="{$LastActionID}" />


</form>
{include file="PostActionFrame.tpl"}
{literal}
<script type="text/javascript">
//<![CDATA[
function superAddObjValue(objID,addValue)
{
    xajax.$(objID).value += ',' + addValue;
}
{/literal}
{if $ActionType neq ''}
{literal}
setConfirmExitArrays();
initSelectDiv('MailTo','selectDivProjectUserList','getInputSearchValueByComma','setValueByComma',true);
xajax.$('ReplyNote').focus();
{/literal}
{/if}
{if $ActionType eq 'OpenResult' or $ActionType eq 'OpenResults'}
    {literal}focusInputEndPos('ResultBuild');{/literal}
{/if}
{literal}
//]]>
</script>
{/literal}
</body>
</html>
