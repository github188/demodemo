{include file="Header.tpl"}
<body class="LMode" onload="initShowGotoBCR();">
{literal}
<script type="text/javascript">
//<![CDATA[
function dispayUserControlDiv(I)
{
    var display=false;
    for(j=0;j<4;j++)
    {
        if(xajax.$('UCTitle'+j))
        {
            if(j == I)
            {
                if(j != 3)
                {
                    xajax.$('UCTitle'+j).className = 'TabSelect';
                }
                displayObj('UCDiv'+j);
                display=true;
            }
            else
            {
                if(j != 3)
                {
                    xajax.$('UCTitle'+j).className = 'Tab';
                }
                hiddenObj('UCDiv'+j);
            }
        }
    }
    if(!display)
    {
        xajax.$('UCTitle'+1).className = 'TabSelect';
        displayObj('UCDiv'+1);
    }
}
/*==============================loading function==============================*/

//]]
</script>
{/literal}
  <div id="LeftUserControl" class="TabContainer">
    <h2 id="UCTitle0" class="Tab"><a href="javascript:void(0);" onclick="dispayUserControlDiv(0);TestCookie.write('TabSelectNum',0);">{$Lang.UCAssignedToMe}</a></h2>
    <h2 id="UCTitle1" class="TabSelect"><a href="javascript:void(0);" onclick="dispayUserControlDiv(1);TestCookie.write('TabSelectNum',1);">{$Lang.UCOpenedByMe}</a></h2>
    <h2 id="UCTitle2" class="Tab"><a href="javascript:void(0);" onclick="dispayUserControlDiv(2);TestCookie.write('TabSelectNum',2);">{$Lang.UCMyQuery}</a></h2>
    <h2 id="UCTitle3" class="Tab {$TestMode}Mode" style="border:0;padding:0 2px;background:none;"><a href="javascript:void(0);" onclick="xajax_xUpdateUserControl('{$TestMode}');return false;"><img src="Image/icon_refresh.png" /></a></h2>
   {if $TestMode == 'Bug'}
   <div id="UCDiv0" class="TabPage" style="display:none;">
    {foreach from=$AssignedList item=Item}
      {$Item.BugID}&nbsp;<a href="Bug.php?BugID={$Item.BugID}" title="{$Item.BugTitle}" target="_blank">{$Item.UCTitle}</a><br />
    {/foreach}
    </div>
    <div id="UCDiv1" class="TabPage" style="display:block;">
    {foreach from=$OpenedList item=Item}
      {$Item.BugID}&nbsp;<a href="Bug.php?BugID={$Item.BugID}" title="{$Item.BugTitle}" target="_blank">{$Item.UCTitle}</a><br />
    {/foreach}
    </div>
    <div id="UCDiv2" class="TabPage" style="display:none;">
    {foreach from=$QueryList item=Item}
      <div>
        <a href="?DelQueryID={$Item.QueryID}" title="{$Lang.Delete}" onclick="return confirm('{$Lang.ConfirmDelQuery}');" target="_self" style="color:#CC0000;text-decoration:none;font-size:11px;font-weight:bold;margin-right:0px;padding:0 5px;"><img src="Image/delete.gif"/></a>
        <a href="BugList.php?QueryID={$Item.QueryID}" title="{$Item.QueryTitle}">{$Item.QueryTitle}</a>
      </div>
    {/foreach}
    </div>
    {elseif $TestMode == 'Case'}
    <div id="UCDiv0" class="TabPage" style="display:none;">
    {foreach from=$AssignedList item=Item}
      {$Item.CaseID}&nbsp;<a href="Case.php?CaseID={$Item.CaseID}" title="{$Item.CaseTitle}" target="_blank">{$Item.UCTitle}</a><br />
    {/foreach}
    </div>
    <div id="UCDiv1" class="TabPage" style="display:block;">
    {foreach from=$OpenedList item=Item}
      {$Item.CaseID}&nbsp;<a href="Case.php?CaseID={$Item.CaseID}" title="{$Item.CaseTitle}" target="_blank">{$Item.UCTitle}</a><br />
    {/foreach}
    </div>
    <div id="UCDiv2" class="TabPage" style="display:none;">
    {foreach from=$QueryList item=Item}
      <div>
        <a href="?DelQueryID={$Item.QueryID}" title="{$Lang.Delete}" onclick="return confirm('{$Lang.ConfirmDelQuery}');" target="_self" style="color:#CC0000;text-decoration:none;font-size:11px;font-weight:bold;padding:0 5px;"><img src="Image/delete.gif" /></a>
        <a href="CaseList.php?QueryID={$Item.QueryID}" title="{$Item.QueryTitle}">{$Item.QueryTitle}</a>
      </div>
    {/foreach}
    </div>
    {elseif $TestMode == 'Result'}
    <div id="UCDiv0" class="TabPage" style="display:none;">
    {foreach from=$AssignedList item=Item}
      {$Item.ResultID}&nbsp;<a href="Result.php?ResultID={$Item.ResultID}" title="{$Item.ResultTitle}" target="_blank" style="color:{$Item.ResultValueColor}">{$Item.UCTitle}</a><br />
    {/foreach}
    </div>
    <div id="UCDiv1" class="TabPage" style="display:block;">
    {foreach from=$OpenedList item=Item}
      {$Item.ResultID}&nbsp;<a href="Result.php?ResultID={$Item.ResultID}" title="{$Item.ResultTitle}" target="_blank" style="color:{$Item.ResultValueColor}">{$Item.UCTitle}</a><br />
    {/foreach}
    </div>
    <div id="UCDiv2" class="TabPage" style="display:none;">
    {foreach from=$QueryList item=Item}
      <div>
        <a href="?DelQueryID={$Item.QueryID}" title="{$Lang.Delete}" onclick="return confirm('{$Lang.ConfirmDelQuery}');" target="_self" style="color:#CC0000;text-decoration:none;font-size:11px;font-weight:bold;margin-right:0px;padding:0 5px;"><img src="Image/delete.gif" /></a>
        <a href="ResultList.php?QueryID={$Item.QueryID}" title="{$Item.QueryTitle}">{$Item.QueryTitle}</a>
      </div>
    {/foreach}
    </div>
    {/if}
<div id="UCDiv3" class="TabPage" style="display:none;">
<strong>Loading...</strong><img src="Image/Loading.gif" />
</div>
</div>
<div id="ModuleListBottom"></div>
{literal}
<script type="text/javascript">
//<![CDATA[
dispayUserControlDiv(TestCookie.read('TabSelectNum'));
if(typeof(xajax) != "undefined")
{
    var LoadingMessage = xajax.$('UCDiv3');
    if(LoadingMessage)
    {
        xajax.loadingFunction = function(){dispayUserControlDiv(3);};
        function hideLoadingMessage()
        {
          dispayUserControlDiv(TestCookie.read('TabSelectNum'));
        }
        xajax.doneLoadingFunction = hideLoadingMessage;
    }
}
setInterval("xajax_xUpdateUserControl('{/literal}{$TestMode}{literal}')",300000);
//]]
</script>
{/literal}
<body>
</html>