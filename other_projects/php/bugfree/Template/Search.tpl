{include file="Header.tpl"}
<body class="{$TestMode}Mode RMode" onload="initShowGotoBCR();">
{literal}
<script type="text/javascript">
//<![CDATA[
{/literal}
var FieldCount = {$FieldCount};
{foreach from=$AutoTextValue key=key item=item}
var {$key} = {$item}
{/foreach}

var OperatorListSelectItem = new Array();

{foreach from=$OperatorListSelectItem key=key item=item}
   OperatorListSelectItem[{$key}] = '{$item}';
{/foreach}

var ValueListSelectItem = new Array();

{foreach from=$ValueListSelectItem key=key item=item}
   ValueListSelectItem[{$key}] = '{$item}';
{/foreach}

{literal}
function getOffset(evt)
{
  if(navigator.userAgent.indexOf("MSIE")>0) { 
    return evt;
  }
  var target = evt.target;
  if (target.offsetLeft == undefined)
  {
    target = target.parentNode;
  }
  var pageCoord = getPageCoord(target);
  var eventCoord =
  {
    x: window.pageXOffset + evt.clientX,
    y: window.pageYOffset + evt.clientY
  };
  var offset =
  {
    offsetX: eventCoord.x - pageCoord.x,
    offsetY: eventCoord.y - pageCoord.y
  };
  return offset;
}

function getPageCoord(element)
{
  var coord = {x: 0, y: 0};
  while (element)
  {
    coord.x += element.offsetLeft;
    coord.y += element.offsetTop;
    element = element.offsetParent;
  }
  return coord;
}

function getEventOffset(evt)
{
  var msg = "";
  if (evt.offsetX == undefined)
  {
    var evtOffsets = getOffset(evt);
    msg += "offsetX: " + evtOffsets.offsetX + "; ";
    msg += "offsetY: " + evtOffsets.offsetY + "; ";
  }
  else
  {
    msg += "offsetX: " + evt.offsetX + "; ";
    msg += "offsetY: " + evt.offsetY + "; ";
  }

  return msg;
}

//]]>
</script>
{/literal}
<form id="Search{$TestMode}" name="Search{$TestMode}" action="{$TestMode}List.php" method='post' target='RightBottomFrame'>
{include file = "SaveQuery.tpl"}
   <div id="SearchBlankCover">
   <table id="SearchTable" class="CommonTable SearchTable {$TestMode}Mode TabelReset" onclick="hiddenDivSaveQuerySetTable()">
    <caption>
        {$Lang.QueryCondition}{if $QueryTitle != ''} - {$QueryTitle}{/if}
        <input type="checkbox" name="AutoComplete" id="AutoComplete" checked="checked" onclick="resetQueryForm();" class="NoBorder" style="display:none"/>
        <!--<label for="AutoComplete">Auto</label>-->
    </caption>
    <colgroup>
      <col span="1" width="7%">
      <col span="3" width="13%">
      <col span="1" width="7%">
      <col span="1" width="7%">
      <col span="3" width="13%">
    </colgroup>
    <tr>
      <th>1<span style="display:none">{$AndOrList.0}</span></th>
      <td>{$FieldList.0}</td>
      <td>{$OperatorList.0}</td>
      <td id="ValueTd0">{$ValueList.0}</td>
      <td rowspan="{math equation="x/y" x=$FieldCount y=2}" id="AndORTd">
        <input type="radio" name="AndOrGroup" id="AndGroup" value="AND" {$And} class="NoBorder"/>
        <label for="AndGroup">{$Lang.AndOr.And}</label><br />
        <input type="radio" name="AndOrGroup" id="OrGroup" value="OR" {$Or} style="border:0;" />
        <label for="OrGroup">{$Lang.AndOr.Or}</label>
      </td>
      <th>2<span style="display:none">{$AndOrList.1}</span></th>
      <td>{$FieldList.1}</td>
      <td>{$OperatorList.1}</td>
      <td id="ValueTd1">{$ValueList.1}</td>
    </tr>
    {foreach from=$AndOrList key=Key value=Value}{if $Key>1}{if $Key%2 == 0}
    <tr>
      <td>{$AndOrList[$Key]}</td>
      <td>{$FieldList[$Key]}</td>
      <td>{$OperatorList[$Key]}</td>
      <td id="ValueTd{$Key}">{$ValueList[$Key]}</td>{else}
      <td>{$AndOrList[$Key]}</td>
      <td>{$FieldList[$Key]}</td>
      <td>{$OperatorList[$Key]}</td>
      <td id="ValueTd{$Key}">{$ValueList[$Key]}</td>
    </tr>{/if}{/if}
    {/foreach}
    <tr>
      <th colspan="9" class="TopLine">
        <input type="submit" class="Btn" name="PostQuery" value="{$Lang.PostQuery}" onclick="document.Search{$TestMode}.action='{$TestMode}List.php';" />
        <input type="button" class="Btn" name="SaveQuery" value="{$Lang.SaveQuery}" onclick="document.Search{$TestMode}.action='SaveQuery.php';x=event.clientX-getOffset(event).offsetX;y=event.clientY-getOffset(event).offsetY;  showSaveQuery(x,y); xajax.$('QueryTitle').value = '{$QueryTitle}'; " />
        <!--<input type="reset"  onclick="location.reload(true);" />-->
        <input type="reset"  class="Btn" value="{$Lang.ResetQuery}" onclick="window.location.href= 'Search{$TestMode}.php?reset=1'" />
        <input type="hidden" name="QueryType" value="{$TestMode}" />
      </th>
    </tr>
  </table>
  </div>
</form>
{literal}
<script type="text/javascript">
//<![CDATA[
function resetQueryForm(OperatorListSelectItem,ValueListSelectItem)
{
    for(var i=0;i<FieldCount;i++)
    {
        setQueryForm(i,OperatorListSelectItem[i],ValueListSelectItem[i]);
    }
}
resetQueryForm(OperatorListSelectItem,ValueListSelectItem);


//]]
</script>
{/literal}
</body>
</html>
