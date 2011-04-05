{include file="XmlHeader.tpl"}
{include file="Header.tpl"}
{include file="UploadFile.tpl"}

<script>
if(parent.window==window)
   window.location='index.php?TestMode=Case';
{literal}

  
function hiddenDivPopUp()
{
    hiddenDivCustomSetTable();
    hiddenDivUploadSetTable();
}
{/literal}
</script>
{include file="ResizeList.tpl"}
<body class="{$TestMode}Mode RMode" onResize="resizeList();" onclick="hiddenDivPopUp()" onload="initShowGotoBCR();resizeList();">
{include file="CustomField.tpl"}
  <div id="BlankCover">
  <table class="CommonTable TableMode">
    <tr>
      <td colspan="{$FieldToShowCount}" class="TdCaption">
      <table style="font-size:12px;width:98%;">
        <tr>
          <td style="text-align:left;border:0;width:50%">
            &nbsp;&nbsp;
            <a href="javascript:void(0);" id="CustomSetLink" onclick="x=513;y=event.clientY;showDivCustomSetTable(x,y);">{$Lang.CustomDisplay}</a>&nbsp;|&nbsp;
            <span id="VReport"><a href="Report.php?ReportMode={$TestMode}" target="'{$TestMode}Report">{$Lang.ReportForms}</a></span>&nbsp;|&nbsp;
            <a href="?Export=XMLFile">{$Lang.ExportCases}</a>&nbsp;|&nbsp;
            <a href="javascript:void(0);" onclick="x=event.clientX+document.body.scrollLeft;y=event.clientY;showUploadFile(x+420,y);">{$Lang.ImportCases}</a>&nbsp;|&nbsp;
           <!--  <a href="ImportCases.php" target="_blank">&nbsp;{$Lang.ImportCases}</a>|-->
          {if $PaginationDetailInfo.RecTotal eq 0}
          <a href="#" onclick="alert('{$Lang.RunCasesNotNull}')">{$Lang.RunCases}</a>
          {elseif $PaginationDetailInfo.RecTotal le 100}
          <a href="Result.php?ActionType=OpenResults" target='_blank'>{$Lang.RunCases}</a> 
          {else}
          <a href="#" onclick=" alert('{$Lang.RunCasesMaxValue}')">{$Lang.RunCases}</a>
          {/if}
          </td>
          <td style="text-align:right;border:0;width:50%">
            {$Lang.Pagination.Result}
            {$PaginationDetailInfo.FromNum}-{$PaginationDetailInfo.ToNum}/{$PaginationDetailInfo.RecTotal}
            &nbsp;
            {$PaginationDetailInfo.RecPerPageList}&nbsp;
            {$PaginationDetailInfo.PrePageLiteralLink}
            {$PaginationDetailInfo.NextPageLiteralLink}
          </td>
        </tr>
      </table>
      </td>
    </tr>
    <tr>
      <td colspan="6">
    <div id="ListSubTable" style="height:350px;overflow-y:scroll;overflow-x:auto">
    <table class="CommonTable CommonSubTable TableSubMode">
    <colgroup>
      {foreach from=$FieldsToShow key=Field item=FieldName}
      <col{if $QueryColumn eq $Field} class="SortActive"{/if}{if $QueryColumn eq 'CaseSeverity'}style="width:3%;"{/if}></col>
      {/foreach}
    <colgroup>
    <tr class="TabLink">
      {foreach from=$FieldsToShow key=Field item=FieldName}
      <th align="{if $Field == 'CaseID' || $Field == 'CasePriority' || $Field == 'DisplayOrder'}center{else}left{/if}">
        <nobr><a href="?OrderBy={$Field}|{$OrderByTypeList[$Field]}&QueryMode={$QueryMode}">{$FieldName}</a>{if $OrderByColumn == $Field}{$OrderTypeArrowArray[$OrderByType]}{/if}</nobr>
      </th>
      {/foreach}
    </tr>
    {foreach from=$CaseList item=CaseInfo}
    <tr>
      {foreach from=$FieldsToShow key=Field item=FieldName}
      <td align="{if $Field == 'CaseID' || $Field == 'CasePriority' || $Field == 'DisplayOrder' }center{else}left{/if}"{if $QueryColumn eq $Field} class="SortActive"{/if}>
        {assign var="FieldValue" value=$Field."Name"}
          {if $Field == 'CaseID' || $Field == 'CaseSeverity'}<nobr>{$CaseInfo[$FieldValue]|default:$CaseInfo[$Field]}</nobr>
          {elseif $Field == 'CaseTitle'}
          <a href="Case.php?CaseID={$CaseInfo.CaseID}" title="{$CaseInfo.CaseTitle}" class="FullLink" target="_blank"><nobr>{$CaseInfo.ListTitle}</nobr></a>
          {elseif $Field == 'OpenedDate' || $Field == 'AssignedDate' || $Field == 'LastEditedDate'}
            {if $CaseInfo[$Field] != $CFG.ZeroTime}
            <a href="?QueryMode={$Field}|{$CaseInfo[$Field]|date_format:"%Y-%m-%d"}"><nobr>{$CaseInfo[$Field]|date_format:"%Y-%m-%d"}</nobr></a>
            {/if}
          {else}
          <a href="?QueryMode={$Field}|{$CaseInfo[$Field]}"><nobr>{$CaseInfo[$FieldValue]|default:$CaseInfo[$Field]}</nobr></a>
          {/if}
      </td>
      {/foreach}
    </tr>
    {/foreach}
    </table>
    </div>
     </td>
    </tr>
  </table>
  </div>
<script>
    if({$QueryID}!='-1')
       parent.RightTopFrame.location = "SearchCase.php?QueryID=" + {$QueryID} + "&QueryTitle=" + encodeURI('{$QueryTitle}');
    else if('{$QueryTitle}'!='')
       parent.RightTopFrame.location = "SearchCase.php?QueryTitle=" + encodeURI('{$QueryTitle}');

</script>
</body>
</html>
