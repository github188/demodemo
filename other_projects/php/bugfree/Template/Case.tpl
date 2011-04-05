{include file="Header.tpl"}
<body class="CaseMode CaseMain" onload="TestMode='Case';initShowGotoBCR();">

{if $ActionType eq 'Edited'}
    {assign var="EditMode" value= 'true'}
{/if}

{if $ActionType eq 'OpenCase'}
    <form id="CaseForm" name="CaseForm" action="PostAction.php?Action=OpenCase" target="PostActionFrame" enctype="multipart/form-data" method="post">
{elseif $EditMode eq 'true'}
  <form id="CaseForm" name="CaseForm" action="PostAction.php?Action=EditCase" target="PostActionFrame" enctype="multipart/form-data" method="post">
{else}
  <form id="CaseForm" name="CaseForm" action="Case.php?CaseID={$CaseInfo.CaseID}" method="post" target="_self">
{/if}

<div id="TopNavMain" class="TopBar">
  <a id="TopNavLogo" href="./" target="_top"><img src="Image/logo.png" title={$Lang.ProductName} /></a>
  <!-- Set the Toolbar buttons status by Lichuan Liu -->
  {if $PreCaseID eq 0 or $EditMode eq 'true'}{assign var="PreButtonStatus" value = 'disabled="disabled"'}{/if}
  {if $NextCaseID eq 0 or $EditMode eq 'true'}{assign var="NextButtonStatus" value = 'disabled="disabled"'}{/if}
  {if $ActionType eq 'OpenCase' or $EditMode eq 'true'}{assign var="EditButtonStatus" value = 'disabled="disabled"'}{/if}
  {if $ActionType eq 'OpenCase' or $EditMode eq 'true'}{assign var="RunCaseButtonStatus" value = 'disabled="disabled"'}{/if}
  {if $ActionType eq ''}{assign var="SaveButtonStatus" value = 'disabled="disabled"'}{/if}
  
  <div id="ButtonList">
    <input type="button" class="ActionButton Btn" accesskey="P" value="{$Lang.PreButton}" onclick="location.href='Case.php?CaseID={$PreCaseID}'" {$PreButtonStatus}/>
    <input type="button" class="ActionButton Btn" accesskey="N" value="{$Lang.NextButton}" onclick="location.href='Case.php?CaseID={$NextCaseID}'" {$NextButtonStatus}/>
    <input type="button" class="ActionButton Btn" accesskey="E" value="{$Lang.EditCaseButton}" onclick="xajax.$('ActionType').value='Edited';submitForm('CaseForm')" {$EditButtonStatus}}/>
    <input type="button" class="ActionButton Btn" accesskey="C" value="{$Lang.CopyCaseButton}" onclick="xajax.$('ActionType').value='OpenCase';submitForm('CaseForm')" {$EditButtonStatus}}/>
    <input type="button" class="ActionButton Btn" accesskey="R" value="{$Lang.RunCaseButton}" {$RunCaseButtonStatus} onclick="openWindow('Result.php?ActionType=OpenResult&CaseID={$CaseInfo.CaseID}','OpenResult');"/>
    <input type="button" class="ActionButton Btn" accesskey="S" value="{$Lang.SaveButton}" name="SubmitButton" id="SubmitButton" onclick="this.disabled='disabled';NeedToConfirm=false;document.CaseForm.submit();" {$SaveButtonStatus}/>
  </div>
</div>

<div id="CaseMain" class="CommonForm CaseMode">
  <span id="CaseId">{if $ActionType eq 'OpenCase'}{$Lang.OpenCase}{else}{$CaseInfo.CaseID}{/if}</span>
  <div id="CaseSummaryInfo">
    <dl style="line-height:17pt">
      <dt>{$Lang.CaseFields.CaseTitle}</dt>
      <dd>
          {if $ActionType eq 'OpenCase' or $EditMode eq 'true'}
            <input type="text" id="CaseTitle" name="CaseTitle" class="MyInput RequiredField" value="{$CaseInfo.CaseTitle}" style="width:535px;"/>
          {else}
            <input type="text" id="CaseTitle" name="CaseTitle" title="{$CaseInfo.CaseTitle}" class="MyInput ReadOnlyField" readonly=readonly value="{$CaseInfo.CaseTitle}" style="width:535px;"/>
          {/if}
      </dd>
    </dl>
    <dl style="line-height:17pt">
      <dt>{$Lang.CaseFields.ProjectName}/{$Lang.CaseFields.ModulePath}</dt>
         {if $ActionType eq 'OpenCase' or $EditMode eq 'true'}
            <dd>{$ProjectList}</dd>
             <dd id="SlaveModuleList">{$ModuleList}</dd>
        {else}
          <dd>{$CaseInfo.ProjectName}{$CaseInfo.ModulePath}</dd>
        {/if}
    </dl>
  </div>
  <div id="CaseMainInfo">
        <table style="width: 100%">
            <tr>
                <td style="width: 30%" valign="top">
              <fieldset class="Normal FloatLeft" style="width: 95%">
                <legend>{$Lang.CaseStatusInfo}</legend>
                <dl>
                  <dt>{$Lang.CaseFields.CaseStatus}</dt>
                  <dd>
                      {if $ActionType eq 'OpenCase'}
                      {$Lang.CaseStatuses.Active}
                      {elseif $EditMode eq 'true'}
                      {$CaseStatusSelectList}
                      {else}
{$CaseInfo.CaseStatusName}
{/if}
                  </dd>
                </dl>
                <dl style="line-height:17pt">
                  <dt>{$Lang.CaseFields.AssignedTo}</dt>
                  <dd>
                               {if $ActionType eq 'OpenCase' or $EditMode eq 'true'}
                          <span id="AssignedToUserList">{$AssignedToUserList}</span>
                      {else}
                                    {$CaseInfo.AssignedToName}
                                {/if}
                  </dd>
                </dl>
                <dl style="line-height:17pt">
                  <dt>{$Lang.CaseFields.MailTo}</dt>
                  <dd>
                               {if $ActionType eq 'OpenCase' or $EditMode eq 'true'}
                          <input id="MailTo" name="MailTo" type="text" value="{$CaseInfo.MailTo}" size="20" maxlength="255" AUTOCOMPLETE="OFF" />
                      {else}
                          <input id="MailTo" name="MailTo" type="text" value="{$CaseInfo.MailToName}" size="20" maxlength="255" readonly=true class="ReadOnlyField" AUTOCOMPLETE="OFF" />
                      {/if}
                  </dd>
                </dl>
                <dl style="line-height:17pt">
                  <dt>{$Lang.CaseFields.CasePriority}</dt>
                  <dd>
                               {if $ActionType eq 'OpenCase' or $EditMode eq 'true'}
                          {$CasePrioritySelectList}
                      {else}
                                    {$CaseInfo.CasePriorityName}
                                {/if}
                  </dd>
                </dl>
                <dl style="line-height:17pt">
                  <dt>{$Lang.CaseFields.CaseType}</dt>
                  <dd>
                               {if $ActionType eq 'OpenCase' or $EditMode eq 'true'}
                          {$CaseTypeSelectList}
                      {else}
                          {$CaseInfo.CaseTypeName}
                                {/if}
                  </dd>
                </dl>
                <dl style="line-height:17pt">
                  <dt>{$Lang.CaseFields.CaseMethod}</dt>
                  <dd>
                               {if $ActionType eq 'OpenCase' or $EditMode eq 'true'}
                          {$CaseMethodSelectList}
                      {else}
                          {$CaseInfo.CaseMethodName}
                                {/if}
                  </dd>
                </dl>
                <dl style="line-height:17pt">
                  <dt>{$Lang.CaseFields.CasePlan}</dt>
                  <dd>
                               {if $ActionType eq 'OpenCase' or $EditMode eq 'true'}
                          {$CasePlanSelectList}
                      {else}
                          {$CaseInfo.CasePlanName}
                                {/if}
                  </dd>
                </dl>
                <dl>
                  <dt>{$Lang.CaseFields.LastEditedBy}</dt>
                    <dd>
                                {if $ActionType eq 'OpenCase'}
                        {else}
                            {$CaseInfo.LastEditedByName}
                        {/if}
                    </dd>
                </dl>
                <dl>
                  <dt>{$Lang.CaseFields.LastEditedDate}</dt>
                    <dd>
                                {if $ActionType eq 'OpenCase'}
                        {else}
                            {$CaseInfo.LastEditedDate|date_format:"%Y-%m-%d"}
                        {/if}
                    </dd>
                </dl>
              </fieldset>
                </td>
                <td style="width: 40%" valign="top">
                <fieldset class="Normal FloatLeft" style="width: 94%">
                  <legend>{$Lang.CaseOpenedInfo}</legend>
                  <dl>
                    <dt>{$Lang.CaseFields.OpenedBy}</dt>
                    <dd>
                                {if $ActionType eq 'OpenCase'}
                            {$templatelite.session.TestRealName}
                        {else}
                            {$CaseInfo.OpenedByName}
                        {/if}
                    </dd>
                  </dl>
                  <dl>
                    <dt>{$Lang.CaseFields.OpenedDate}</dt>
                    <dd>
                                {if $ActionType eq 'OpenCase'}
                            {$templatelite.now|date_format:"%Y-%m-%d"}
                        {else}
                            {$CaseInfo.OpenedDate|date_format:"%Y-%m-%d"}
                        {/if}
                    </dd>
                  </dl>
                </fieldset>
                <fieldset class="Normal FloatLeft" style="width: 95%">
                  <legend>{$Lang.CaseAutomationInfo}</legend>
                  <dl style="line-height:17pt">
                    <dt>{$Lang.CaseFields.ScriptStatus}</dt>
                  <dd id="BuildContainer">
                      {if $ActionType eq 'OpenCase' or $EditMode eq 'true'}
                      {$ScriptStatusSelectList}
                      {else}
                          {$CaseInfo.ScriptStatusName}<input type="hidden" name="ScriptStatus" value="{$CaseInfo.ScriptStatus}" />
                        {/if}
                  </dd>
                  </dl>
                  <dl style="line-height:17pt">
                    <dt>{$Lang.CaseFields.ScriptedBy}</dt>
                    <dd>
                       {if $ActionType eq 'OpenCase' or $EditMode eq 'true'}
                       <span id="ScriptedByList">
                         {$ScriptedBySelectList}
                       </span>
                        {else}
                          {$CaseInfo.ScriptedByName}
                        {/if}
                    </dd>
                  </dl>
                  <dl style="line-height:17pt">
                    <dt>{$Lang.CaseFields.ScriptedDate}</dt>
                    <dd>
                       {if $ActionType eq 'OpenCase' or $EditMode eq 'true'}
                        <input type=text name="ScriptedDate" id="ScriptedDate" size="12" class="MyInput" value="{$CaseInfo.ScriptedDate}" readonly="readonly" />
<script type="text/javascript">
{literal}
  Calendar.setup(
    {
      inputField  : "ScriptedDate",
      ifFormat    : "%Y-%m-%d",
      button      : "ScriptedDate"
    }
  );
{/literal}
</script>
                        {else}
                  {$CaseInfo.ScriptedDate}
                {/if}
                    </dd>
                  </dl>
                 <dl style="line-height:17pt">
                  <dt>{$Lang.CaseFields.ScriptLocation}</dt>
                  <dd>
                     {if $ActionType eq 'OpenCase' or $EditMode eq 'true'}
                        <input id="ScriptLocation" name="ScriptLocation" type="text" value="{$CaseInfo.ScriptLocation}" size="33" maxlength="255" AUTOCOMPLETE="OFF" />
                     {else}
                        <input id="ScriptLocation" name="ScriptLocation" type="text" value="{$CaseInfo.ScriptLocation}" size="33" maxlength="255" readonly=true class="ReadOnlyField" AUTOCOMPLETE="OFF"  title="{$CaseInfo.ScriptLocation}" />
                        {if $CaseInfo.ScriptLocation neq ''}<a href="{$CaseInfo.ScriptLocation}" title="{$CaseInfo.ScriptLocation}" target="_blank"><img src="Image/file.png" /></a>{/if}
                     {/if}
                  </dd>
                 </dl>
                </fieldset>
                </td>
                <td style="width: 33%" valign="top">
                <fieldset class="Normal FloatLeft" style="width: 94%">
                  <legend>{$Lang.CaseOtherInfo}</legend>
                  <dl style="line-height:17pt">
                    <dt>{$Lang.CaseFields.MarkForDeletion}</dt>
                    <dd>
                      {if $ActionType eq 'OpenCase'}
                          {$Lang.MarkForDeletions.0}
                      {elseif $EditMode eq 'true'}
                          {$CaseMarkForDeletionSelectList}
                      {else}
                          {$CaseInfo.MarkForDeletionName}
                                {/if}
                    </dd>
                  </dl>
                  <dl style="line-height:17pt">
                    <dt>{$Lang.CaseFields.CaseKeyword}</dt>
                    <dd>
                               {if $ActionType eq 'OpenCase' or $EditMode eq 'true'}
                            <input type="text" name="CaseKeyword" id="CaseKeyword" size="20" class="MyInput" maxlength="80" value="{$CaseInfo.CaseKeyword}" />
                        {else}
                            <input type="text" name="CaseKeyword" id="CaseKeyword" size="20" class="MyInput ReadOnlyField" maxlength="80" value="{$CaseInfo.CaseKeyword}" />
                        {/if}
                    </dd>
                  </dl>

                  <dl style="line-height:17pt">
                  <dt>{$Lang.CaseFields.DisplayOrder}</dt>
                  <dd>
                      {if $ActionType eq 'OpenCase' or $EditMode eq 'true'}
                          <input id="DisplayOrder" name="DisplayOrder" class="MyInput" type="text" value="{if $CaseInfo.DisplayOrder eq ''}0{else}{$CaseInfo.DisplayOrder}{/if}" size="20" maxlength="255" AUTOCOMPLETE="OFF" />
                      {else}
                          <input id="DisplayOrder" name="DisplayOrder" type="text" value="{$CaseInfo.DisplayOrder}" size="20" maxlength="255" readonly=true class="ReadOnlyField" AUTOCOMPLETE="OFF" />
                      {/if}
                  </dd>
                </dl>
                </fieldset>
                <fieldset class="Normal FloatLeft" style="width: 94%">
                  <legend>{$Lang.CaseConditionInfo}</legend>
                  <dl style="line-height:17pt">
                    <dt>{$Lang.CaseFields.BugID}</dt>
                    <dd>
                       {if $ActionType eq 'OpenCase' or $EditMode eq 'true'}
                          <input type="text" name="BugID" id="BugID" size="20" class="MyInput" value="{$CaseInfo.BugID}" />
                       {else}
                         {foreach from=$CaseInfo.BugIDList item="BugID"}
                           <a href="Bug.php?BugID={$BugID}" target="_blank">{$BugID}</a>
                         {/foreach}
                       {/if}
                    </dd>
                  </dl>
                  <dl style="line-height:17pt">
                    <dt>{$Lang.CaseFields.LinkID}</dt>
                    <dd>
                       {if $ActionType eq 'OpenCase' or $EditMode eq 'true'}
                          <input type="text" name="LinkID" id="LinkID" size="20" class="MyInput" value="{$CaseInfo.LinkID}" />
                       {else}
                         {foreach from=$CaseInfo.LinkIDList item="LinkID"}
                           <a href="Case.php?CaseID={$LinkID}" target="_blank">{$LinkID}</a>
                         {/foreach}
                       {/if}
                    </dd>
                  </dl>
				  <dl style="line-height:17pt">
                    <dt>{$Lang.CaseFields.ResultID}</dt>
                    <dd>
                    {if $ActionType neq 'OpenCase'}
                        {foreach from=$CaseInfo.ResultIDList item="ResultID"}
                            <a href="Result.php?ResultID={$ResultID}" target="_blank">{$ResultID}</a>
                        {/foreach}
                    {/if}
                    </dd>
                  </dl>
                </fieldset>
                <fieldset class="Normal FloatLeft" style="width: 94%">
                  <legend>{$Lang.CaseFiles}</legend>
                   <dl>
                    <dd style="text-align:left;">
                      {assign var="FileList" value=$CaseInfo.FileList}
                      {include file="FileInfo.tpl"}
                    </dd>
                  </dl>
                </fieldset>
                 </td>
            </tr>
        </table>
    </div>

  <div id="CaseHistory">
      <table style="width: 100%">
            <tr>
              <td style="width: 50%" valign="top">
                <fieldset class="Normal FloatLeft" id="CaseHistoryInfo" style="width: 98%">
                  <legend>{$Lang.Comments}</legend>
                  {if $ActionType eq 'OpenCase' or $EditMode eq 'true'}
                      <textarea id="ReplyNote" name="ReplyNote" rows="5" style="overflow-y:visible;" ></textarea>
                  {/if}
                  {include file="CaseHistory.tpl"}
                </fieldset>
              </td>
                <td style="width: 50%" valign="top">
                    <fieldset class="Normal FloatLeft" id="CaseStepsInfo" style="width: 96%">
                  <legend>{$Lang.CaseStepsInfo}</legend>
                  {if $ActionType eq 'OpenCase' or $EditMode eq 'true'}
                    <textarea id="CaseSteps" name="CaseSteps" rows="11" style="overflow-y:visible;">{if $ActionType eq 'OpenCase' and $CopyCaseID <=0 }
{$Lang.CaseDefaultStepInfo}{else}
{$CaseInfo.CaseSteps}{/if}</textarea>
                  {else}
                    <p style="overflow: auto">{$CaseInfo.CaseSteps|replace:" ":"&nbsp;"|bbcode2html}</p>
                  {/if}
                </fieldset>
              </td>
            </tr>
        </table>
  </div>

</div>

<input type="hidden" id="CaseID" name="CaseID" value="{$CaseInfo.CaseID}" />
<input type="hidden" id="DeleteFileIDs" name="DeleteFileIDs" value="" />
<input type="hidden" id="ActionType" name="ActionType" value="{$ActionType}" />
<input type="hidden" id="ActionObj" name="ActionObj" value="Case" />
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
initSelectDiv('MailTo','selectDivProjectUserList','getInputSearchValueByComma','setValueByComma', true);
xajax.$('ReplyNote').focus();
{/literal}
{/if}
{if $ActionType eq 'OpenCase'}
    {literal}focusTextLast('CaseTitle');{/literal}
{/if}
{literal}
//]]>
</script>
{/literal}
</body>
</html>
