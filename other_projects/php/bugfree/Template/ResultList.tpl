{include file="XmlHeader.tpl"}
{include file="Header.tpl"}
{include file="ResizeList.tpl"}
<script>if(parent.window==window)window.location='index.php?TestMode=Result';</script>
<body class="{$TestMode}Mode RMode" onResize="resizeList();" onclick="hiddenDivCustomSetTable()" onload="initShowGotoBCR();resizeList();">
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
            <a href="?Export=XMLFile">{$Lang.ExportCases}</a>
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
    <div id="ListSubTable" style="height:290px;overflow-y:scroll;overflow-x:auto">
    <table class="CommonTable CommonSubTable TableSubMode">
    <colgroup>
      {foreach from=$FieldsToShow key=Field item=FieldName}
      <col{if $QueryColumn eq $Field} class="SortActive"{/if}{if $QueryColumn eq 'ResultSeverity'}style="width:3%;"{/if}></col>
      {/foreach}
    <colgroup>
    <tr class="TabLink">
      {foreach from=$FieldsToShow key=Field item=FieldName}
      <th align="{if $Field == 'ResultID'}center{else}left{/if}">
        <nobr><a href="?OrderBy={$Field}|{$OrderByTypeList[$Field]}&QueryMode={$QueryMode}">{$FieldName}</a>{if $OrderByColumn == $Field}{$OrderTypeArrowArray[$OrderByType]}{/if}</nobr>
      </th>
      {/foreach}
    </tr>
    {foreach from=$ResultList item=ResultInfo}
    <tr class="ResultValue{$ResultInfo.ResultValue}">
      {foreach from=$FieldsToShow key=Field item=FieldName}
      <td align="{if $Field == 'ResultID' || $Field == 'CaseID'}center{else}left{/if}"{if $QueryColumn eq $Field} class="SortActive"{/if}>
        {assign var="FieldValue" value=$Field."Name"}
          {if $Field == 'ResultID' || $Field == 'ResultSeverity'}<nobr>{$ResultInfo[$FieldValue]|default:$ResultInfo[$Field]}</nobr>
          {elseif $Field == 'ResultTitle'}
          <span class="Title"><a href="Result.php?ResultID={$ResultInfo.ResultID}" title="{$ResultInfo.ResultTitle}" class="FullLink Title" target="_blank"><nobr>{$ResultInfo.ListTitle}</nobr></a></span>
          {elseif $Field == 'OpenedDate' || $Field == 'AssignedDate' || $Field == 'LastEditedDate'}
            {if $ResultInfo[$Field] != $CFG.ZeroTime}
            <a href="?QueryMode={$Field}|{$ResultInfo[$Field]|date_format:"%Y-%m-%d"}"><nobr>{$ResultInfo[$Field]|date_format:"%Y-%m-%d"}</nobr></a>
            {/if}
          {else}
          <a href="?QueryMode={$Field}|{$ResultInfo[$Field]}"><nobr>{$ResultInfo[$FieldValue]|default:$ResultInfo[$Field]}</nobr></a>
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
       parent.RightTopFrame.location = "SearchResult.php?QueryID=" + {$QueryID} + "&QueryTitle=" + encodeURI('{$QueryTitle}');
    else if('{$QueryTitle}'!='')
       parent.RightTopFrame.location = "SearchResult.php?QueryTitle=" + encodeURI('{$QueryTitle}');

</script>
</body>
</html>
