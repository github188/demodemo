{include file="XmlHeader.tpl"}
{include file="Header.tpl"}
{include file="ResizeList.tpl"}
<script>if(parent.window==window)window.location='index.php';</script>
<body class="{$TestMode}Mode RMode" onResize="resizeList();" onclick="hiddenDivCustomSetTable()" onload="initShowGotoBCR();resizeList();">
{include file="CustomField.tpl"}
  <div id="BlankCover">
    <table class="CommonTable TableMode">
      <tr>
        <td colspan="{$FieldToShowCount}" class="TdCaption">
          <table style="font-size:12px;width:98%;">
            <tr>
              <td style="text-align:left;width:50%">
                &nbsp;&nbsp;
                <a href="javascript:void(0);" id="CustomSetLink" onclick="x=530;y=event.clientY;showDivCustomSetTable(x,y);">
                    {$Lang.CustomDisplay}
                </a>&nbsp;|&nbsp;        
                <span id="VReport">
                  <a href="Report.php?ReportMode={$TestMode}" target="'{$TestMode}Report">
                    {$Lang.ReportForms}
                  </a>
                </span>&nbsp;|&nbsp;
                <a href="?Export=XMLFile">
                  {$Lang.ExportCases}
                </a>
              </td>
              <td style="text-align:right;width:50%">
                {$Lang.Pagination.Result} 
                {$PaginationDetailInfo.FromNum}-{$PaginationDetailInfo.ToNum}/{$PaginationDetailInfo.RecTotal}
                &nbsp;
                {$PaginationDetailInfo.RecPerPageList}
                &nbsp;
                {$PaginationDetailInfo.PrePageLiteralLink}
                {$PaginationDetailInfo.NextPageLiteralLink}
              </td>
            </tr>
          </table>
        </td>
      </tr>
      <tr>
        <td colspan="{$FieldToShowCount}">
          <div id="ListSubTable" style="height:350px;width:600px;overflow-y:scroll;overflow-x:auto">
            <table class="CommonTable CommonSubTable TableSubMode">
              <tr class="TabLink">
                {foreach from=$FieldsToShow key=Field item=FieldName}
                <th align="{if $Field == 'BugID' || $Field == 'BugSeverity'}center{else}left{/if}">
                  <a href="?OrderBy={$Field}|{$OrderByTypeList[$Field]}&QueryMode={$QueryMode}">
                  {if $Field == 'BugSeverity'}Sev{elseif $Field == 'BugPriority'}Pri{else}{$FieldName}{/if}
                  </a>
                  {if $OrderByColumn == $Field}{$OrderTypeArrowArray[$OrderByType]}{/if}
                </th>
                {/foreach}
              </tr>
              {foreach from=$BugList item=BugInfo}
              <tr class="BugStatus{$BugInfo.BugStatus}">
                {foreach from=$FieldsToShow key=Field item=FieldName}
                <td align="{if $Field == 'BugID' || $Field == 'BugSeverity' || $Field == 'BugPriority'}center{else}left{/if}"{if $QueryColumn eq $Field} class="SortActive"{/if}>
                  {assign var="FieldValue" value=$Field."Name"}
                  {if $Field == 'BugID' || $Field == 'BugSeverity' || $Field == 'BugPriority'}<nobr>{$BugInfo[$FieldValue]|default:$BugInfo[$Field]}</nobr>
                  {elseif $Field == 'BugTitle'}
                  <span class="Title">
                    <a href="Bug.php?BugID={$BugInfo.BugID}" title="{$BugInfo.BugTitle}" class="FullLink Title" target="_blank">
                      {$BugInfo.ListTitle}
                    </a>
                  </span>
                  {elseif $Field == 'OpenedDate' || $Field == 'AssignedDate' || $Field == 'ResolvedDate' || $Field == 'ClosedDate' || $Field == 'LastEditedDate'}
                  {if $BugInfo[$Field] != $CFG.ZeroTime}
                  <a href="?QueryMode={$Field}|{$BugInfo[$Field]|date_format:"%Y-%m-%d"}">
                    {$BugInfo[$Field]|date_format:"%Y-%m-%d"}
                  </a>
                  {/if}
                  {else}
                  <a href="?QueryMode={$Field}|{$BugInfo[$Field]}">
                    {$BugInfo[$FieldValue]|default:$BugInfo[$Field]}
                  </a>
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
       parent.RightTopFrame.location = "SearchBug.php?QueryID=" + {$QueryID} + "&QueryTitle=" + encodeURI('{$QueryTitle}');
    else if('{$QueryTitle}'!='')
       parent.RightTopFrame.location = "SearchBug.php?QueryTitle=" + encodeURI('{$QueryTitle}');
</script>
</body>
</html>