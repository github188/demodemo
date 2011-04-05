{include file="XmlHeader.tpl"}
{include file="Header.tpl"}
<body class="{$TestMode}Mode PaddingBoy">
  <table class="CommonTable ListTable {$Test}Mode">
    <tr>
      {foreach from=$FieldsToShow key=Field item=FieldName}
      <th align="{if $Field == 'DataID'}center{else}left{/if}">
        <nobr>{$FieldName}</nobr>
      </th>
      {/foreach}
    </tr>
    {foreach from=$DataList item=DataInfo}
    <tr>
      {foreach from=$FieldsToShow key=Field item=FieldName}
      <td>
        {assign var="FieldValue" value=$Field."Name"}
        <nobr>
          {$DataInfo[$FieldValue]|default:$DataInfo[$Field]}
        </nobr>
      </td>
      {/foreach}
    </tr>
    {/foreach}
    </table>
    </div>
     </td>
    </tr>
  </table>
</body>
</html>
