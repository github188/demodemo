{include file="ReportHeader.tpl"}
<body onResize="reSize();" onload="reSize();" class="{$StatMode} PaddingBoy" style ="overflow:auto;">
  <table class="CommonTable ListTable BugMode" style="border:0">
    <tr>
      <td colspan="{$FieldToShowCount}" style="border:0">
    <div id="ListSubTable" style="height:290px;overflow:auto;">
{foreach from=$FCScriptList key=Key item=FCScript}
<fieldset style="border:1px solid #8AAFE1;">
<legend>{$FCLegendList[$Key]}</legend>
{$FCScript}
{$FCNoteList[$Key]}
</fieldset>
{if $Key < $FCScriptCount}<br />{/if}
{/foreach}
    </div>
     </td>
    </tr>
  </table>
{literal}
<script>
function reSize()
{
    var h1 = document.body.clientHeight;
    var h2 = document.documentElement.clientHeight;
    var isXhtml = (h2>0)?true:false;
    var body = (isXhtml && h2>h1)?document.documentElement:document.body;
    xajax.$('ListSubTable').style.height = body.clientHeight-13 + 'px';
}
</script>
{/literal}
</body>
</html>
